import sys
import pexpect 

class Tops10:
    """a telnet connection for tops10 and decwar"""
    
    def __init__(self, ip, port, user):
        self.ip, self.port, self.user = ip, port, user
        self.tc = pexpect.spawn(f'telnet {self.ip} {self.port}', timeout=10, logfile=sys.stdout.buffer, echo=False)
        self.tc.expect('\r\n\n', timeout=10)
        self.tc.readline()
        self.connmsg = self.tc.readline().decode('utf-8')
        self.login()
        
    def logout(self):
        """do the kjob command"""
        self.tc.send('kjob\r\n')
        self.tc.expect('\n\r\n', timeout=10)
        
    def login(self):
        """do the login command"""
        self.tc.send(f'login {self.user}\r\n')
        index = self.tc.expect(['Unknown command', 'Non-numeric coordinate', 'Please KJOB', '\n\r\n'], timeout=10)
        if index in [0, 1]: 
            self.tc.sendcontrol('c')
            ndx2 = self.tc.expect(['Do you really want', 'Use QUIT'], timeout=10)
            if ndx2 == 0: 
                self.tc.sendline('yes')
            else:
                self.tc.sendline('quit')
                self.tc.sendline('yes')
        elif index == 2: self.logout()
        else: pass
        self.tc.expect('.\n\r', timeout=10)
        
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
    