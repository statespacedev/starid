#!/bin/bash

python runner.py --nomad &

python runner.py --user demo --name batty &

python runner.py --user demo --name player2 &

python runner.py --user demo --name player3 &
