#!/bin/bash

rm -rf ./tmp
mkdir ./tmp

cp ../trek/utexas/*.* ./tmp
cp ../trek/utexas/include/*.* ./tmp
cp ../trek/compuserve/*.* ./tmp

cp ../trek/utexas/hlp/DECWAR.HLP ./tmp
cp ../trek/utexas/hlp/DECWAR.NWS ./tmp

cp ../trek/scripts/COM1.CMD ./tmp
cp ../trek/scripts/COM2.CMD ./tmp
cp ../trek/scripts/CAN1.CMD ./tmp

./back10 -cf ./tapes/decwar.tap ./tmp/*
./back10 -lf ./tapes/decwar.tap

