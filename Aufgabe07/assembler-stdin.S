.global _start

.data
    input: .byte 0
	linebreak: .byte 10

.text
    _start:

		mov (%rsp), %r12
		mov %r12, %r13

		//read() - get input
        mov  $1, %rdx              // length of input is 1 byte
        mov $input, %rsi         // set rsi to pointer to input
        mov  $0, %rax               // syscall 0 is read
        mov  $0, %rdi               // stdin has a file descriptor of 0
        syscall                     // make the system call

		loop:
		//write() -> write input
        mov  $1, %rdx              // length of input is 1 byte
        mov $input, %rsi         // set rsi to pointer to input
        mov  $1, %rax               // syscall 1 is write
        mov  $1, %rdi               // stdout has a file descriptor of 1
        syscall                     // make the system call

		//write() -> linebreak
        mov  $1, %rdx              // length of input is 1 byte
        mov  $linyebreak, %rsi         // set rsi to pointer to input
        mov  $1, %rax               // syscall 1 is write
        mov  $1, %rdi               // stdout has a file descriptor of 1
        syscall                     // make the system call

		dec %r12
		jnz loop 

		//exit() -> quit program
		mov  $60, %rax              // syscall 60 is exit
		mov  %r13, %rdi   
        syscall                     // make the system call