section .modinfo
    __mod_license        db "license=GPL", 0
    __mod_author         db "author=SegFault42", 0
    __mod_description    db "description=hello world module in asm", 0

section .data
	init_msg db		"<6> init_module", 11, 0	;<6> for KERN_INFO
	cleanup_msg db	"<6> cleanup_module", 14, 0

section .text
	global init_module
	global cleanup_module

	extern printk

init_module:
	push rbp
	mov rbp, rsp

	mov rdi, init_msg
	call printk

	mov rsp, rbp
	pop rbp

cleanup_module:
	push rbp
	mov rbp, rsp

	mov rdi, cleanup_module
	call printk

	mov rsp, rbp
	pop rbp
