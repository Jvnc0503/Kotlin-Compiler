.data
print_fmt:     .string "%ld "
print_fmt_ln:  .string "%ld \n"
.text
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 movq $2, %rax
 pushq %rax
 movq $3, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 pushq %rax
 movq $3, %rax
 pushq %rax
 movq $3, %rax
 movq %rax, %rcx
 popq %rax
 cqto
 idivq %rcx
 pushq %rax
 movq $2, %rax
 pushq %rax
 movq $4, %rax
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movq %rax, -8(%rbp)
 subq $16, %rsp
 movq -8(%rbp), %rax
 movq %rsp, %rbx
 andq $15, %rbx
 subq %rbx, %rsp
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 xor  %eax, %eax
 call printf@PLT
 addq %rbx, %rsp
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
