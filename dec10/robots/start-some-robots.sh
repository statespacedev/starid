# bash robots/start-some-robots.sh &
# tail -f log1
# pkill -f python

rm log*
#./msc/create-tape-utexas23-reconstruction
#./dec10blinken simh/boot-from-disk.ini > log0 &
#sleep 30

python -u robots/noninteractive.py nomad > log1 &
sleep 2
python -u robots/noninteractive.py robot2 > log2 &
sleep 5
python -u robots/noninteractive.py robot3 > log3 &
sleep 5
python -u robots/noninteractive.py robot4 > log4 &
sleep 5
python -u robots/noninteractive.py robot5 > log5 &
sleep 5
python -u robots/noninteractive.py robot6 > log6 &
sleep 5
python -u robots/noninteractive.py robot7 > log7 &
sleep 5
python -u robots/noninteractive.py robot8 > log8 &
sleep 5
python -u robots/noninteractive.py robot9 > log9 &
sleep 5
python -u robots/noninteractive.py robot10 > log10 &
sleep 5
#python -u robots/noninteractive.py robot11 > log11 &
#sleep 5
#python -u robots/noninteractive.py robot12 > log12 &
