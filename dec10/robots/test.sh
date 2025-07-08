
python -y robots/run nomad > log1 &
sleep 10
python -y robots/run robot2 > log2 &
sleep 10
python -y robots/run robot3 > log3 &
sleep 10
python -y robots/run robot4 > log4 &
sleep 10
python -y robots/run robot5 > log5 &
sleep 10
python -y robots/run robot6 > log6 &

