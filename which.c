#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>

#define BUF_LEN 512 * sizeof(char)

int is_cmd_exec(const char *cmd) {
    struct stat buf;
    if (stat(cmd, &buf) == 0 && buf.st_mode & S_IXUSR) {
        return 0;
    } else {
        return 1;
    }
}

int search_cmd(char *cmd, char *path, bool find_all) {
    int missed = 1;

    // Don't use $PATH if the command contains a slash
    if (strchr(cmd, '/')) {
        if (is_cmd_exec(cmd) == 0) {
            puts(cmd);
            missed = 0;
        }
    } else {
        char *temp = strdup(path);
        char *dir = strtok(temp, ":");

        // Go through each directory in PATH
        while (dir != NULL) {
            char *command = malloc(BUF_LEN);
            if (!command) {
            	free(temp);
                perror("malloc");
                exit(1);
            }
            memset(command, 0, BUF_LEN);

            // Construct command path to check
            snprintf(command, BUF_LEN, "%s/%s", dir, cmd);

            // Check if the command exists and is executable
            if (is_cmd_exec(command) == 0) {

            	// Show only 1 finding
            	if (find_all == false) {
                    puts(command);
                    free(command);

                    missed = 0;
                    break;
            	} else { // Show all findings
                    puts(command);
                    missed = 0;
            	}
            }

            // Search the next directory for the command
            dir = strtok(NULL, ":");
            free(command);
        }
        free(temp);
    }

    return missed;
}


void usage(void) {
    puts("usage: which cmd... [-a]");
}

int main(int argc, char *argv[]) {
    char *path;
    int ret = 0;

    if (argc < 2) {
        usage();
    } else {
        if ((path = getenv("PATH")) == NULL) {
            puts("$PATH not found");
        }

        bool all_findings = false;
        for (int i = 1; i < argc; i++) {
            if (all_findings == false) {
                for (int j = i; j < argc; j++) {
                    if (strncmp(argv[j], "-a", strlen(argv[j])) == 0) {
                        all_findings = true;
                    }
                }
            }

            if (strncmp(argv[i], "-a", strlen(argv[i])) == 0) {
                continue;
            }
            ret = search_cmd(argv[i], path, all_findings);
        }
    }

    return ret;
}
