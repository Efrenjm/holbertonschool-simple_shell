#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 256

void display_prompt()
{
	printf("($) ");
	fflush(stdout);
}

int execute_command(char *command)
{
	pid_t pid;
	int status;

	pid = fork();

	if (pid == 0)
	{
		execlp(command, command, (char *)NULL);
		perror("simple_shell");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (1);
	}
	else
	{
		perror("simple_shell");
		exit(EXIT_FAILURE);
	}
}

int main()
{
	char command_line[MAX_COMMAND_LENGTH];
	int status;

	while (1)
	{
		display_prompt();

		if (fgets(command_line, sizeof(command_line), stdin) == NULL)
			break;

		command_line[strcspn(command_line, "\n")] = '\0';

		status = execute_command(command_line);

		if (status != 0)
			printf("simple_shell: command failed with status %d\n", status);
		if (strcmp(command_line, "exit") == 0)
			break;
	}
	return (0);
}
