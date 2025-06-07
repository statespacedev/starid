from .tops10 import Tops10

class DecwarMonitor:
    
    def __init__(self, ip, port, ppn):
        self.tops10 = Tops10(ip, port, ppn)
        self.tc = self.tops10.tc
        
    def enter_game(self):
        self.tc.sendline('r gam:decwar')
        self.tc.expect('Your name please: ')
        self.tc.sendline('aa')
        self.tc.expect('line: ')
        self.tc.sendline('')
        index = self.tc.expect(['Regular or Tournament', 'Which side', 'There are'])
        if index == 0:
            self.tc.sendline(); self.tc.sendline(); self.tc.sendline()
        else:
            pass
        self.tc.sendline('fed')
        self.tc.sendline('y')
        self.waitfor('Commands From TTY')
        self.tc.expect('>')
        
    def super_list(self):
        self.tc.sendline('*password *mink / li / ti')
        res = [self.tc.readline().decode('utf-8')]
        while 'time of day' not in res[-1]: res.append(self.tc.readline().decode('utf-8'))  
        return res
    
    def waitfor(self, targstr):
        line = self.tc.readline().decode('utf-8')
        while targstr not in line: line = self.tc.readline().decode('utf-8')
        return line