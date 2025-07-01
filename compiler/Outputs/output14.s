.data
print_fmt:     .string "%ld "
print_fmt_ln:  .string "%ld \n"
.text
.globl factorial
factorial:
 pushq %rbp
 movq %rsp, %rbp
 movq %rdi,-8(%rbp)
 subq $16, %rsp
 movq -8(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 sete %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_0
 movq $1, %rax
 jmp .end_factorial
 jmp endif_0
 else_0:
endif_0:
 movq -8(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 mov %rax, %rdi
call factorial
 pushq %rax
 movq -8(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
 jmp .end_factorial
.end_factorial:
leave
ret
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $16, %rsp
 movq $8, %rax
 mov %rax, %rdi
call factorial
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
