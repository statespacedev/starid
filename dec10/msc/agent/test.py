import sys
import pexpect 
tc = pexpect.spawn('telnet 192.168.178.21 2030', timeout=None)
tc.logfile = sys.stdout.buffer

tc.expect('\r\n\n')

tmp = tc.readline().decode('utf-8')
conmsg = tc.readline().decode('utf-8')
# tc.sendcontrol('c')
# tc.sendcontrol('g')
# tmp = tc.readline().decode('utf-8')
# tmp = tc.readline().decode('utf-8')

# tc.expect('Please LOGIN\r\n\r\n.')
tc.send('kjob\r\n')
tc.expect('\n\r\n')
tc.send('login decwar\r\n')
tc.expect('\n\r\n')

tc.send('dir\r\n')
out = []
while True: 
    out += [tc.readline().decode('utf-8')]
    if 'Total of' in out[-1]: break 

tc.send('dir\r\n')
out2 = []
while True: 
    out2 += [tc.readline().decode('utf-8')]
    if 'Total of' in out2[-1]: break 

pass

