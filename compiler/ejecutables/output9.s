.data
print_fmt:     .string "%ld "
print_fmt_ln:  .string "%ld \n"
.text
.globl print_fun
print_fun:
 pushq %rbp
 movq %rsp, %rbp
 movq %rdi,-8(%rbp)
 subq $16, %rsp
 movq -8(%rbp), %rax
 movq %rsp, %rbx
 andq $15, %rbx
 subq %rbx, %rsp
 movq %rax, %rsi
 leaq print_fmt_ln(%rip), %rdi
 xor  %eax, %eax
 call printf@PLT
 addq %rbx, %rsp
.end_print_fun:
leave
ret
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $16, %rsp
 movq $2, %rax
 mov %rax, %rdi
call print_fun
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
