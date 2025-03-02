#!/bin/bash

rm -rf ./decwar-files
mkdir ./decwar-files

cp ~/decwar/utexas/*.* ./decwar-files
cp ~/decwar/utexas/include/*.* ./decwar-files
cp ~/decwar/compuserve/*.* ./decwar-files

cp ~/decwar/utexas/hlp/DECWAR.HLP ./decwar-files
cp ~/decwar/utexas/hlp/DECWAR.NWS ./decwar-files

cp ~/decwar/scripts/COM1.CMD ./decwar-files
cp ~/decwar/scripts/COM2.CMD ./decwar-files
cp ~/decwar/scripts/CAN1.CMD ./decwar-files

./back10 -cf ./decwar.tap ./decwar-files/*
./back10 -lf ./decwar.tap

