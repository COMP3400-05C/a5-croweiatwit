#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    enum { BUFSIZE = 8 };
    char buffer[BUFSIZE];

    if (argc != 2) {
        fprintf(stderr, "USAGE: parta FILEIN\n");
        return 1;
    }

    const char *filename = argv[1];

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    long upper_count  = 0;
    long lower_count  = 0;
    long number_count = 0;
    long space_count  = 0;
    long other_count  = 0;

    while (1) {
        ssize_t bytes_read = read(fd, buffer, BUFSIZE);
        if (bytes_read < 0) {
            perror("read");
            close(fd);
            return 1;
        }
        if (bytes_read == 0) {
            // End of file
            break;
        }

        for (ssize_t i = 0; i < bytes_read; i++) {
            unsigned char c = (unsigned char)buffer[i];
            if (isupper(c)) {
                upper_count++;
            } else if (islower(c)) {
                lower_count++;
            } else if (isdigit(c)) {
                number_count++;
            } else if (isspace(c)) {
                space_count++;
            } else {
                other_count++;
            }
        }
    }

    close(fd);

    // Output format must match exactly
    printf("Upper,%ld\n",  upper_count);
    printf("Lower,%ld\n",  lower_count);
    printf("Number,%ld\n", number_count);
    printf("Space,%ld\n",  space_count);
    printf("Others,%ld\n", other_count);

    return 0;
}