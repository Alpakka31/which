#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>

int is_cmd_exec(std::string cmd) {
    struct stat buf;
    if (stat(cmd.c_str(), &buf) == 0 && buf.st_mode & S_IXUSR) {
        return 0;
    } else {
        return 1;
    }
}

std::vector<std::string> split_str(std::string string, std::string delimeter) {
    std::vector<std::string> splitted;
    int start = 0;
    int end = string.find(delimeter);
    while (end != -1) {
        splitted.push_back(string.substr(start, end - start));
        start = end + delimeter.size();
        end = string.find(delimeter, start);
    }
    splitted.push_back(string.substr(start, end - start));
    return splitted;
}

int search_cmd(std::string cmd, std::string path, bool find_all) {
    int missed = 1;

    // Don't use $PATH if the command contains a slash
    if (cmd.find('/') != std::string::npos) {
        if (is_cmd_exec(cmd) == 0) {
            std::cout << cmd << std::endl;
            missed = 0;
        }
    } else {
        std::vector<std::string> dirs = split_str(path, ":");

        for (auto & dir : dirs) {
            std::string command = dir + "/" + cmd;
            if (is_cmd_exec(command) == 0) {
                if (find_all == false) {
                    std::cout << command << std::endl;
                    missed = 0;

                    break;
                } else {
                    std::cout << command << std::endl;
                    missed = 0;
                }
            }
        }
    }

    return missed;
}

void usage(void) {
    std::cout << "usage: which cmd... [-a]" << std::endl;
}

int main(int argc, char **argv) {
    char *path;
    int ret = 0;

    if (argc < 2) {
        usage();
    } else {
        if ((path = getenv("PATH")) == NULL) {
            std::cout << "$PATH not found" << std::endl;
            std::exit(1);
        }

        bool all_findings = false;
        for (int i = 1; i < argc; i++) {
            if (all_findings == false) {
                for (int j = i; j < argc; j++) {
                    std::string arg = argv[j];
                    if (arg == "-a") {
                        all_findings = true;
                    }
                }
            }

            std::string arg = argv[i];
            if (arg == "-a") {
                continue;
            }
            ret = search_cmd(arg, std::string(path), all_findings);
        }
    }
    
    return ret;
}
