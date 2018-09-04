some basic functionality to test the package

    ~/starid$ venv/bin/python -m starid -h
    usage: starid [-h] [-w] [-r] [--sky SKY] [--cat CAT]
    
    optional arguments:
      -h, --help  show this help message and exit
      -w, --wsky  write sky binary file
      -r, --rsky  read sky binary file
      --sky SKY   filename for sky binary file
      --cat CAT   filename for skymap text file

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