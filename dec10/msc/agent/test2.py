import sys
import pexpect 
tc = pexpect.spawn('telnet 192.168.178.21 2030', logfile=sys.stdout.buffer, timeout=None)
tc.expect('.\n\r')
tc.sendline('login decwar')
tc.expect('.\n\r')
tc.sendcontrol('c')
tc.sendline('yes')
tc.expect('.\n\r')
tc.sendline('dir')
tc.expect('Total')
tc.expect('.\n\r')
tc.sendline('r gam:decwar')
tc.expect('Your name please: ')
tc.sendline('aa')
tc.expect('line: ')
tc.sendline('')
index = tc.expect(['Regular or Tournament', 'Which side'])
if index == 0:
    tc.sendline()
    tc.sendline()
    tc.sendline()
else: 
    pass
tc.sendline('fed')
tc.sendline('y')
tc.interact()
