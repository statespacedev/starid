import time
import random
import pexpect
import sys
import cli
from definitions import agents, ships

class Decwar:

    def __init__(self, *args, **kwargs):
        self.args, self.kwargs = args, kwargs
        self.name = kwargs['name']
        self.nomad = True if kwargs['name'] == 'nomad' else False
        
    def run(self):
        try:
            self.connect_to_dec10()
            self.start_decwar()
            self.tc.expect('Commands From TTY', timeout=10)
            self.tc.expect('>', timeout=10)
            if self.nomad:
                self.tc.sendline('*password *mink')
                self.tc.expect('>', timeout=10)
            self.gameloop()
        except: pass
        
    def gameloop(self):
        while True:
            if self.nomad:
                self.tc.sendline('tell all; Nomad is alive')
                self.tc.expect('>', timeout=10)
                targs = self.list()
            else:
                self.speak_randomly()
                targs = self.move()
            if not targs: return
            time.sleep(10)
            
    def start_decwar(self):
        self.tc.sendline('r gam:decwar')
        self.tc.expect('Your name please: ', timeout=10)
        self.tc.sendline(f'{self.name}')
        self.tc.expect('line: ', timeout=10)
        self.tc.sendline('')
        ndx1 = self.tc.expect(['DECWAR', 'Regular or Tournament', 'Federation or Empire', 'You will join', 'choose another ship?'], timeout=10)
        if ndx1 > 0:
            if ndx1 == 1: self.tc.sendline(); self.tc.sendline(); self.tc.sendline()
            if ndx1 < 3: self.tc.sendline() # join default side
            if ndx1 == 4: self.tc.sendline('yes')
            for ship in sorted(ships):
                ndx2 = self.tc.expect(['DECWAR', 'Which vessel'], timeout=10)
                if ndx2 == 0: break
                else: self.tc.sendline(ship)
        
    def speak_randomly(self):
        if self.name not in agents: return
        if random.uniform(0, 1) > .05: return 
        msg = random.choice(agents[self.name])
        self.tc.sendline(f'tell all; {msg}')
        self.tc.expect('>', timeout=10)

    def move(self):
        v, h = 0, 0
        while v == 0 and h == 0:
            v, h = random.randint(-1, 1), random.randint(-1, 1)
        self.tc.sendline(f'move relative {v} {h} / targets 10 / time')
        res = [self.tc.readline().decode('utf-8')]
        while 'time of day' not in res[-1]: res.append(self.tc.readline().decode('utf-8'))
        self.tc.expect('>', timeout=10)
        return res

    def list(self):
        self.tc.sendline('list / time')
        res = [self.tc.readline().decode('utf-8')]
        while 'time of day' not in res[-1]: res.append(self.tc.readline().decode('utf-8'))
        self.tc.expect('>', timeout=10)
        return res
    
    def shields(self, *args):
        if 'down' in args: self.tc.sendline('shields down')
        else: self.tc.sendline('shields up')
        self.tc.expect('>', timeout=10)
    
    def connect_to_dec10(self):
        self.tc = pexpect.spawn(f"telnet {self.kwargs['ip']} {self.kwargs['port']}", timeout=10, logfile=sys.stdout.buffer, echo=False)
        self.tc.expect('\r\n\n', timeout=10)
        self.tc.readline()
        connection_msg = self.tc.readline().decode('utf-8')
        self.tops10_login()
        self.tops10_sys()
        
    def tops10_login(self):
        """do the tops10 login command"""
        self.tc.send(f"login {self.kwargs['ppn']}\r\n")
        index = self.tc.expect(['Unknown command', 'Non-numeric coordinate', 'Please KJOB', '\n\r\n'], timeout=10)
        if index in [0, 1]: 
            self.tc.sendcontrol('c')
            ndx2 = self.tc.expect(['Do you really want', 'Use QUIT'], timeout=10)
            if ndx2 == 0: 
                self.tc.sendline('yes')
            else:
                self.tc.sendline('quit')
                self.tc.sendline('yes')
        elif index == 2: self.tops10_logout()
        else: pass
        self.tc.expect('.\n\r', timeout=10)
        
    def tops10_logout(self):
        """do the kjob command"""
        self.tc.send('kjob\r\n')
        self.tc.expect('\n\r\n', timeout=10)

    def tops10_dir(self):
        """dir command, mostly as a example of how commands can work"""
        self.tc.send('dir\r\n')
        out = []
        while True:
            out += [self.tc.readline().decode('utf-8')]
            if 'Total of' in out[-1]: break
        return out

    def tops10_sys(self):
        """dir command"""
        self.tc.send('sys\r\n')
        out = []
        while True:
            out += [self.tc.readline().decode('utf-8')]
            if 'Total Free' in out[-1]: break
        return out    
        
    # def __del__(self):
    #     try: self.tc.sendcontrol('c')
    #     except: pass
    #     try: self.tc.sendline('yes')
    #     except: pass
    #     try: self.tc.sendline('kjob')
    #     except: pass
    #     try: self.tc.sendcontrol(']')
    #     except: pass
    #     try: self.tc.sendline('close')
    #     except: pass

if __name__ == "__main__":
    args, kwargs = cli.main()
    dw = Decwar(*args, **kwargs).run()
    