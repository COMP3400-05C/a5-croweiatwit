
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
      
        fprintf(stderr, "USAGE: partb FILEIN FILEOUT\n");
        return 1;
    }

    const char *input_filename  = argv[1];
    const char *output_filename = argv[2];

    int fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0770);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Redirect STDOUT -> file
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        close(fd);
        return 1;
    }

    close(fd);

  
    printf("Category,Count\n");
    fflush(stdout);

    // Prepare args for ./parta
    char *parta_args[] = {
        "./parta",
        (char *)input_filename,
        NULL
    };

    int eret = execv("./parta", parta_args);
    
    (void)eret;
    perror("execv");
    return 1;
}
