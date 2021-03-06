/*
Name: Sunghwa Cho
ID: 1001723723
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 100    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 10     // Mav shell only supports ten arguments

//To save comment that a user types
//It take int pointer, char arrary and cmd_str and char array as parameters
//This function will add 1 to int pointer to indicate this function is called.
void save_history(int *history_last_ptr,char history[15][MAX_COMMAND_SIZE],char *cmd_str,char history_over_15_store_1[MAX_COMMAND_SIZE] )
{
    //Save till 15th comments
    if(*history_last_ptr<15)
    {
        strcpy(history[*history_last_ptr],cmd_str);
    }
    //If a user put more than 15 comments
    else if(*history_last_ptr>=15)
    {
        //Store the first command so that it can be printed
        //if a user type !1
        //History[0] will be replace with history[1]
        //when a user put new command
        strcpy(history_over_15_store_1,history[0]);

        //Move 2 to 15 to 1 to 14
        //so that the new command can be placed in 15 
        int i;
        for(i=0;i<14;i++)
        {
            strcpy(history[i],history[i+1]);
        }

        //Place the new command in 15th position
        strcpy(history[14],cmd_str);


    }

    *history_last_ptr+=1;
}

//This function stores pids in an array and return showpids_last to indicate how many pids are stored
//It take int array, pid_t and int as arguments
int save_pids(int showpids[MAX_COMMAND_SIZE], pid_t pid, int showpids_last)
{
    //Save pids till 15th
    if(showpids_last<15)
    {
        showpids[showpids_last]=pid;
    }
    
    else if(showpids_last>=15)
    {
        //Move erase the oldest and add newest pid
        int i;
        for(i=0;i<14;i++)
        {
            showpids[i]=showpids[i+1];
        }

        showpids[14]=pid;
    }
    showpids_last+=1;

    return showpids_last;
}



//To ask user again to type an input.
//It takes cmd_str as parameters
void ask_input(char *cmd_str)
{
    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    printf ("msh> ");
    fgets (cmd_str, MAX_COMMAND_SIZE, stdin);
}



int main()
{
    //To save user's command
    char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
    
    //To save first 15 pids
    //It is used to print when "showpids" is typed
    int showpids[15]={0};
    //To indicate the last position of array 'showpids' 
    //After add pids to array 'showpids', 1 is added to showpids_last
    int showpids_last=0;

    //To save command that a user put
    //Used 2D array
    char history[15][MAX_COMMAND_SIZE]={""};
    //After add command to array 'history', 1 is added to history_last
    char history_over_15_store_1[MAX_COMMAND_SIZE]="";
    
    //To indicate the last position of array 'history' 
    int history_last = 0;


    int * history_last_ptr = &history_last;

    //To indicate if fork should be called or not
    //Some commend such as history, cd, showpids !n do not need to fork
    int fork_or_not=1;


    while( 1 )
    {
        // Print out the msh prompt
        printf ("msh> ");

        // Read the command from the commandline.  The
        // maximum command that will be read is MAX_COMMAND_SIZE
        fgets (cmd_str, MAX_COMMAND_SIZE, stdin);
        //Call save_history function to save user's input
        save_history(history_last_ptr,history,cmd_str,history_over_15_store_1);
        //Defalt setting for calling fork is 1
        //IF fork is not neccesary, it will be 0
        fork_or_not=1;


        // This while command will wait here until the user
        // inputs something since fgets returns NULL when there
        // is no input
        if(strcmp(cmd_str,"\n") == 0)
        {
            while(strcmp(cmd_str,"\n") == 0)
            {
                ask_input(cmd_str);
                save_history(history_last_ptr,history,cmd_str,history_over_15_store_1);
            }
        }
        //Stop the program if user types "quit" or "exit"
        else if(strcmp(cmd_str,"quit\n")==0||(strcmp(cmd_str,"exit\n")==0))
        {
            fork_or_not=0;
            return 0;
        }

   
        //Compare string with !1 and get strcmp
        //If 0<= strcmp <= 8
        //a user put !1 to !9
        else if((strcmp(cmd_str,"!1\n")>=0&&strcmp(cmd_str,"!1\n")<=8))
        {
            //if number of commands that a user put is less than 15
            if(*history_last_ptr<=15)
            {   
                //print error if no commend in history
                //Do not need to call fork.
                //Return to begining of first while loop
                if(strcmp(history[strcmp(cmd_str,"!1\n")],"")==0)
                {
                    printf("Command not in history.\n");
                    fork_or_not=0;
                }
                //If commend in the chosen position, copy that string to cmd_str so that it can be tokenize again
                else
                {
                    strcpy(cmd_str,history[strcmp(cmd_str,"!1\n")]);
                }
            }
            //if number of commands that a user put is greater than 15
            //the position is changed.
            else
            {
                //History[0] is not stored anymore in the new position
                //Use history_over_15_store_1 to bring history[0]
                if(strcmp(cmd_str,"!1\n")==0)
                {
                    strcpy(cmd_str,history_over_15_store_1);
                }
                else
                {
                strcpy(cmd_str,history[strcmp(cmd_str,"!1\n")-1]);
                }            
            }
            
        }

        //Compare string with !1 and get strcmp
        //If 38<= strcmp <= 43
        //a user put !10 to !15
        //Same algorithm for !1 to !9
        else if((strcmp(cmd_str,"!1\n")>=38&&strcmp(cmd_str,"!1\n")<=43))
        {
            //same algorithm for !1 to !9
            if(*history_last_ptr<=15)
            {
                if(strcmp(history[strcmp(cmd_str,"!1\n")-29],"")==0)
                {
                    printf("Command not in history.\n");
                    fork_or_not=0;
                }
                else
                {
                    strcpy(cmd_str,history[strcmp(cmd_str,"!1\n")-29]);
                }
            }
            else
            {
                strcpy(cmd_str,history[strcmp(cmd_str,"!1\n")-30]);
            }
        }

        //When a user types "showpids"
        //print list in array in 'showpids'
        //It diactivate fork.
        if(strcmp(cmd_str,"showpids\n")==0)
        {
 
            int i=0;
            while(i <=14 && showpids[i]!=0)
            {
                printf("%d: %d\n",i+1,showpids[i]);
                i+=1;
            }
            fork_or_not=0;
        }

        //When a user types "history"
        //print list in array in 'history'
        //It diactivate fork.
        else if(strcmp(cmd_str,"history\n")==0)
        {
            int i=0;
            while(i <=14 && strcmp(history[i],"")!=0)
            {
                printf("%d: %s",i+1,history[i]);
                i+=1;
            }

            fork_or_not=0;
        }

        else if(strcmp(cmd_str,"\n")==0)
        {
            fork_or_not=0;
        }


        //If user called system call
        //fork_or_not is 1,
        //execute the rest of the code. 
        if(fork_or_not)
        {
            /* Parse input */
            char *token[MAX_NUM_ARGUMENTS];

            int   token_count = 0;                                 
                                        
            // Pointer to point to the token
            // parsed by strsep
            char *argument_ptr;                                         
                                        
            char *working_str  = strdup( cmd_str );                

            // we are going to move the working_str pointer so
            // keep track of its original value so we can deallocate
            // the correct amount at the end
            char *working_root = working_str;

            // Tokenize the input stringswith whitespace used as the delimiter
            while ( ( (argument_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) && 
            (token_count<MAX_NUM_ARGUMENTS))
            {
                token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
                if( strlen( token[token_count] ) == 0 )
                {
                    token[token_count] = NULL;
                }
                token_count++;
            }

            //If a user type cd
            //excute this code
            if(strcmp(token[0],"cd")==0)
            {
                chdir(token[1]);
            }
            //If commends need to call fork
            else
            {

                pid_t pid = fork( );

                //Save 15 pids in showpids array 
                //if pid >0 
                if(pid !=0)
                {
                    showpids_last = save_pids(showpids,pid,showpids_last);
                }


                //Child processes
                if( pid == 0 )
                {
                    int ret = execvp( token[0], &token[0] ); 
                    //Print error if execvp returns -1
                    if( ret == -1 )
                    {
                        int i;

                        //To eleminate "\n" in the comment
                        char temp[MAX_COMMAND_SIZE] = "";
                       
                        for(i=0;i<strlen(cmd_str)-1;i++)
                        {
                            temp[i]=cmd_str[i];
                        }
                        
                        printf("%s : Command not found.\n",temp);
                    }

                    return 0;
                }

                //Wait till child process is done
                else 
                {
                    int status;
                    wait( & status );
                }

            }





            //Free dynamic allocated memory
            free( working_root );
        }




    }
    return 0;
}