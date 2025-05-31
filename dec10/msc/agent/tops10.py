import sys
import pexpect 
class Tops10:
    """a telnet connection for tops10 and decwar"""
    
    def __init__(self, hostip, hostport):
        self.tc = pexpect.spawn(f'telnet {hostip} {hostport}', timeout=None)
        # self.logfile = sys.stdout.buffer
        self.tc.expect('\r\n\n')
        self.tc.readline()
        self.connmsg = self.tc.readline()
        self.tc.send('kjob\r\n')
        self.tc.expect('\n\r\n')
        self.tc.send('login decwar\r\n')
        self.tc.expect('\n\r\n')
        
    def dir(self):
        self.tc.send('dir\r\n')
        out = []
        while True:
            out += [self.tc.readline().decode('utf-8')]
            if 'Total of' in out[-1]: break
        return out
            