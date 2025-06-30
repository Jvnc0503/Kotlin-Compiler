.data
print_fmt:     .string "%ld "
print_fmt_ln:  .string "%ld \n"
.text
.globl suma
suma:
 pushq %rbp
 movq %rsp, %rbp
 movq %rdi,-8(%rbp)
 movq %rsi,-16(%rbp)
 movq %rdx,-24(%rbp)
 subq $32, %rsp
 movq -8(%rbp), %rax
 pushq %rax
 movq -16(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 pushq %rax
 movq -24(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 jmp .end_suma
.end_suma:
leave
ret
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $16, %rsp
 movq $1, %rax
 mov %rax, %rdi
 movq $2, %rax
 mov %rax, %rsi
 movq $3, %rax
 mov %rax, %rdx
call suma
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
