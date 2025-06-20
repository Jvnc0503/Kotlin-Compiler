.data
print_fmt:     .string "%ld "
print_fmt_ln:  .string "%ld \n"
.text
.globl fib
fib:
 pushq %rbp
 movq %rsp, %rbp
 movq %rdi,-8(%rbp)
 subq $16, %rsp
 movq -8(%rbp), %rax
 pushq %rax
 movq $2, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setl %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_0
 movq -8(%rbp), %rax
 jmp .end_fib
 jmp endif_0
 else_0:
 movq -8(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 mov %rax, %rdi
call fib
 pushq %rax
 movq -8(%rbp), %rax
 pushq %rax
 movq $2, %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 mov %rax, %rdi
call fib
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 jmp .end_fib
endif_0:
.end_fib:
leave
ret
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 movq $1, %rax
 movq %rax, -8(%rbp)
 subq $16, %rsp
while_1:
 movq -8(%rbp), %rax
 pushq %rax
 movq $50, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setl %al
 movzbq %al, %rax
 cmpq $0, %rax
 je endwhile_1
 movq -8(%rbp), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movq -8(%rbp), %rax
 mov %rax, %rdi
call fib
 movq %rax, %rsi
 leaq print_fmt_ln(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movq -8(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movq %rax, -8(%rbp)
 jmp while_1
endwhile_1:
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
