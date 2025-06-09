#!/bin/bash

python runner.py --nomad &

python runner.py --user demo --name batty --team e --ship h &

python runner.py --user demo --name sulu --team f --ship n &
