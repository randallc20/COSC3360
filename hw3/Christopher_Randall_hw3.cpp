//Christopher Randall
//Assignment 3 - COSC 3360 - Paris
//Got a lot of help from Hussain

#include <vector>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <thread>
#include <stdio.h>
#include <sstream>
#include <condition_variable>

using namespace std;

//variables came from professors slides
static int nVisitorsOnPlatform, nVisitors, nHadToWait;
static pthread_mutex_t ladder;//originally named access - but that was causing tons of errors for some reason
static pthread_cond_t green = PTHREAD_COND_INITIALIZER;

struct Visitor{
    string name;
    int elapsedTime = 0; 
    int climbingTime = 0;
    int platformTime = 0;
    int goingDownTime = 0;
};

//this is all the logic/going up and down to the platform 
void*visitor(void*arg) {
    //came from professor
    Visitor v1; 
    v1 = *((Visitor*)arg);
    //or
    //struct Visitor* v1 = (Visitor*) v;
    cout << v1.name << " has arrive at the ladder:" << "\n";
    pthread_mutex_lock(&ladder);
    //from Hussain
    if (nVisitorsOnPlatform == 3)
    {
        nHadToWait++;
        pthread_cond_wait(&green, &ladder);
    }
    nVisitorsOnPlatform++;
    
    cout << v1.name << " is climbing the ladder: " << "\n";
    //sleep climb time
    sleep(v1.climbingTime);
    pthread_mutex_unlock(&ladder);

    cout << v1.name << " is on the platform: " << "\n";
    pthread_mutex_unlock(&ladder);
    //sleep the time on platform
    sleep(v1.platformTime);
    cout << v1.name << " decides to leave: " << "\n";
    pthread_mutex_lock(&ladder);//lock the ladder as they are going down

    cout << v1.name << " goes down the ladder: " << "\n";
    //sleep down time
    sleep(v1.goingDownTime);
    nVisitorsOnPlatform = nVisitorsOnPlatform - 1;
    cout << v1.name << " leaves: " << "\n";

    pthread_mutex_unlock(&ladder);
    pthread_cond_signal(&green);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_mutex_init(&ladder, NULL);
    vector<pthread_t> visitors = vector<pthread_t>();
    //stoi wasnt working - so I split them up 
    string tempName;
    int elapsedTime, climbTime, tempPlatformTime, downTime;
    //read in the file
    //
        while (cin >>  tempName >> elapsedTime >> climbTime >> tempPlatformTime >> downTime) 
        {
            sleep(elapsedTime);//sleep for interval delay - was in powerpoint
            Visitor x;
            x.name = tempName;
            x.climbingTime = climbTime;
            x.platformTime = tempPlatformTime;
            x.goingDownTime = downTime;
            //now you have to make a thread for every visitor
            //this is the parent thread
            pthread_t tid;
            pthread_create(&tid, NULL, visitor, (void *) &x);
            nVisitors++;//tracks the visitor amount based on lines in input file
            //store the thread ID
            visitors.push_back(tid);
        }
        //came from professor paris - need this
        for(int i =0; i < visitors.size(); i++)
        {
            pthread_join(visitors.at(i),NULL);
        }
        //print out simulation summary
        //X amount of visitors came to visit the platform 
        cout << nVisitors << " visitors came to the platform" << "\n";
        //X amount of them had to wait 
        cout << nHadToWait << " of them had to wait" << "\n";
    return 0;
}