[bits 64]

entry:
  mov rax, 0x01016e69626f6e69
  mov rcx, 0x0101010101010101
  xor rax, rcx
  push rax
  mov rdi, rsp
  pop rax
