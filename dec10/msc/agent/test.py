import sys, time
import pexpect 
flog = open('./log.txt', 'wb')
tc = pexpect.spawn('telnet 192.168.178.21 2030', timeout=None, echo=True, logfile=flog)
tc.expect('\r\n\n')
tmp = tc.readline().decode('utf-8')
conmsg = tc.readline().decode('utf-8')
# tc.send('kjob\r\n')
# tc.expect('\n\r\n')
tc.send('login decwar\r\n')
tc.expect('\n\r\n')
tc.sendcontrol('c')
tc.expect('.\n\r')
tc.send('dir\r\n')
tc.expect('Total of')
tc.expect('.\n\r')
tc.send('r gam:decwar\r\n')
tc.expect('Your name please: ')
tc.send('aa\r\n')
# tc.expect('\n\r\n')
tc.expect('Your name please: ')
tc.send('aa\r\n')
# tc.expect('.\n\r')
tc.expect('line: ')
tc.send('\r\n')
tc.expect('\n\r\n')
# tc.expect('Regular or Tournament game? (Regular) ')
# tc.send('\r\n')
# tc.expect('\n\r\n')
# tc.expect('Is the Romulan Empire involved in this conflict? (yes) ')
# tc.send('\r\n')
# tc.expect('\n\r\n')
# tc.expect('Do you want black holes? (no) ')
# tc.send('\r\n')
# tc.expect('\n\r\n')
tc.expect('(Federation or Empire) ')
tc.send('\r\n')
tc.expect('\n\r\n')
tc.send('Which vessel do you desire? ')
tc.send('y\r\n')
tc.expect('\n\r\n')



# 
# tc.send('dir\r\n')
# out2 = []
# while True: 
#     out2 += [tc.readline().decode('utf-8')]
#     if 'Total of' in out2[-1]: break 

pass

