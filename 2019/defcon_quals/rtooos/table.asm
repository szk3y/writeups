[bits 64]
[org 0x55]

  ;times 0x55 db 0x90
print_byte:
  call [0x77777000]
  ret
  nop
  nop
  nop
unused_func:
  times 9 db 0x90
read_buf:
  call [0x77777008]
  ret
  nop
  nop
  nop
  nop
  nop
println:
  call [0x77777010]
  ret
  nop
  nop
do_ls:
  jmp do_ls_spare
  nop
  nop
  nop
  nop
  nop
do_cat:
  call [0x77777020]
  ret
do_ls_spare:
  call [0x77777018]
  ret
