#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 256

void display_prompt()
{
	fflush(stdout);
}

int execute_command(char *command, char *envp[])
{
	pid_t pid;
	int status;
	char **args = (char **)malloc(2 * sizeof(char *));
	
	if (args == NULL)
        {
            perror("simple_shell");
            _exit(EXIT_FAILURE);
        }
	args[0] = command;
        args[1] = NULL;
	
	pid = fork();
	if (pid == 0)
	{
		if (execve(command, args, envp) == -1)
		{
			perror("simple_shell");
			free(args);
			_exit(EXIT_FAILURE);
		}
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
		_exit(EXIT_FAILURE);
	}
	return (0);
}

int main()
{
	char command_line[MAX_COMMAND_LENGTH];
	int status;
	char *envp[] = {
        "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
        NULL
    };

	while (1)
	{
		display_prompt();

		if (fgets(command_line, sizeof(command_line), stdin) == NULL)
			break;

		command_line[strcspn(command_line, "\n")] = '\0';

		if (strcmp(command_line, "exit") == 0)
			break;

		status = execute_command(command_line, envp);

		if (status != 0)
			printf("simple_shell: command failed with status %d\n", status);
	}
	return (0);
}
