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
int fd[2]; // the file descriptor array

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
    char readbuffer[2];
    char stringArray[2];
    strcpy(stringArray, "@"); // what is happening here?

        // Child one prints the hour, min, sec every second
        // while the 2nd process hasn't told the first to terminate
        // keep printing the time
        
        //read(fd[0],readbuffer,sizeof(readbuffer));

        // wht does strcmp do?
        while(strcmp(readbuffer,stringArray) != 0) {
            system_time = time(NULL);
            local_time = localtime(&system_time);
            read(fd[0],readbuffer,sizeof(readbuffer));
            
            printf("+++++++your buffer is : %s\n", readbuffer);
            printf("-------your stringArray is : %s\n", stringArray);
            
            //printf("**Your system time is %d\n", system_time);
           // printf("**Your alarm time is %d\n",alarm_time);
            printf("The local time is : %d:%d:%d\n", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
            // if clock time matches the run time, print the ALARM time
            // compare the arg 2, 3, 4 against the tm hour/min/sec
            if(alarm_time == system_time) {
                printf(" <<<<<<<<  system and alarm time match\n");
            }
            sleep(1);
        }
        exit(1);
}

int create_child_two_countdown()
{
   time_t target_time;
   int timer;
    // SECOND process upon reaching the end of the RUNTIME
    //     notify using signal or pipe the FIRST child to terminate
       //printf("child two created!\n");
       system_time = time(NULL);
       target_time = system_time + run_time;
          
       for(timer = 0; timer < 30; timer++)
       {
            printf("\n"); 
       }
       //printf("CHILD TWO: system time is: %d\n", system_time);
       //printf("********************\n");
       //printf("CHILD TWO: target time is: %d\n", target_time);
       int b = 1;
       string flagString = "z";
       while(b){
           //printf("child two is checking system time vs target time\n");
           system_time = time(NULL);
           //printf("\n");
           if(system_time == target_time) {
               // time is up, tell child one to terminate
               //printf(">>>>>>>>>>> Time is Up! Signal CAUGHT<<<<<<<<<<< \n");
               // if child_one is terminated, terminate child two
               fflush(stdout); 
             //dup2(fd[1],1); // set to the write - this is the standard output
             // close(fd[0]); 
             //printf("@\n");
              write(fd[1], "@",2); // write to the fd pipe, tell one to terminate
               exit(1);
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
    //char fdValue = 
    pipe(fd); // create the pipe, and pass in the fd array
     // read from fd[0] 
     // write to fd[1]

    // read the run time the user wants the program to run for
    //       if no value then do a default of 30 seconds
    obtain_runtime_alarm(argc, argv);

     //read(fd[0],readbuffer,sizeof(readbuffer)i)
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
           // call child one method
           printf("child one is born\n");
           dup2(fd[0],0); // set to the read - this end of the pipe is standard input
           close(fd[1]); // close the write portion cause we dont need it here
           create_child_one_clock();
           //perror(create_child_one_clock()); // it failed
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
           printf("creating child two\n");
           dup2(fd[1],1); // set to the write - this is the standard output
           close(fd[0]);
           create_child_two_countdown();   
           //perror(create_child_two_countdown());
           exit(1);
        default:
            break;
    }
    //printf("******in main about to close the file descriptors\n");
    close(fd[0]);
    close(fd[1]);
    //printf(" ********* in main nust closed the file descriptors\n");
    // the main program will busy wait until all child process are done

    // Once both children are done, print a friendly message and
    // then main program wil then cleanly exit
    //printf("Assignment 3 is finished, main program exiting\n");
    int n = 2;
    int status;
    pid_t pid;
    while(n > 0){
        pid = wait(&status);
        printf("Child with PID %d exited with status 0x%x.\n", (long)pid, status);
    --n;

    }
    

    //wait(NULL);
    printf(" main is done\n");
    return 0;
}
