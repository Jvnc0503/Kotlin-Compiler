.data
print_fmt:     .string "%ld "
print_fmt_ln:  .string "%ld \n"
.text
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 movq $1, %rax
 movq %rax, 0(%rbp)
for_0:
 movq 0(%rbp), %rax
 pushq %rax
 movq $5, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setle %al
 movzbq %al, %rax
 cmpq $0, %rax
 je endfor_0
 movq 0(%rbp), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movq 0(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movq %rax, 0(%rbp)
 jmp for_0
 endfor_0:
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
