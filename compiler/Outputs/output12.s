.data
print_fmt:     .string "%ld "
print_fmt_ln:  .string "%ld \n"
.text
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $16, %rsp
 movq $1, %rax
 movq %rax, 0(%rbp)
for_0:
 movq 0(%rbp), %rax
 pushq %rax
 movq $3, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setle %al
 movzbq %al, %rax
 cmpq $0, %rax
 je endfor_0
 movq 0(%rbp), %rax
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
 movq %rax, 0(%rbp)
for_1:
 movq 0(%rbp), %rax
 pushq %rax
 movq $10, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setle %al
 movzbq %al, %rax
 cmpq $0, %rax
 je endfor_1
 movq 0(%rbp), %rax
  negq  %rax
 movq %rsp, %rbx
 andq $15, %rbx
 subq %rbx, %rsp
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 xor  %eax, %eax
 call printf@PLT
 addq %rbx, %rsp
 movq 0(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movq %rax, 0(%rbp)
 jmp for_1
 endfor_1:
 jmp endif_0
 else_0:
endif_0:
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
