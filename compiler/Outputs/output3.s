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
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setg %al
 movzbq %al, %rax
 movq %rax, -8(%rbp)
 subq $8, %rsp
 movq $1, %rax
 pushq %rax
 movq $2, %rax
 pushq %rax
 movq $3, %rax
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
 pushq %rax
 movq $4, %rax
 movq %rax, %rcx
 popq %rax
 cqto
 idivq %rcx
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
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
endif_0:
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
