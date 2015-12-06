global loader
extern main
extern initializeKernelBinary

extern 		main
extern 		initializeKernelBinary

extern		video_write_line
extern 		video_write_nl
extern 		keyboardHandler
extern 		irq0_handler

;SYSCALLS
extern 		sys_get_time
extern 		sys_set_time
extern 		sys_write
extern 		sys_read
extern 		sys_malloc
extern 		sys_calloc
extern 		sys_free
extern 		sys_keyboard_replace_buffer
extern 		sys_set_delay_screensaver
extern		sys_clear_screen
extern 		sys_show_screensaver

loader:

		call 		initializeKernelBinary		; Set up the kernel binary, and get thet stack address

		mov			rsp, rax					; Set up the stack with the returned address
		push 		rax

		call 		set_interrupt_handlers
		call 		init_pic
		call		main


hang:
		cli										; Clear interrupts
		hlt										; halt machine should kernel return
		jmp 		hang


IDTR64:											; Interrupt Descriptor Table Register
		dw 			256*16-1					; limit of IDT (size minus one) (4096 bytes - 1)
		dq 			0x0000000000000000			; linear address of IDT

create_gate:
		push 		rdi
		push 		rax

		shl 		rdi, 4						; quickly multiply rdi by 16
		stosw									; store the low word (15..0)
		shr 		rax, 16
		add 		rdi, 4						; skip the gate marker
		stosw									; store the high word (31..16)
		shr 		rax, 16
		stosd									; store the high dword (63..32)

		pop 		rax
		pop 		rdi
		ret

set_interrupt_handlers:

		mov 		rdi, 0x21
		mov 		rax, keyboard
		call 		create_gate

		mov 		rdi, 0x80
		mov 		rax, software_interruptions
		call		create_gate

		mov			rdi, 0x20
		mov			rax, pit_handler
		call		create_gate

		lidt 		[IDTR64]					; load IDT register
		ret

align 16
software_interruptions:							; Interrupciones de software, int 80h
		push 		rdi
		;push 		rax

		cmp 		rdi,	1
		jz			int_sys_get_time

		cmp 		rdi,	2
		jz			int_sys_set_time
		
		cmp			rdi, 3
		jz 			int_sys_read

		cmp			rdi, 4
		jz			int_sys_write

		cmp 		rdi, 5
		jz 			int_malloc

		cmp 		rdi, 6
		jz 			int_calloc

		cmp 		rdi, 7
		jz 			int_free

		cmp 		rdi, 8
		jz 			int_keyboard_replace_buffer

		cmp 		rdi, 	9
		jz 			int_sys_set_delay_screensaver

		cmp 		rdi, 	10
		jz 			int_sys_show_screensaver

		cmp			rdi, 11
		jz			int_sys_clear_screen

		cmp			rdi, 12
		jz			hang

		jmp 		soft_interrupt_done 		; La syscall no existe

int_sys_get_time:
		call 		prepare_params
		call 		sys_get_time
		jmp			soft_interrupt_done

int_sys_set_time:
		call 		prepare_params
		call 		sys_set_time
		jmp 		soft_interrupt_done

int_sys_write:
		call 		prepare_params
		call 		sys_write
		jmp 		soft_interrupt_done
int_sys_read:
		call 		prepare_params
		call 		sys_read
		jmp 		soft_interrupt_done

int_malloc:
		call 		prepare_params
		call 		sys_malloc
		jmp 		soft_interrupt_done

int_calloc:
		call 		prepare_params
		call 		sys_calloc
		jmp 		soft_interrupt_done

int_free:
		call 		prepare_params
		call 		sys_free
		jmp 		soft_interrupt_done

int_keyboard_replace_buffer:
		call 		prepare_params
		call 		sys_keyboard_replace_buffer
		jmp 		soft_interrupt_done

int_sys_set_delay_screensaver:
		call 		prepare_params
		call		sys_set_delay_screensaver
		jmp 		soft_interrupt_done

int_sys_clear_screen:
		call 		prepare_params
		call 		sys_clear_screen
		jmp			soft_interrupt_done

int_sys_show_screensaver:
		call 		prepare_params
		call		sys_show_screensaver
		jmp 		soft_interrupt_done

soft_interrupt_done:
		push 		rax
		mov 		al, 0x20				; Acknowledge the IRQ
		out 		0x20, al

		pop 		rax
		pop 		rdi
		iretq

prepare_params:
		mov 		rdi, rsi
		mov 		rsi, rdx
		mov 		rdx, rcx
		mov			rcx, r8

		ret

pit_handler:
		push 		rdi
		push 		rax

		call 		irq0_handler

		mov			al, 0x20					; Acknowledge the IRQ
		out 		0x20, al

		pop 		rax
		pop 		rdi
		iretq

align 16
keyboard:
		push 		rdi
		push 		rax

		xor			eax, eax

		in 			al, 0x60					; Get the scancode from the keyboard

		mov 		rdi, rax
		call 		keyboardHandler

keyboard_done:
		mov			al,  0x20					; Acknowledge the IRQ
		out 		0x20, al

		pop 		rax
		pop 		rdi
		iretq

init_pic:
	; Enable specific interrupts
	in al, 0x21
	mov al, 11111000b							; Enable Cascade, Keyboard
	out 0x21, al

	sti											; Enable interrupts
	ret
