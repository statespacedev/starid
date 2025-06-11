import time
import random
import pexpect
from timeout import timeout
from tops10 import Tops10
from definitions import agents, ships

class Decwar:

    def __init__(self, *args, **kwargs):
        self.args, self.kwargs = args, kwargs
        self.name = kwargs['name']
        self.team = kwargs['team']
        self.ship = kwargs['ship']
        self.nomad = True if kwargs['name'] == 'nomad' else False
        self.tops10 = Tops10(self.kwargs['ip'], self.kwargs['port'], self.kwargs['user'])
        self.tc = self.tops10.tc
        sys = self.tops10.sys()
        pass
    
    def __del__(self):
        try: self.tc.sendcontrol('c')
        except: pass
        try: self.tc.sendline('yes')
        except: pass
        try: self.tc.sendline('kjob')
        except: pass
        try: self.tc.sendcontrol(']')
        except: pass
        try: self.tc.sendline('close')
        except: pass
        
    def run(self):
        try:
            self.tc.sendline('r gam:decwar')
            self.tc.expect('Your name please: ')
            self.tc.sendline(f'{self.name}')
            self.tc.expect('line: ')
            self.tc.sendline('')
            ndx1 = self.tc.expect(['DECWAR', 'Regular or Tournament', 'Federation or Empire', 'You will join', 'choose another ship?'])
            if ndx1 > 0:
                if ndx1 == 1: self.tc.sendline(); self.tc.sendline(); self.tc.sendline()
                if ndx1 < 3: self.tc.sendline() # join default side
                if ndx1 == 4: self.tc.sendline('yes')
                for ship in sorted(ships):
                    ndx2 = self.tc.expect(['DECWAR', 'Which vessel'])
                    if ndx2 == 0: break
                    else: self.tc.sendline(ship)
            self.waitfor('Commands From TTY')
            self.tc.expect('>')
            if self.nomad:
                self.tc.sendline('*password *mink')
                self.tc.expect('>')
            self.shields()
            self.gameloop()
        except: pass
        self.__del__() # just being explicit, should happen in any case
        
    def gameloop(self):
        while True:
            # ndx = self.tc.expect([pexpect.TIMEOUT, 'line:', '>'], timeout=20)
            # if ndx < 2: return
            self.tc.expect('>', timeout=10)
            if self.nomad:
                self.tc.sendline('tell all; Nomad is alive')
                self.tc.expect('>')
                targs = self.list()
            else:
                self.speak_randomly()
                targs = self.move()
            if not targs: return
            time.sleep(10)
            
    @timeout()
    def speak_randomly(self):
        if self.name not in agents: return
        if random.uniform(0, 1) > .05: return 
        msg = random.choice(agents[self.name])
        self.tc.sendline(f'tell all; {msg}')

    @timeout()
    def move(self):
        v, h = 0, 0
        while v == 0 and h == 0:
            v, h = random.randint(-1, 1), random.randint(-1, 1)
        self.tc.sendline(f'move relative {v} {h} / targets 10 / time')
        res = [self.tc.readline().decode('utf-8')]
        while 'time of day' not in res[-1]: res.append(self.tc.readline().decode('utf-8'))
        return res

    @timeout()
    def list(self):
        self.tc.sendline('list / time')
        res = [self.tc.readline().decode('utf-8')]
        while 'time of day' not in res[-1]: res.append(self.tc.readline().decode('utf-8'))
        return res
    
    def shields(self, *args):
        if 'down' in args: self.tc.sendline('shields down')
        else: self.tc.sendline('shields up')

    def waitfor(self, targstr):
        line = self.tc.readline().decode('utf-8')
        while targstr not in line: line = self.tc.readline().decode('utf-8')
        return line
    