#!/bin/bash

yes j | ./shm_inst 
./init_gal
./def & > /dev/null 2>&1
./rom & > /dev/null 2>&1 
/bin/gotty -w  /app/dwmain
