""" 'pkill -2 -f python' for ctrl c """
from sshkeyboard import listen_keyboard, stop_listening
import signal
import threading
import cli
from robot2 import Robot2
args, kwargs = cli.main()
ro = Robot2(*args, **kwargs)
rothread = threading.Thread(target=ro.main)
rothread.start()
    
def main():
    signal.signal(signal.SIGINT, signal_handler)
    try:
        ro.set_mode('2')
        listen_keyboard(on_press=press)
    except: pass

def press(key):
    print(f"'{key}' key pressed")
    ro.set_mode(key)
    if key == '0': stop_listening()

def signal_handler(sig, frame):
    print('ctrl c pressed and translated as 0 key')
    press('0')
    
if __name__ == "__main__":
    main()
