//https://www.techiedelight.com/convert-char-to-string-cpp/
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <netdb.h>
#include <map>
#include <utility>
#include <sstream>
//using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

struct recievewrapper{
    char testcode[256];
    char birthday[256];
};

int main(int argc, char *argv[])
{
    std::vector< std::vector<std::string> > myresults;
    struct recievewrapper input;
    int sockfd, newsockfd;
    //std::string portnos;
    int portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    //this comes from the link at the top of the page
    std::string newtestcode;
    std::string newbday;
    std::stringstream cs;
    std::stringstream bs;
    cs << input.testcode;
    cs >> newtestcode;
    bs << input.birthday;
    bs >> newbday;


    std::map<std::pair<std::string, std::string>,std::string> results;
    std::string result;//this is the N,P from the txt file
    std::string line;
    std::string space = " ";
    std::size_t pos =0;
    std::vector<std::string>temp;
    std::ifstream ifs("testResults20.txt");
    if(ifs.is_open())
    {
        //got this vector code from geeks for geeks - was super similar to the maps 
        while (!ifs.eof())//std::getline(ifs, line) - this missed the first two lines for some reason 
        {
            //std::cout << newtestcode << "\n";
            //std::cout << newbday << "\n";
            //std::cout << result << "\n";
            ifs >> newtestcode >> newbday >> result;
            temp = std::vector<std::string>();
            temp.push_back(newtestcode);
            temp.push_back(newbday);
            temp.push_back(result);
            myresults.push_back(temp);
            //results.insert(std::make_pair(std::pair<std::string,std::string>(newtestcode,newbday), result)); 
        }

        ifs.close();
    }
        std::cout << "Enter the server port number: ";
        std::cin >> portno;

     //this is where the socket is created
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
     {
        error("ERROR on binding");
     }
        
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while(true)
     {
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
    
    n = read(newsockfd, &input, sizeof(struct recievewrapper));//this is where I try to read from client
    std::string combokey;
    std::string testc;
    std::string nbday;
    std::string finalresult;
    std::stringstream qs;
    std::stringstream ws;
    qs << input.testcode;
    qs >> testc;
    ws << input.birthday;
    ws >> nbday;
    combokey =  testc + " " + nbday;
    for(int i =0; i < myresults.size(); i++)
        {
            if(myresults.at(i).at(0) == testc && myresults.at(i).at(1) == nbday)
            {
                finalresult = myresults.at(i).at(2);
            }
        }
    //std::cout << "test id: " << input.testcode << std::endl;//testing
    //std::cout << "birthdate: " << input.birthday << std::endl;//testing

    if(finalresult == "P")
    {
        std::cout << "Test result: " << finalresult << std::endl;
        n = write(newsockfd, "Your test result was POSITIVE", 29);
    }
    else if(finalresult == "N")
    {
        std::cout << "Test result: " << finalresult << std::endl;
        n = write(newsockfd, "Your test result was NEGATIVE.", 30);//does the number at the end need to be the exact characters
    }
    else
    {
        std::cout << "Test result: " << "X" << std::endl;
        n = write(newsockfd, "The test result you requested is not in our database.", 54);
    }

    /*if(combokey == results.find(std::make_pair(std::pair<std::string,std::string>(newtestcode,newbday))))//if test id and birthdate match then check P or N 
    {
        if(results.result == "N")//if user key matches map key then print value 
    {
        std::cout << "Test result: " << results.result << std::endl;
        n = write(newsockfd, "Your test result was NEGATIVE.", 30);//does the number at the end need to be the exact characters
    }
        else if(results.result == "P")//positive
    {
        std::cout << "Test result: " << results.result << std::endl;
        n = write(newsockfd, "Your test result was POSITIVE", 29);
    }
    }
    else//not in system
    {
        std::cout << "Test result: " << "X" << std::endl;
        n = write(newsockfd, "The test result you requested is not in our database.", 54);
    }*/


     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     if (n < 0) error("ERROR writing to socket");

     close(newsockfd);
     }
     close(sockfd);
     return 0; 
}