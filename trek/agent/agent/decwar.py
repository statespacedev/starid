import time
import random
from tops10 import Tops10


class Decwar:

    def __init__(self, *args, **kwargs):
        self.args, self.kwargs = args, kwargs
        self.name = kwargs['name']
        self.team = kwargs['team']
        self.ship = kwargs['ship']
        self.nomad = True if kwargs['name'] == 'nomad' else False
        if 'nomad' in args:
            self.user = 'decwar'
            self.name = 'nomad'
            self.team = 'fed'
            self.ship = 'y'
            self.nomad = True
        self.tops10 = Tops10(kwargs['ip'], kwargs['port'], kwargs['user'])
        self.tc = self.tops10.tc

    def gameloop(self):
        while True:
            time.sleep(10)
            if self.nomad: self.super_list()
            self.move()

    def move(self):
        v, h = random.randint(-1, 1), random.randint(-1, 1)
        self.tc.sendline(f'move relative {v} {h}')
        self.tc.expect('>')

    def shields(self):
        self.tc.sendline('shields up')
        self.tc.expect('>')

    def run(self):
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

    def super_list(self):
        self.tc.sendline('list / time / tell all; nomad is alive')
        res = [self.tc.readline().decode('utf-8')]
        while 'time of day' not in res[-1]: res.append(self.tc.readline().decode('utf-8'))
        return res

    def waitfor(self, targstr):
        line = self.tc.readline().decode('utf-8')
        while targstr not in line: line = self.tc.readline().decode('utf-8')
        return line
    