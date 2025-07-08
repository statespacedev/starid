# bash robots/test.sh &
# use tail -f to watch one of the log files
# pkill -f python to end them

#./msc/create-tape-utexas23-reconstruction
#./dec10blinken simh/boot-from-disk.ini > log0 &
#sleep 30

telnet localhost 2030 > loga &
telnet localhost 2030 > logb &
telnet localhost 2030 > logc &

rm log*
bash python -u robots/run nomad > log1 &
sleep 5
bash python -u robots/run robot2 > log2 &
sleep 10
bash python -u robots/run robot3 > log3 &
sleep 10
bash python -u robots/run robot4 > log4 &
sleep 10
bash python -u robots/run robot5 > log5 &
sleep 10
bash python -u robots/run robot6 > log6 &

