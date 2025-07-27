import random
import time
from definitions import robots

class Brain2:
    
    def __init__(self, name, braincnt, tc):
        self.name = name
        self.tc = tc
        self.braincnt = braincnt
        self.loopcnt = 0
        self.cmdcnt = 0
        
    def nextstep(self):
        time.sleep(3)
        targets = self.command_and_response('targets')
        time.sleep(3)
        status = self.command_and_response('status')
        time.sleep(3)
        self.move()

    def command_and_response(self, cmd):
        """read till eor end of response"""
        self.tc.sendline(cmd)
        if 'move' in cmd: return
        line = self.tc.readline().decode('utf-8').strip()
        while 'move' in line or line == '' or line == '>': line = self.tc.readline().decode('utf-8').strip()
        res = [line]
        while True:
            line = self.tc.readline().decode('utf-8').strip()
            if not line: break
            if ' unit P ' in line or ' unit T ' in line: continue
            res.append(line)
        if 'Message' in res[0]: return
        if ' unit P ' in res[0] or ' unit T ' in res[0]: return
        if 'Navigation Officer' in res[0]: return
        self.cmdcnt += 1
        if 'targets' in res[0]: tmp = ('targets', [x.split() for x in res[1:]])
        elif 'status' in res[0]: tmp = ('status', [x.split() for x in res[1:]])
        print(f'{str(self.braincnt)}|{str(self.loopcnt)}|{str(self.cmdcnt)}|{tmp}')
        return res

    def speak(self):
        if True:
            if self.name not in robots: return
            if random.uniform(0, 1) > .1: return
        msg = random.choice(robots[self.name])
        res = self.command_and_response(f'tell all; {msg}')
    
    def move(self):
        v, h = 0, 0
        while v == 0 and h == 0: v, h = random.randint(-1, 1), random.randint(-1, 1)
        res = self.command_and_response(f'move relative {v} {h}')
    
    def targets(self):
        res = self.command_and_response('targets')

    def time(self):
        res = self.command_and_response('time')
    
    def list(self, *args):
        res = self.command_and_response('list ships')
    