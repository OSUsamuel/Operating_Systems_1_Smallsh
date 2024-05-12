#include <stdio.h>
#include <string.h>
#include <signal.h> 
#include <sys/types.h>
#include "code.c"




int main(void) {
	struct sigaction sa_int, sa_stp;

    sa_int.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_int, NULL);

    sa_stp.sa_handler = &handle_SIGSTP;
    sigaction(SIGTSTP, &sa_stp, NULL);

 


    char user_input[2048];
    int run = 1;

    while(run){
    fflush(stdout);
    printf(": ");
    fflush(stdout);
    memset(&user_input, 0, sizeof(user_input));
    
    fgets(user_input, sizeof(user_input), stdin); // Read user input
            // Trim leading and trailing whitespace
        char *trimmed_input = strtok(user_input, "\t\n");

        if(trimmed_input && trimmed_input[0] != '#'){
            run = parse_input(user_input);
        }

    }
    return 0;
}