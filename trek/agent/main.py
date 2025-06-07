from agent.tops10 import Tops10
from agent.decwar_monitor import DecwarMonitor

dm = DecwarMonitor(ip='192.168.178.21', port=2030, ppn='decwar')
sys = dm.tops10.sys()
dm.enter_game()
sl = dm.super_list()
for line in sl: print(sl)
