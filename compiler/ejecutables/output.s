.data
print_fmt:     .string "%ld "
print_fmt_ln:  .string "%ld \n"
.text
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 movq $1, %rax
 movq %rax, -8(%rbp)
 subq $8, %rsp
 movq $1, %rax
 movq %rax, -16(%rbp)
 subq $32, %rsp
 movq -8(%rbp), %rax
 cmpq $0, %rax
 je else_0
 movq -16(%rbp), %rax
 movq %rsp, %rbx
 andq $15, %rbx
 subq %rbx, %rsp
 movq %rax, %rsi
 leaq print_fmt_ln(%rip), %rdi
 xor  %eax, %eax
 call printf@PLT
 addq %rbx, %rsp
 jmp endif_0
 else_0:
