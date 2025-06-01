
class Decwar:
    
    def __init__(self, tops):
        self.tc = tops.tc
        tmp = self.tc.readline().decode('utf-8')
        self.tc.send('r gam:decwar\r\n')
        out = []
        while True:
            out += [self.tc.readline().decode('utf-8')]
            if 'Total of' in out[-1]: break
            
    def tmp(self):
        self.tc.expect('\n\r\n')
        self.tc.expect('You name please:\r\n')
        self.tc.sendline('noah\r\n')
            