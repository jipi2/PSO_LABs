.intel_syntax noprefix
	.global main
	.text
main:
	# write message to stdout
	# read(1, message, 14)
    # TODO:

    mov rdi, 1
    lea rsi, message
	mov rdx, 14
	mov rax, 1

	syscall

	mov rax, 60
	syscall
	

	# exit program
	# exit(0)
	# TODO 
 
.section .rodata
message:
	.asciz "Hello, world!\n"