.globl main
main:
    subq $8, %rsp           # align stack
    movq 8(%rsi), %rsi      # fetch argv[1]
    leaq fmt(%rip), %rdi    # format string
    xorl %eax, %eax         # no xmm used
    call printf             # print
    xorl %eax, %eax         # zero return value
    addq $8, %rsp           # restore stack
    ret                     # done

.data
fmt: .string "First argument is %s\n"