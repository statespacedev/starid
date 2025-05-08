#!/bin/bash

rm -rf ./tmp
mkdir ./tmp

cp ../decwar/step2/*.FOR ./tmp
cp ../decwar/step2/*.MAC ./tmp

cp ../decwar/step2/hlp/DECWAR.HLP ./tmp
cp ../decwar/step2/hlp/DECWAR.NWS ./tmp

./back10 -cf ./tapes/decwar2.tap ./tmp/*
./back10 -lf ./tapes/decwar2.tap

