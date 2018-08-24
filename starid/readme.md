
    ~/starid$ venv/bin/python -m starid -h
    usage: starid [-h] [-w] [-r] [--sky SKY] [--cat CAT]
    
    optional arguments:
      -h, --help  show this help message and exit
      -w, --wsky  write sky binary file
      -r, --rsky  read sky binary file
      --sky SKY   filename for sky binary file
      --cat CAT   filename for skymap text file

compiling libstarid from setuptools and pip isn't automated yet - for now

    mkdir build; cd build
    cmake ../libstarid/eigen-3.3.5
    su make install
    cmake ..
    make
    update PYTHONPATH with the folder containing the *.so

what seems to be working now is

    ~/starid$ python3 setup.py develop
