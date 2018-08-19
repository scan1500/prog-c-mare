#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <x86_64-linux-gnu/asm/ptrace-abi.h>
#include <x86_64-linux-gnu/sys/reg.h>
#include <sys/syscall.h> /* For SYS_write etc */

void getData(pid_t child, long addr, char *str, int len)
{
	char* str_ptr=str;
	int i = 0;
	union u
	{
		long val;
		char chars[8];
	}data;

	for (i = 0; i < len; i++, str_ptr += 8)
	{
		data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * 8, NULL);
		memcpy(str_ptr, data.chars, 8);
	}
	str[len] = '\0';
}

int main(int argc, char *argv[])
{
	pid_t child;
	long orig_eax, eax, params[3];
	int status, insyscall = 0;

	child = fork();
	if (child == 0)
	{
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl(argv[1], argv[1], NULL);
	}
	else
	{
		printf("\n\n---TRACE-START---\n\n");
		while (1)
		{
			wait(&status);
			if (WIFEXITED(status))
				break;

			orig_eax = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);

			if (orig_eax == SYS_open)
			{
				if (insyscall == 0)
				{
					printf("---OPEN-ENTRY---\n");
					insyscall = 1;
					params[0] = ptrace(PTRACE_PEEKUSER, child, 8 * RDI, NULL); //FILENAME
					params[1] = ptrace(PTRACE_PEEKUSER, child, 8 * RSI, NULL); //OPENFLAGS
					params[2] = ptrace(PTRACE_PEEKUSER, child, 8 * RDX, NULL); //PERMISSIONS
					printf("Open called with %ld, %ld, %ld\n", params[0], params[1], params[2]);

					/* GET FILENAME */
					char *filename = (char *)calloc(31, sizeof(char)), *fn_ptr = filename;
					getData(child, params[0], filename, 30);
					printf("Filename: %s\n", filename);
				}
				else
				{
					eax = ptrace(PTRACE_PEEKUSER, child, 8 * RAX, NULL);
					printf("Open returned with %ld\n", eax);
					printf("---OPEN-EXIT---\n\n");
					insyscall = 0;
				}
			}
			else if (orig_eax == SYS_write)
			{
				if (insyscall == 0)
				{
					printf("---WRITE-ENTRY---\n");
					insyscall = 1;
					params[0] = ptrace(PTRACE_PEEKUSER, child, 8 * RDI, NULL); // FILE DESCRIPTOR
					params[1] = ptrace(PTRACE_PEEKUSER, child, 8 * RSI, NULL); // ADDRESS
					params[2] = ptrace(PTRACE_PEEKUSER, child, 8 * RDX, NULL); // LENGHT
					printf("Write called with %ld, %lx, %ld\n", params[0], params[1], params[2]);

					/* READING VALUES */
					char *str = (char *)malloc((params[2] + 1) * sizeof(char));
					getData(child, params[1], str, params[2]);

					printf("%s wants to write: %s\n", argv[1], str);

					if (strstr(str, "VIRUS"))
					{
						printf("!!! MALWARE DETECTED !!!\n\n -> exit(0)\n\n");
						//ptrace(PTRACE_KILL, child, NULL, NULL);
						kill(child, SIGKILL);
						exit(0);
					}
				}
				else
				{
					eax = ptrace(PTRACE_PEEKUSER, child, 8 * RAX, NULL);
					printf("Write returned with %ld\n", eax);
					printf("---WRITE-EXIT---\n\n");
					insyscall = 0;
				}
			}
			else if (orig_eax == SYS_read)
			{
				if (insyscall == 0)
				{
					printf("---READ-ENTRY---\n");
					insyscall = 1;
					params[0] = ptrace(PTRACE_PEEKUSER, child, 8 * RDI, NULL);
					params[1] = ptrace(PTRACE_PEEKUSER, child, 8 * RSI, NULL);
					params[2] = ptrace(PTRACE_PEEKUSER, child, 8 * RDX, NULL);
					printf("Read called with %ld, %lx, %ld\n", params[0], params[1], params[2]);
				}
				else
				{
					eax = ptrace(PTRACE_PEEKUSER, child, 8 * RAX, NULL);
					printf("Read returned with %ld\n", eax);
					printf("---READ-EXIT---\n\n");
					insyscall = 0;
				}
			}
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		}
		printf("---TRACE-END---\n\n\n");
	}
	return 0;
}

