import time
import pexpect
from definitions import ships
from brain import Brain

class Robot2:

    def __init__(self, *args, **kwargs):
        self.args, self.kwargs = args, kwargs
        self.name = kwargs['name']
        self.mode = '1'
        self.braincnt = 0
        self.brain = None

    def main(self):
        try:
            self.brain.loopcnt += 1
            while self.mode: # main loop
                if self.mode == '1': time.sleep(1)
                elif self.mode == '2': self.brain.nextstep()
        except:
            for _ in range(5):
                time.sleep(1)
                try:
                    self.tc.sendline(''); self.tc.expect('>', timeout=1)
                    self.main()  # reenter loop
                except: pass
        self.exit()

    def set_mode(self, key):
        if key == '0': self.mode = 0; return # set to false, causes main loop to exit
        if self.mode == '1' and key == '2': self.entry()
        elif self.mode == '2' and key == '1': self.exit()
        self.mode = key

    def entry(self):
        self.telnet_entry()
        self.tops10_entry()
        self.decwar_entry()
        self.braincnt += 1
        self.brain = Brain(self.name, self.braincnt, self.tc)

    def exit(self):
        self.brain = None
        self.decwar_exit()
        self.tops10_exit()
        self.telnet_exit()

    def telnet_entry(self):
        try:
            self.tc = pexpect.spawn(f"telnet {self.kwargs['ip']} {self.kwargs['port']}", timeout=10, echo=False)
            time.sleep(1)
            self.tc.expect('.', timeout=10)
            self.tc.sendline('')
            time.sleep(1)
            self.tc.expect('.', timeout=10)
        except: pass

    def tops10_entry(self):
        try:
            self.tc.sendline('')
            time.sleep(1)
            self.tc.expect('.', timeout=10)
            self.tc.sendline(f"login {self.kwargs['ppn']}")
            time.sleep(1)
            self.tc.expect('.', timeout=10)
            self.tc.sendline('')
            time.sleep(1)
            self.tc.expect('.', timeout=10)
        except: pass
        
    def decwar_entry(self):
        """from tops10, run game and get to command prompt"""
        try:
            self.tc.sendline('r gam:decwar')
            self.tc.expect('Your name please: ', timeout=10)
            self.tc.sendline(f'{self.name}')
            self.tc.expect('line: ', timeout=10)
            self.tc.sendline('')
            ndx1 = self.tc.expect(['DECWAR', 'Regular or Tournament', 'Federation or Empire', 'You will join', 'choose another ship?'], timeout=10)
            if ndx1 > 0:
                if ndx1 == 1: self.tc.sendline(); self.tc.sendline(); self.tc.sendline()
                if ndx1 < 3: self.tc.sendline()  # join default side
                if ndx1 == 4: self.tc.sendline('yes')
                for ship in ships:
                    ndx2 = self.tc.expect(['DECWAR', 'Which vessel'], timeout=10)
                    if ndx2 == 0: break
                    else: self.tc.sendline(ship)
            self.tc.expect('Commands From TTY', timeout=10)
            self.tc.sendline('')
            self.tc.expect('>', timeout=10)
        except: pass

    def decwar_exit(self):
        try:
            self.tc.sendline()
            self.tc.expect('>', timeout=10)
            self.tc.sendline('quit')
            self.tc.expect('Do you really want to quit?', timeout=10)
            self.tc.sendline('yes')
        except: pass

    def tops10_exit(self):
        try:
            self.tc.sendline('')
            time.sleep(1)
            self.tc.expect('.', timeout=10)
            self.tc.sendline('kjob')
            time.sleep(1)
            self.tc.expect('.', timeout=10)
        except: pass

    def telnet_exit(self):
        try:
            self.tc.sendline('')
            time.sleep(1)
            self.tc.expect('.', timeout=10)
            self.tc.sendcontrol(']')
            time.sleep(1)
            self.tc.sendline('close')
            time.sleep(1)
            self.tc.terminate(force=True)
        except: pass
            