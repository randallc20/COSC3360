//Christopher Randall HW_2 COSC 3360
//Links used so far:
//http://www.cs.cmu.edu/~prs/15-441-F10/lectures/r01-sockets.pdf
//https://www.geeksforgeeks.org/socket-programming-cc/
//and the two links provided in the assignment pdf
//also I am using my last two grace days - so after this assignment I will have none left 
//assignment was due monday nov 15 - I turned in wednesday nov 17

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>



void error(const char *msg)
{
    perror(msg);
    exit(0);
}

struct sendwrapper
{
    char testcode[256];
    char birthday[256];
};

int main(int argc, char *argv[])
{
    struct sendwrapper input;
    int sockfd, portno, n;
    char buffer[1024];
    char hostname[1024];
    char transfer[1024];
    char temp1[1024];
    char temp2[1024];
    //std::string hostname;
    //std::string testcode;
    //std::string birthday;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    bzero(buffer, 255);

    std::cout << "Enter the server host name: ";
    std::cin >> hostname;
    server = gethostbyname(hostname);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    std::cout << "Enter the server port number: ";
    std::cin >> portno;

    // Step 1: Socket Creation 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
    if (sockfd < 0) 
        error("ERROR opening socket");

    // Step 2: Connection with a remote server using the server (localhost)
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    // Step 3: Post Connection Phase 
    std::cout << "Enter your test code: ";
    scanf("%s", temp1);
    strcpy(input.testcode, temp1);
    std::cout << "Enter your birthday in MMDDYYY format: ";
    scanf("%s",temp2);
    strcpy(input.birthday, temp2);
    n = write(sockfd, &input, sizeof(struct sendwrapper));
    n = read(sockfd, buffer, 255);
    std::cout << buffer;
    return 0;
}