.data
print_fmt:     .string "%ld "
print_fmt_ln:  .string "%ld \n"
.text
.globl sumar
sumar:
 pushq %rbp
 movq %rsp, %rbp
 movq %rdi,-8(%rbp)
 movq %rsi,-16(%rbp)
 subq $32, %rsp
 movq -8(%rbp), %rax
 pushq %rax
 movq -16(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 jmp .end_sumar
.end_sumar:
leave
ret
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 movq $3, %rax
 movq %rax, -8(%rbp)
 subq $8, %rsp
 movq $5, %rax
 movq %rax, -16(%rbp)
 subq $8, %rsp
 movq -8(%rbp), %rax
 mov %rax, %rdi
 movq -16(%rbp), %rax
 mov %rax, %rsi
call sumar
 movq %rax, -24(%rbp)
 subq $32, %rsp
 movq -24(%rbp), %rax
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
