#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

#define READ_END 0
#define WRITE_END 1

int main() {

    printf("Be patient, the program will take around 7 seconds to run.\n");
    printf("At the end you can do \"cat results.out\" to see the result.\n");

    int fp_out = open("./result.out", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fp_out < 0) {
        perror("Failed to open result.out");
        exit(EXIT_FAILURE);
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) < 0) {
        perror("lab2p2f: ");
    }

    int pid = fork();
    // child execute ./slow, parent execute ./talk
    if (pid == 0) {
        close(pipe_fd[READ_END]);
        // stdout now goes to pipe write end
        dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
        execlp("./slow", "slow", "5", NULL);
    } else {
        close(pipe_fd[WRITE_END]);
        // we always wait for child, i.e. ./slow to finish executing first
        waitpid(pid, NULL, WEXITED);
        // stdin now goes to pipe read end
        dup2(pipe_fd[READ_END], STDIN_FILENO);
        dup2(fp_out, STDOUT_FILENO);
        execlp("./talk", "talk", NULL);
    }
}

