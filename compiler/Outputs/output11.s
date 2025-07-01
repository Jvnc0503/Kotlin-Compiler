.data
print_fmt:     .string "%ld "
print_fmt_ln:  .string "%ld \n"
.text
.globl Rectangle$ctor
Rectangle$ctor:
 movq %rsi, 0(%rdi)
 movq %rdx, 8(%rdi)
 movq 0(%rdi), %rax
 pushq %rax
 movq 8(%rdi), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 pushq %rax
 movq $2, %rax
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
 movq %rax, 16(%rdi)
ret
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 subq $24, %rsp
 leaq 0(%rsp), %rdi
 movq $2, %rax
 mov %rax, %rsi
 movq $4, %rax
 mov %rax, %rdx
call Rectangle$ctor
 movq %rdi,%rax
 movq %rax, -8(%rbp)
 subq $32, %rsp
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
