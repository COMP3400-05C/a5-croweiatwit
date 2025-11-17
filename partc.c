


#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USAGE: partc FILEIN\n");
        return 1;
    }

    const char *input_filename = argv[1];
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }

    if (pid > 0) {

        close(pipefd[0]);

       
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            close(pipefd[1]);
            return 1;
        }
        close(pipefd[1]);

        char *parta_args[] = {
            "./parta",
            (char *)input_filename,
            NULL
        };

        int eret = execv("./parta", parta_args);
        (void)eret;
        perror("execv");
        return 1;
    } else {

        close(pipefd[1]);

      
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            close(pipefd[0]);
            _exit(1);
        }
        close(pipefd[0]);

        char *sort_args[] = {
            "sort",
            "-t,",
            "-k2",
            "-n",
            NULL
        };

        int eret = execvp("sort", sort_args);
        (void)eret;
        perror("execvp");
        _exit(1);
    }

    return 0;
}