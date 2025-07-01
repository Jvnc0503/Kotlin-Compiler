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
 movq $4, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movq %rax, -8(%rbp)
 subq $8, %rsp
 movq $4, %rax
 movq %rax, -16(%rbp)
 subq $32, %rsp
 movq -8(%rbp), %rax
 pushq %rax
 movq -16(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
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
