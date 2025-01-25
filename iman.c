#include "headers.h"

void iman(char * command) {
    char *tokens[2] = {NULL, NULL};
    int ind = 0;

    char *tok = strtok(command, " \t\n");
    while (tok != NULL && ind < 2) 
    {
        tokens[ind++] = strdup(tok);
        tok = strtok(NULL, " \t\n");
    }

    char *host = "man.he.net"; 
    char page[N];
    snprintf(page , N , "http://man.he.net/?topic=%s&section=all" , tokens[1]);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct hostent *server = gethostbyname(host);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); 

    if (server == NULL) 
    {
        perror("Host error");
        return;
    }
    if (sockfd < 0) 
    {
        perror("Socket error");
        return;
    }
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Connection error");
        return;
    }
    char request[10097];
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", page, host);
    if (send(sockfd, request, strlen(request), 0) < 0) {
        perror("Request sending error");
        return;
    }
    char response[N];
    ssize_t bytes_received;
    int flag = 0;
    while ((bytes_received = recv(sockfd, response, sizeof(response), 0)) > 0) 
    {
        if(flag == 1)
        {
            char* to_break = strstr(response , "AUTHOR");
            if(to_break!=NULL)
            {
                for(int i=0 ; i<to_break - response ; i++)
                {
                    printf("%c" , response[i]);
                }
                break;
            }
        }
        char* result = strstr(response , "NAME\n");
        if(result!=NULL)
        {
            flag = 1;
            char * next = strstr(result , "AUTHOR");
            if(next != NULL)
            {
                for(int i = 0; i < next - result; i++)
                {
                    printf("%c" , result[i]);
                }
                break;
            }
            else{
                printf("%s" , result);
            }
        }       
    }
    if(flag == 0)
    {
        fprintf(stderr, "%s is not a command", tokens[1]);
    }
    if (bytes_received < 0) {
        perror("Response receiving error");
        return ;
    }
    close(sockfd);
    return;
}
