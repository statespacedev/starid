import sys
import pexpect 
class Tops10:
    """a telnet connection for tops10 and decwar"""
    
    def __init__(self, ip, port):
        self.ip, self.port = ip, port
        self.tc = pexpect.spawn(f'telnet {self.ip} {self.port}', timeout=None, logfile=sys.stdout.buffer, echo=False)
        self.tc.expect('\r\n\n')
        self.tc.readline()
        self.connmsg = self.tc.readline().decode('utf-8')
        # self.logout()
        self.login()
        
    def logout(self):
        """do the kjob command"""
        self.tc.send('kjob\r\n')
        self.tc.expect('\n\r\n')
        
    def login(self):
        """do the login command"""
        self.tc.send('login decwar\r\n')
        self.tc.expect('\n\r\n')
        
    def dir(self):
        """dir command, mostly as a example of how commands can work"""
        self.tc.send('dir\r\n')
        out = []
        while True:
            out += [self.tc.readline().decode('utf-8')]
            if 'Total of' in out[-1]: break
        return out
            
    def sys(self):
        """dir command"""
        self.tc.send('sys\r\n')
        out = []
        while True:
            out += [self.tc.readline().decode('utf-8')]
            if 'Total Free' in out[-1]: break
        return out
