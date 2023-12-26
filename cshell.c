#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

#define MAX_COMMAND_LENGTH 1024

void display_prompt()
{
	fflush(stdout);
}

void execute_command(const char *command)
{
	pid_t pid;
	int status;
	char *args[MAX_COMMAND_LENGTH];
	char *token;
	int arg_count;

	pid = fork();
	if (pid == 0)
	{
		token = strtok((char *)command, " ");
		while (token != NULL)
		{
			args[arg_count++] = token;
			token = strtok(NULL, " ");
		}
		args[arg_count] = NULL;

		execvp(args[0], args);
		perror(command);
		exit(EXIT_FAILURE);

	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
			printf("\n");
		else
			printf("Error: %s\n", command);
	}
	else
	{
		perror("simple_shell");
		_exit(EXIT_FAILURE);
	}
}

int main()
{
	char command_line[MAX_COMMAND_LENGTH];

	while (1)
	{
		display_prompt();

		if (fgets(command_line, sizeof(command_line), stdin) == NULL)
		{
			printf("\n");
			break;
		}
		command_line[strcspn(command_line, "\n")] = '\0';

		if (strcmp(command_line, "exit") == 0)
			break;

		execute_command(command_line);
	}
	return (0);
}
