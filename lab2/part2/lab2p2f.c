#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {

    printf("Be patient, the program will take around 7 seconds to run.\n");
    printf("At the end you can do \"cat results.out\" to see the result.\n");

    //
    // Add code here to pipe from ./slow 5 to ./talk and redirect
    // output of ./talk to results.out
    // I.e. your program should do the equivalent of ./slow 5 | talk > results.out
    // WITHOUT using | and > from the shell.
    //
    
    // slow sout>pipe>sin talk >pipe? 
    
    // 1: FD for result file
    int fp_out = open("./results.out", O_CREAT | O_WRONLY);

    // p[0] read, p[1] write
    int p[2];
    if (pipe(p) <0) {
        perror("lab2pf2c: ");
    }
    
    if (fork() != 0) {
        // parent
        close(p[1]); // close write
        wait(NULL); // wait for child to send 
        dup2(p[0], STDIN_FILENO);
        dup2(fp_out, STDOUT_FILENO);
        if (fork() == 0) {
            execlp("./talk", "talk", (char *) 0);
        } else {
            wait(NULL);
            close(fp_out);
            close(p[0]);
        }
    } else {
        close(p[0]); // close read
        dup2(p[1], STDOUT_FILENO); // stdout now goes to write pipe
        if (fork() == 0) {
            // execlp("./slow", "slow", (char *) 0);
            execlp("./slow", "slow", "5", NULL);
        } else {
            wait(NULL);
            // write(p[1], 1, EOF);
            close(p[1]);
        }
        // write(p[1], NULL, 1);
    }
}

