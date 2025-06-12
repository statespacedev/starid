#!/bin/bash

python decwar.py --nomad &

python decwar.py --ppn demo --name batty &

python decwar.py --ppn demo --name player2 &

python decwar.py --ppn demo --name player3 &
