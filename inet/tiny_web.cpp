//
// Created by vienna on 1/8/2016.
//


#include <netinet/in.h>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "../sysio/sysio.h"
#include "sock.h"

#define HTTP_HEADER_SIZE 4096
#define IS_STATIC_REQUEST 1
#define IS_DYNAMIC_REQUEST 0

extern char **environ;

void doit(int fd);
void read_requesthdrs(rio_t *p_rio);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void client_error(int fd, const char *cause, const char *errnum, const char *shortmsg, const char *longmsg);

int main(int argc, char *argv[])
{
    int listenfd, clientfd, port;
    socklen_t client_addr_len;
    sockaddr_in client_addr;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);

    listenfd = open_listenfd(port);
    while (1) {
        client_addr_len = sizeof(client_addr);

        clientfd = Accept(listenfd, (SA *)&client_addr, &client_addr_len);

        doit(clientfd);
        close(clientfd);
    }

}


void doit(int clientfd)
{
    int is_static;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    struct stat file_stat;
    rio_t rio;

    rio_initb(&rio, clientfd);
    rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);
    if (strcasecmp(method, "GET") != 0) {
        // not equal to "GET"
        client_error(clientfd, method, "501", "Not Implemented", "The requested method does not implemented yet.");
        return ;
    }

    // just skip headers for now
    read_requesthdrs(&rio);

    is_static = parse_uri(uri, filename, cgiargs);
    if (stat(filename, &file_stat) < 0) {
        // not found
        client_error(clientfd, filename, "404", "Not Found", "File Not Found");
        return ;
    }

    if (is_static) {
        if (!S_ISREG(file_stat.st_mode) || !(S_IRUSR & file_stat.st_mode)) {
            client_error(clientfd, filename, "403", "Forbidden", "The file can not be read");
            return ;
        }

        serve_static(clientfd, filename, file_stat.st_size);
    } else {

        if (!S_ISREG(file_stat.st_mode) || !(S_IXUSR & file_stat.st_mode)) {
            client_error(clientfd, filename, "403", "Forbidden", "The cgi program can not be executed");
            return ;
        }

        serve_dynamic(clientfd, filename, cgiargs);
    }

}


void read_requesthdrs(rio_t *p_rio)
{
    char line[MAXLINE];

    while (rio_readlineb(p_rio, line, MAXLINE)) {

        printf("%s", line);

        if (strcmp(line, "\r\n") == 0) {
            // equal to "\r\n"
            break;
        }
    }
}


void client_error(
        int fd,
        const char *cause,
        const char *errnum,
        const char *shortmsg,
        const char *longmsg)
{
    char line[MAXLINE], body[HTTP_HEADER_SIZE];

    // build body
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=\"#ffffffff\">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s \r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web Server</em></hr>\r\n", body);

    // output response
    sprintf(line, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    rio_writen(fd, line, strlen(line));
    sprintf(line, "Content-type: text/html\r\n");
    rio_writen(fd, line, strlen(line));
    sprintf(line, "Content-length: %d\r\n\r\n", (int)strlen(body));
    rio_writen(fd, line, strlen(line));
    rio_writen(fd, body, strlen(body));
}



int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;

    strcpy(filename, ".");      // relative path
    strcat(filename, uri);

    // handle cgiargs
    if (strstr(uri, "cgi-bin")) {
        // found "cgi-bin", it is a dynamic request

        ptr = index(uri, '?');
        if (ptr) {
            strcpy(cgiargs, ptr+1);
            *ptr = '\0';
        } else {
            strcpy(cgiargs, "");
        }

        return IS_DYNAMIC_REQUEST;

    } else {

        strcpy(cgiargs, "");

        if (filename[strlen(filename) - 1] == '/') {
            // check '/' only for linux path
            strcat(filename, "index.html");
        }

        return IS_STATIC_REQUEST;
    }
}


void serve_static(int clientfd, char *filename, int filesize)
{

    void *base_addr;
    char response[HTTP_HEADER_SIZE], filetype[MAXLINE];
    
    // send response header
    get_filetype(filename, filetype);
    sprintf(response, "HTTP/1.0 200 OK\r\n");
    sprintf(response, "%sServer: Tiny Web Server\r\n", response);
    sprintf(response, "%sContent-length: %d\r\n", response, filesize);
    sprintf(response, "%sContent-type: %s\r\n\r\n", response, filetype);
    rio_writen(clientfd, response, strlen(response));
    

    int file_fd = open(filename, O_RDONLY);
    base_addr = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, file_fd, 0);
    rio_writen(clientfd, base_addr, filesize);
    close(file_fd);
    munmap(base_addr, filesize);
}

void get_filetype(char *filename, char *filetype)
{
    if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else
        strcpy(filetype, "text/plain");
}

void serve_dynamic(int clientfd, char *filename, char *cgiargs)
{
    char line[MAXLINE], *emptyargv[] = { NULL };

    // send header
    sprintf(line, "HTTP/1.0 200 OK\r\n");
    rio_writen(clientfd, line, strlen(line));
    sprintf(line, "Server: Tiny Web Server\r\n");
    rio_writen(clientfd, line, strlen(line));

    if (fork() == 0) {
        // child process
        setenv("QUERY_STRING", cgiargs, 1);
        dup2(clientfd, STDOUT_FILENO);
        execve(filename, emptyargv, environ);
    }

    wait(NULL);
}

