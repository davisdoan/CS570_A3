/**
 * Group: Davis Doan(cssc1090) & Jon Verne(cssc1145)
 * CS 570 Operating Systems Summer 2017
 * Assignment 3: Interrupt, Clock, & Timer
 * Filename: main.c
 */

#include "header.h"
int run_time;
time_t alarm_time; 
int alarm_argument;
time_t system_time;
struct tm *local_time;
int create_child_two_countdown();
void handle_signal_up(int signal_num);
int flag = 0;
static volatile sig_atomic_t signal_caught = 0;

void obtain_runtime_alarm(int argc, char *argv[])
{
   if( argc == 3) {
     printf("two args provided\n");     
     run_time = atoi(argv[1]);
     alarm_argument = atoi(argv[2]);
   } else if (argc == 2){ // only one argument given 
     printf("only one argument provided\n");
     run_time = atoi(argv[1]);
     alarm_argument = 10;     
    } else {
        // set to default values cause none were provided
        printf("no arguments provided\n");
        run_time = 30;
        alarm_argument = 10;
    }
}


int create_child_one_clock()
{
    system_time = time(NULL); 
    alarm_time = system_time + alarm_argument;   
        // child succesfully created!
        printf("Child one is ready\n");
        
        // Child one prints the hour, min, sec every second
        // while the 2nd process hasn't told the first to terminate
        // keep printing the time
        while(flag == 0) {
            system_time = time(NULL);
            local_time = localtime(&system_time);
            printf("**Your system time is %d\n", system_time);
            printf("**Your alarm time is %d\n",alarm_time);
            printf("The local time is : %d:%d:%d\n", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
            sleep(1);
            // if clock time matches the run time, print the ALARM time
            // compare the arg 2, 3, 4 against the tm hour/min/sec
            if(alarm_time == system_time) {
                printf(" <<<<<<<<  system and alarm time match\n");
            }
        }
}

int create_child_two_countdown()
{
   time_t target_time;
    // SECOND process upon reaching the end of the RUNTIME
    //     notify using signal or pipe the FIRST child to terminate
       printf("child two created!\n");
       system_time = time(NULL);
       target_time = system_time + run_time;
       printf("CHILD TWO: system time is: %d\n", system_time);
       printf("CHILD TWO: target time is: &d\n", system_time);
       int b = 1;
       while(b){
           system_time = time(NULL);
           if(system_time == target_time) {
              
               signal(SIGHUP, handle_signal_up);
               
               // time is up, tell child one to terminate
               printf(">>>>>>>>>>> Time is Up! Signal CAUGHT<<<<<<<<<<< \n");
               // if child_one is terminated, terminate child two
               flag = 1;
               break;
           }
           
       }
}

void handle_signal_up(int signal_num)
{
    if(signal_num == SIGHUP) {
        signal_caught = 1;
    }
}

int main( int argc, char *argv[]){
    // read the run time the user wants the program to run for
    //       if no value then do a default of 30 seconds
    obtain_runtime_alarm(argc, argv);

    // create two child processes
    // first process implements a clock which prints the hour, min, and sec every second
    //       if the hour,min, sec matches the user ALARM time, PRINT THE ALARM TOO
    switch(fork())
    {
        case -1:
            printf("Creating child failed\n");
            exit(1);
            break;
        case 0:
           // call child one methodi
           create_child_one_clock();
           exit(1);
        default:
            break;
    }

    // SECOND process upon reaching the end of the RUNTIME
    //     notify using signal or pipe the FIRST child to terminate
    //     then the second child terminates itself
 
    switch(fork())
    {
        case -1:
            printf("Creating child failed\n");
            exit(1);
            break;
        case 0:
           create_child_two_countdown();   
           exit(1);
        default:
            break;
    }
    // the main program will busy wait until all child process are done

    // Once both children are done, print a friendly message and
    // then main program wil then cleanly exit
    //printf("Assignment 3 is finished, main program exiting\n");
    return 0;
}
