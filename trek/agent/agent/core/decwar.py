import time
import random
from .timeout import timeout
from .tops10 import Tops10
from .definitions import agents

class Decwar:

    def __init__(self, *args, **kwargs):
        self.args, self.kwargs = args, kwargs
        self.name = kwargs['name']
        self.team = kwargs['team']
        self.ship = kwargs['ship']
        self.nomad = True if kwargs['name'] == 'nomad' else False
        pass

    def gameloop(self):
        while True:
            if self.nomad: 
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
        self.tc.expect('>')

    @timeout()
    def move(self):
        v, h = random.randint(-1, 1), random.randint(-1, 1)
        self.tc.sendline(f'move relative {v} {h} / targets 10 / time')
        res = [self.tc.readline().decode('utf-8')]
        while 'time of day' not in res[-1]: res.append(self.tc.readline().decode('utf-8'))
        self.tc.expect('>')
        return res

    @timeout()
    def list(self):
        self.tc.sendline('list / time / tell all; Nomad is alive')
        res = [self.tc.readline().decode('utf-8')]
        while 'time of day' not in res[-1]: res.append(self.tc.readline().decode('utf-8'))
        self.tc.expect('>')
        return res
    
    def shields(self, *args):
        if 'down' in args: self.tc.sendline('shields down')
        else: self.tc.sendline('shields up')
        self.tc.expect('>')

    def run(self):
        while True:
            try:
                self.tops10 = Tops10(self.kwargs['ip'], self.kwargs['port'], self.kwargs['user'])
                self.tc = self.tops10.tc
                sys = self.tops10.sys()
                self.tc.sendline('r gam:decwar')
                self.tc.expect('Your name please: ')
                self.tc.sendline(f'{self.name}')
                self.tc.expect('line: ')
                self.tc.sendline('')
                index = self.tc.expect(['Regular or Tournament', 'Which side', 'There are'])
                if index == 0:
                    self.tc.sendline(); self.tc.sendline(); self.tc.sendline()
                else:
                    pass
                self.tc.sendline(self.team)
                self.tc.sendline(self.ship)
                self.waitfor('Commands From TTY')
                self.tc.expect('>')
                if self.nomad:
                    self.tc.sendline('*password *mink')
                    self.tc.expect('>')
                self.shields()
                self.gameloop()
            except: break

    def waitfor(self, targstr):
        line = self.tc.readline().decode('utf-8')
        while targstr not in line: line = self.tc.readline().decode('utf-8')
        return line
    