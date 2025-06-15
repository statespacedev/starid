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
        
    def game(self):
        """main entrypoint. the game 'session'. when this dies, quit game and kjob on tops10. from painful 
        experience, this appears essential, for avoiding game state corruption. this is done by __del__"""
        try:
            self.connect()
            self.start()
            for _ in range(3): self.cmdloop()
        except: 
            print('except out of game')
            return  # triggers __del__()
        
    def cmdloop(self):
        try:
            if self.nomad:
                self.tc.sendline('*password *mink')
                self.tc.expect('>', timeout=10)
            while True:
                self.speak_randomly()
                if self.nomad: res = self.list('ships')
                else: res = self.move()
                time.sleep(10)
        except:
            for _ in range(3):
                try:
                    time.sleep(1)
                    self.tc.sendline('')
                    self.tc.expect('>', timeout=10)
                    time.sleep(1)
                    self.tc.sendline('')
                    self.tc.expect('>', timeout=10)
                    time.sleep(1)
                    self.tc.sendline('')
                    self.tc.expect('>', timeout=10)
                    self.cmdloop()
                except: pass
            print('except out of cmdloop')
            raise
            
    def start(self):
        """from tops10, run game and get to command prompt"""
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
            for ship in ships:
                ndx2 = self.tc.expect(['DECWAR', 'Which vessel'], timeout=10)
                if ndx2 == 0: break
                else: self.tc.sendline(ship)
        self.tc.expect('Commands From TTY', timeout=10)
        self.tc.sendline('')
        self.tc.expect('>', timeout=10)
        
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

    def list(self, *args):
        if 'ships' in args: self.tc.sendline('list ships / time')
        else: return
        res = [self.tc.readline().decode('utf-8')]
        while 'time of day' not in res[-1]: res.append(self.tc.readline().decode('utf-8'))
        self.tc.expect('>', timeout=10)
        return res
    
    def shields(self, *args):
        if 'down' in args: self.tc.sendline('shields down')
        else: self.tc.sendline('shields up')
        self.tc.expect('>', timeout=10)
    
    def connect(self):
        self.tc = pexpect.spawn(f"telnet {self.kwargs['ip']} {self.kwargs['port']}", timeout=10, logfile=sys.stdout.buffer, echo=False)
        self.tc.expect('\r\n\n', timeout=10)
        self.tc.readline()
        connection_msg = self.tc.readline().decode('utf-8')
        self.tops10_login()
        # self.tops10_sys()
        
    def tops10_login(self):
        """do the tops10 login command"""
        self.tc.send(f"login {self.kwargs['ppn']}\r\n")
        index = self.tc.expect(['\n\r\n', 'Unknown command', 'Non-numeric coordinate', 'login: '], timeout=10)
        if index > 0: 
            self.tc.sendcontrol('c')
            ndx2 = self.tc.expect(['Do you really want', 'Use QUIT'], timeout=10)
            if ndx2 == 0: 
                self.tc.sendline('yes')
            else:
                self.tc.sendline('quit')
                self.tc.sendline('yes')
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

    def __del__(self):
        try: 
            self.tc.sendcontrol('c')
            time.sleep(1)
            self.tc.sendcontrol('c')
            time.sleep(1)
            ndx2 = self.tc.expect(['Do you really want', 'Use QUIT', '.'], timeout=10)
            time.sleep(1)
            if ndx2 == 0: 
                self.tc.sendline('yes')
            elif ndx2 == 1:
                self.tc.sendline('quit')
                time.sleep(1)
                self.tc.sendline('yes')
            else: pass
            time.sleep(1)
        except: print('nogo quit game')
        try:
            self.tc.expect('.', timeout=10)
            time.sleep(1)
            self.tc.sendline('kjob')
            time.sleep(1)
        except: print('nogo kjob')
        try: 
            self.tc.expect('.', timeout=10)
            time.sleep(1)
            self.tc.sendcontrol(']')
            time.sleep(1)
            self.tc.sendline('close')
            time.sleep(1)
            self.tc.terminate(force=True)
        except: print('nogo close connections')

if __name__ == "__main__":
    args, kwargs = cli.main()
    # while True:
    #     dw = Decwar(*args, **kwargs).game()
    #     time.sleep(random.randint(5, 10))
    dw = Decwar(*args, **kwargs).game()
