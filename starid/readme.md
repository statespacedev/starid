some basic functionality for testing the package

    ~/starid$ starid -h
    usage: starid [-h] [-t] [--cat CAT] [--sky SKY] [--starpairs PAIRS] [--rsky]
                  [--wsky] [--rstarpairs] [--wstarpairs]
    
    optional arguments:
      -h, --help         show this help message and exit
      -t, --test         show test image
      --cat CAT          filename for skymap text file
      --sky SKY          filename for sky binary file
      --starpairs PAIRS  filename for starpairs binary file
      --rsky             read sky binary file
      --wsky             write sky binary file
      --rstarpairs       read starpairs binary file
      --wstarpairs       write starpairs binary file

180827

pip install of the package with cmake build of libstarid is working now. haven't tested if it's properly handling eigen on a clean system - so for now will leave the os-level install

    mkdir build; cd build
    cmake ~/starid/libstarid/eigen-3.3.5
    su make install
    
    ~/starid$ python3 setup.py develop

180813

building libstarid from setuptools and pip isn't automated yet - for now

    mkdir build; cd build
    cmake ~/starid/libstarid/eigen-3.3.5
    su make install
    
    cmake ~/starid
    make
    
update PYTHONPATH with the folder containing the *.so - this is line 3 of ~/starid/starid/__main__.py 

for nmt see [here](https://github.com/tensorflow/nmt)