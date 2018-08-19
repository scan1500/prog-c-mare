#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <x86_64-linux-gnu/asm/ptrace-abi.h>
#include <x86_64-linux-gnu/sys/reg.h>
#include <sys/syscall.h> /* For SYS_write etc */

int main()
{
	pid_t child;
	long orig_eax, eax, params[3];
	int status, insyscall = 0;

	child = fork();
	if(child == 0) 
	{	
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl("./write_example.o", "write_example", NULL);
	}
	else 
	{
		while(1) 
		{
			wait(&status);
			if(WIFEXITED(status))
				break;

			orig_eax = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);

			if(orig_eax == SYS_write) 
			{
				if(insyscall == 0) 
				{/* Syscall entry */
					insyscall = 1;
					params[0] = ptrace(PTRACE_PEEKUSER, child, 8 * RDI, NULL);
					params[1] = ptrace(PTRACE_PEEKUSER, child, 8 * RSI, NULL);
					params[2] = ptrace(PTRACE_PEEKUSER, child, 8 * RDX, NULL);
					printf("Write called with %ld, %lx, %ld\n",params[0], params[1], params[2]);
				}
				else 
				{/* Syscall exit */
					eax = ptrace(PTRACE_PEEKUSER, child, 8 * RAX, NULL);
					printf("Write returned with %ld\n", eax);
					insyscall = 0;
				}
			}
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		}
	}
	return 0;
}

