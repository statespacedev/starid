"""for pycharm make sure the run config has terminal emulation switched on, this was a painful lesson!!!"""
import sys
import pexpect 

def main():
    tc = pexpect.spawn('telnet 192.168.178.21 2030', logfile=sys.stdout.buffer, timeout=None)
    tc.expect('.\n\r')
    tc.sendline('login decwar')
    index = tc.expect(['Unknown command', '\n\r\n'])
    if index == 0: tc.sendcontrol('c'); tc.expect('Do you really want'); tc.sendline('yes')
    else: pass
    tc.expect('.\n\r')
    tc.sendline('r gam:decwar')
    tc.expect('Your name please: ')
    tc.sendline('aa')
    tc.expect('line: ')
    tc.sendline('')
    index = tc.expect(['Regular or Tournament', 'Which side', 'There are'])
    if index == 0: tc.sendline(); tc.sendline(); tc.sendline()
    else: pass
    tc.sendline('fed')
    tc.sendline('y')
    waitfor(tc, 'Commands From TTY')
    tc.expect('>')
    tc.sendline('*password *mink / li / ti')
    res = [tc.readline().decode('utf-8')]
    while 'time of day' not in res[-1]: res.append(tc.readline().decode('utf-8'))   
    return

def waitfor(tc, aaa):
    line = tc.readline().decode('utf-8')
    while aaa not in line: line = tc.readline().decode('utf-8')
    return line
    
if __name__ == '__main__':
    main()
    