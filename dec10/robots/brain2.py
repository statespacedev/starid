import random
import time
from definitions import robots, Mode
from concepts.enemybases import EnemyBases
from concepts.enemyships import EnemyShips

class Brain2:
    superbot = 'nomad'

    def __init__(self, name, braincnt, tc):
        self.name = name
        self.tc = tc
        self.braincnt = braincnt
        self.shipcnt = 1
        self.last_new_ship_cmdcnt = 0
        self.cmdcnt = 0
        self.age = 0
        self.mode = Mode.defense
        self.modespan = 100 + random.randint(1, 100)
        self.enemybases = EnemyBases()
        self.enemyships = EnemyShips()

    def next(self):
        try:
            self.age = self.cmdcnt - self.last_new_ship_cmdcnt
            if self.age > self.modespan: self.mode = Mode.offense
            self.speak()
            if self.name == self.superbot:
                res = self.command_and_response('time')
                res = self.command_and_response('po a')
                res = self.command_and_response('list ships')
            res = self.command_and_response('targets')
            if self.mode == Mode.offense:
                res = self.command_and_response('bases enemy')
                targ = self.enemybases.update(res)
                if not targ[2]: self.explore()
                else:
                    if targ[2] > 6: self.approach(targ)
                    else: self.attack(targ)
            elif self.mode == Mode.defense:
                res = self.command_and_response('list ships enemy')
                targ = self.enemyships.update(res)
                if not targ[2]: self.explore()
                else:
                    if targ[2] > 6: self.approach(targ)
                    else: self.attack(targ)
        except:
            try:
                for _ in range(2): self.tc.sendline(); self.tc.expect('>', timeout=10) # try to stay in game
            except: raise

    def speak(self):
        if True:
            if self.name not in robots: return
            if random.uniform(0, 1) > .1: return
        msg = random.choice(robots[self.name])
        res = self.command_and_response(f'tell all; {msg}')

    def attack(self, targ):
        res = self.command_and_response(f'ph r {targ[0]} {targ[1]}')
        if random.uniform(0, 1) > .5: res = self.command_and_response(f'to r 1 {targ[0]} {targ[1]}')

    def approach(self, targ):
        dv = 0 if targ[0] == 0 else int(3 * targ[0] / abs(targ[0]))
        dh = 0 if targ[1] == 0 else int(3 * targ[1] / abs(targ[1]))
        tmp = random.uniform(0, 1)
        if tmp < .05: dv = 0; dh *= -1 # in case stuck
        elif tmp > .95: dh = 0; dv *= -1
        res = self.command_and_response(f'm r {dv} {dh}')
        res = self.command_and_response(f'sc')

    def explore(self):
        dv, dh = random.randint(-3, 3), random.randint(-3, 3)
        res = self.command_and_response(f'm r {dv} {dh}')
        res = self.command_and_response(f'sc')

    def listen(self, secs):
        res, tmp = [], ''
        tf = time.time() + secs
        while time.time() < tf:
            try: tmp += self.tc.read_nonblocking(3000, timeout=.2).decode('utf-8')
            except: pass
        tmp = tmp.replace('\x07', '')  # \x07 is \xhh hex for \a escape seq for ascii bell
        tmp = tmp.strip().lower().splitlines()
        for line in tmp:
            newshipsignals = ['set prompt', 'set ocdef', 'set output', 'reading commands', 'runs out']
            if self.age > 100 and any(x in line for x in newshipsignals): 
                self.shipcnt += 1
                self.last_new_ship_cmdcnt = self.cmdcnt
            res.append(line)
        return res

    def command_and_response(self, command):
        """send a command and get the response"""
        try:
            for _ in range(2): self.tc.sendline(); self.tc.expect('>', timeout=10)
            self.tc.sendline(command)
            res = self.listen(4)
            self.cmdcnt += 1
            for rec in res:
                if not rec: continue
                print(f'{self.shipcnt}|{self.cmdcnt - self.last_new_ship_cmdcnt}|{rec}')
            return res
        except: print(f"cmd exception '{command}'")
