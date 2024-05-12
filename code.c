#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>

const int MAX_BG = 512;
static int bg_processes[512];
static int finished[512];

pid_t foreground_pid = 0;

int foreground_exit_status = 0;
int foreground_mode = 0;





struct command_line{
    char* command;
    char* args[512];
    char* input_file;
    char* output_file;
    int background;
};

void handle_SIGINT(int signo){
	

    
    // Convert integer to string
    char buffer[20]; // Assuming a maximum of 20 digits
    int length = snprintf(buffer, sizeof(buffer), "terminated by signal %d\n", signo);
    
    // Write the string to standard output
    write(1, buffer, length);
    

}


void handle_SIGSTP(int signo){
    char* enter_fg_mode = "\nEntering foreground-only mode (& is now ignored)\n: ";
    char* exit_fg_mode = "\nExiting foreground-only mode\n: ";

    if(foreground_mode){
        write(1, exit_fg_mode, strlen(exit_fg_mode));
        foreground_mode = 0;
    }else {
        write(1, enter_fg_mode, strlen(enter_fg_mode));
        foreground_mode = 1;
    }

}









// Function to replace a substring with another string in a given string
int replaceSubstring(char *string, const char *substring, const char *replacement) {
    char *pos = strstr(string, substring); // Find the position of the substring
    if (pos != NULL) {
        // Calculate the length of the remaining string after the substring
        size_t remainingLength = strlen(pos + strlen(substring));
        // Move the remaining string to create space for the replacement
        memmove(pos + strlen(replacement), pos + strlen(substring), remainingLength + 1);
        // Copy the replacement into the position of the substring
        memcpy(pos, replacement, strlen(replacement));
    } else{
        return 1;
    }
        replaceSubstring(string, substring, replacement);

}






int interpret(struct command_line* cmd){
    char* command = cmd->command;

        struct sigaction sa;

        sa.sa_handler = SIG_IGN;

        sigaction(SIGINT, &sa, NULL);





    if(strcmp(command, "exit") == 0){
        //Handles the exit case
        return 0;

    } else if(strcmp(command, "cd") == 0){
        //Handles the change directory case
       

        if(cmd->args[0] != NULL){
            cmd->args[0][strcspn(cmd->args[0], "\n")] = 0;
            chdir(cmd->args[0]);
        }  else {
            chdir(getenv("HOME"));
        }
    
        

    } else if(strcmp(command, "status") == 0){
        printf("Exit status %d\n", foreground_exit_status);
        fflush(stdout);
        //Handles the status case

    } else{
        //Handles everything else

        
        
        

        int pid = fork();
        if(pid != 0){
            if(cmd->background){
                    for(int i = 0; i < MAX_BG; i++){
                        if(bg_processes[i] == 0){
                            bg_processes[i] = pid;
                            printf("Successfully added process %d to background\n", bg_processes[i]);
                            break;
                        }
                    }
                }
               
            } 


        if(pid == 0){
            foreground_pid = getpid();
            sa.sa_handler = SIG_DFL;
            sigaction(SIGINT, &sa, NULL);



            

            int count = 0;
            while(cmd->args[count] != NULL){
                count++;
            }

            char* argument_list[count + 2];
            argument_list[0] = cmd->command;
            for(int i = 0; i < count; i++){
                argument_list[i+1] = cmd->args[i];
            }
            argument_list[count + 1] = NULL;


            int index = 0;


            if(cmd->output_file){
                int output_file = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                int result = dup2(output_file, 1);
                if(result == -1){
                    perror("dup2");
                } 
                close(output_file);
            } 

            if(cmd->input_file){
                int input_file = open(cmd->input_file, O_RDONLY);
                int result = dup2(input_file, 0);
                if(result == -1){
                    perror("dup2");
                } 
                close(input_file);
            }
            
            

            int status_code = execvp(command, argument_list);
            if (status_code == -1) {
                perror("execvp"); // Print an error message if execvp fails
            }
            exit(status_code);
                        
            return 1;
        
        }else {



            int status; 
            
            for(int i = 0; i < MAX_BG; i++){
                // printf("%d", bg_processes[i]);
                if(bg_processes[i] != 0 && waitpid(bg_processes[i], &finished[i], WNOHANG) != 0){
                    printf("Process %d has terminated\n", bg_processes[i]);
                    bg_processes[i] = 0;
                }
                    fflush(stdout);
            }

            

            if(!cmd->background){
                waitpid(0, &status, 0);
                foreground_exit_status = status;
                if(status != 0){
                printf("terminated by signal %d\n", status);
                }
                fflush(stdout);
            }
            
        }
        

        }
       

    

    return 1;
}




int parse_input(char* user_input){



    struct command_line* cmd = malloc(sizeof(struct command_line));
    char* token;
    char background;
    int count = 0;
    user_input[strcspn(user_input, "\n")] = 0;
    

    


    char* start = user_input;
    char pid[25];
    sprintf(pid, "%d", getpid());
    replaceSubstring(user_input, "$$", pid);
    


    
    


    token = strtok(user_input, " ");
    cmd->command = token;
    
    token = strtok(NULL, " ");


    

    //For arguments
    while(token != NULL){
        // printf("Token to be parsed: %s\n",token);


        // Input file


        if(strcmp(token, "<") == 0){
                token = strtok(NULL, " ");
                cmd->input_file = token;
                token = strtok(NULL, " ");
        //Output file
            } else if(strcmp(token, ">") == 0){
                token = strtok(NULL, " ");
                cmd->output_file = token;
                token = strtok(NULL, " ");
            } else{
        
                cmd->args[count] = token;         
                
                background = token[0];
                token = strtok(NULL, " ");
                fflush(stdout);
                count++;
            }
    }
    cmd->args[count] = NULL;
    

    if(background == '&'){
        if(!foreground_mode){
            cmd->background = 1;
            cmd->args[count-1] = NULL;
        } else {
            cmd->args[count-1] = NULL;
        }


    } else{
        cmd->background = 0;

    }

    int exit = interpret(cmd);
    
    cmd->output_file = NULL;
    cmd->input_file = NULL; 

    free(cmd);

    return exit;
};



