#include <stdio.h>
#include <string.h>
#include "code.c"




int main(void) {
    char user_input[2048];
    int run = 1;

    while(run){
    printf(": ");
    
    fgets(user_input, sizeof(user_input), stdin); // Read user input
        if(user_input && user_input[0] != '#'){
            run = parse_input(user_input);
        }

    }
    return 0;
}