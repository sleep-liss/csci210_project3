#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for (int i = 0; i < N; i++) {
		if (strcmp(cmd, allowed[i]) == 0) {
			return i + 1;
		}
	}

	return 0;
}

void print_help() {
	
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
    char *argv[21];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c
	//
	int argc = 0;
	char *token = strtok(line, " ");
	while (token != NULL && argc < 20) {
		argv[argc++] = token;
		token = strtok(NULL, " ");
	}
	argv[argc] = NULL;

	int cmd_index = isAllowed(argv[0]) - 1;

	if (cmd_index == -1){
		printf("NOT ALLOWED!\n");
		continue;
    }

	if (cmd_index == 9) {
		if (argc > 2) {
			printf("-rsh: cd: too many arguments\n");
		} else if (argc == 1 || chdir(argv[1]) !=0) {
			perror("-rsh: cd");
		}
	} else if (cmd_index == 10) {
		break;
	} else if (cmd_index == 11) {
		printf("The allowed commands are:\n");
		for (int i = 0; i < N; i++) {
			printf("%d: %s\n", i + 1, allowed[i]);
		}
	} else {
		pid_t pid;
		int status;
		if (posix_spawnp(&pid, argv[0], NULL, NULL, argv, environ) != 0) {
			perror("-rsh: posix_spawnp failed");
		} else {
			if (waitpid(pid, &status, 0) == -1) {
				perror("-rsh: waitpid failed");
			} else if (WIFEXITED(status)) {
				printf("Process exited with status %d\n", WEXITSTATUS(status));
			}
		}
	}

    }
    return 0;
 }

