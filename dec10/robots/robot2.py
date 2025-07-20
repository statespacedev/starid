import time
import pexpect
from definitions import ships
from brain import Brain

class Robot2:

    def __init__(self, *args, **kwargs):
        self.args, self.kwargs = args, kwargs
        self.name = kwargs['name']
        self.mode = '1'

    def main(self):
        self.loop()
        self.exit()
        
    def loop(self):
        while self.mode: # main loop
            if self.mode == '1':
                pass
            elif self.mode == '2':
                pass
            time.sleep(1)

    def set_mode(self, key):
        if key == '0':
            self.mode = 0 # set to false, which causes main loop to exit
            return
        if self.mode == '1' and key == '2':
            self.entry()
        elif self.mode == '2' and key == '1':
            self.exit()
        self.mode = key

    def entry(self):
        self.telnet_entry()
        self.tops10_entry()
        self.decwar_entry()

    def exit(self):
        self.decwar_exit()
        self.tops10_exit()
        self.telnet_exit()

    def telnet_entry(self):
        try:
            # self.tc = pexpect.spawn(f"telnet {self.kwargs['ip']} {self.kwargs['port']}", timeout=2, logfile=sys.stdout.buffer, echo=False)
            self.tc = pexpect.spawn(f"telnet {self.kwargs['ip']} {self.kwargs['port']}", timeout=2, echo=False)
            time.sleep(1)
            self.tc.expect('.', timeout=2)
            self.tc.sendline('')
            time.sleep(1)
            self.tc.expect('.', timeout=2)
            print('telnet entry ok')
        except:
            print('telnet entry nok')

    def tops10_entry(self):
        try:
            self.tc.sendline('')
            time.sleep(1)
            self.tc.expect('.', timeout=2)
            self.tc.sendline(f"login {self.kwargs['ppn']}")
            time.sleep(1)
            self.tc.expect('.', timeout=2)
            self.tc.sendline('')
            time.sleep(1)
            self.tc.expect('.', timeout=2)
            print('tops10 entry ok')
        except:
            print('tops10 entry nok')

    def decwar_entry(self):
        """from tops10, run game and get to command prompt"""
        try:
            self.tc.sendline('r gam:decwar')
            self.tc.expect('Your name please: ', timeout=2)
            self.tc.sendline(f'{self.name}')
            self.tc.expect('line: ', timeout=2)
            self.tc.sendline('')
            ndx1 = self.tc.expect(
                ['DECWAR', 'Regular or Tournament', 'Federation or Empire', 'You will join', 'choose another ship?'],
                timeout=2)
            if ndx1 > 0:
                if ndx1 == 1: self.tc.sendline(); self.tc.sendline(); self.tc.sendline()
                if ndx1 < 3: self.tc.sendline()  # join default side
                if ndx1 == 4: self.tc.sendline('yes')
                for ship in ships:
                    ndx2 = self.tc.expect(['DECWAR', 'Which vessel'], timeout=2)
                    if ndx2 == 0:
                        break
                    else:
                        self.tc.sendline(ship)
            self.tc.expect('Commands From TTY', timeout=2)
            self.tc.sendline('')
            self.tc.expect('>', timeout=2)
            print('decwar entry ok')
        except:
            print('decwar entry nok')

    def decwar_exit(self):
        try:
            self.tc.sendline()
            self.tc.expect('>', timeout=2)
            self.tc.sendline('quit')
            self.tc.expect('Do you really want to quit?', timeout=2)
            self.tc.sendline('yes')
            print('decwar exit ok')
        except:
            print('decwar exit nok')

    def tops10_exit(self):
        try:
            self.tc.sendline('')
            time.sleep(1)
            self.tc.expect('.', timeout=2)
            self.tc.sendline('kjob')
            time.sleep(1)
            self.tc.expect('.', timeout=2)
            print('tops10 exit ok')
        except:
            print('tops10 exit nok')

    def telnet_exit(self):
        try:
            self.tc.sendline('')
            time.sleep(1)
            self.tc.expect('.', timeout=2)
            self.tc.sendcontrol(']')
            time.sleep(1)
            self.tc.sendline('close')
            time.sleep(1)
            self.tc.terminate(force=True)
            print('telnet exit ok')
        except:
            print('telnet exit nok')
            