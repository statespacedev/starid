#!/bin/bash

rm -rf ./decwar-files
mkdir ./decwar-files

cp ../trek/utexas/*.* ./decwar-files
cp ../trek/utexas/include/*.* ./decwar-files
cp ../trek/compuserve/*.* ./decwar-files

cp ../trek/utexas/hlp/DECWAR.HLP ./decwar-files
cp ../trek/utexas/hlp/DECWAR.NWS ./decwar-files

cp ../trek/scripts/COM1.CMD ./decwar-files
cp ../trek/scripts/COM2.CMD ./decwar-files
cp ../trek/scripts/CAN1.CMD ./decwar-files

./back10 -cf ./tapes/decwar.tap ./decwar-files/*
./back10 -lf ./tapes/decwar.tap

