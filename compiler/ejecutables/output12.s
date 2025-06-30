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
.globl sumaPerimetros
sumaPerimetros:
 pushq %rbp
 movq %rsp, %rbp
 movq %rdi,-8(%rbp)
 movq %rsi,-16(%rbp)
 subq $32, %rsp
 movq -8(%rbp),%rax
 movq 0(%rax), %rax
 pushq %rax
 movq -16(%rbp),%rax
 movq 0(%rax), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 jmp .end_sumaPerimetros
.end_sumaPerimetros:
leave
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
 subq $8, %rsp
 subq $24, %rsp
 leaq 0(%rsp), %rdi
 movq $3, %rax
 mov %rax, %rsi
 movq $5, %rax
 mov %rax, %rdx
call Rectangle$ctor
 movq %rdi,%rax
 movq %rax, -24(%rbp)
 subq $48, %rsp
 movq -8(%rbp), %rax
 mov %rax, %rdi
 movq -24(%rbp), %rax
 mov %rax, %rsi
call sumaPerimetros
 movq %rsp, %rbx
 andq $15, %rbx
 subq %rbx, %rsp
 movq %rax, %rsi
 leaq print_fmt_ln(%rip), %rdi
 xor  %eax, %eax
 call printf@PLT
 addq %rbx, %rsp
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
