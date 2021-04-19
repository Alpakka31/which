/*
 * Author: Alpakka31
 * Description: Search for the path of a given command from PATH and return it.
 * License: MIT
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN 128

/*
 * searchCmd
 * ---------
 *  Searches the given command from PATH
 *  environment variable.
 *
 *  cmd: the command to search for
 *
 *  returns: path to the given command if found
 *           otherwise NULL
 *
 */
char *searchCmd(char *cmd) {
    char *path = getenv("PATH");
    if (path == NULL) {
        printf("PATH wasn't found\n");
        return NULL;
    }

    char* pathdir = strtok(path, ":");

    // Go through each directory in PATH
    while (pathdir != NULL) {
        char *buf = malloc(BUF_LEN * sizeof(char));

        // Construct command path to search for
        snprintf(buf, BUF_LEN * sizeof(char), "%s/%s", pathdir, cmd);
        buf[BUF_LEN * sizeof(char) - 1] = '\0';

        int exists = 0;

        // Check if the command exists
        FILE *file = fopen(buf, "r");
        if (file != NULL) {
            fclose(file);
            exists = 1;
        } else {
            exists = 0;
        }

        // Return the command path if it was found
        if (exists == 1) {
            return buf;
        }

        // Go to the next directory
        pathdir = strtok(NULL, ":");

        free(buf);
    }

    return NULL;
}

/*
 * usage
 * ---------
 *  Tells how to use the program
 *
 *  returns: nothing
 *
 */
void usage(void) {
    printf("Usage: which <command>\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
    } else if (argc > 2) {
        printf("Too many arguments. Only one expected.\n");
        return 1;
    } else {
        char *res = searchCmd(argv[1]);
        if (res == NULL) {
            printf("which: command '%s' not found in PATH\n", argv[1]);
            return 1;
        }
        printf("%s\n", res);

        free(res);
    }

    return 0;
}
