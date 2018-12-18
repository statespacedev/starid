
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
      
181218

preparing for gitlab automated test install using a basic ubuntu container. may want something like
    
    sudo apt install libeigen3-dev
    
rather than os-level install from internal as below. apt install should be simple in the ubuntu container. then setup.py. container script's looking something like

    - apt-get -qq update -qy
    - apt-get -qq install -y python3.6 python3-venv python3-pip
    - apt-get -qq install -y libeigen3-dev
    - python3 -m venv venv
    - . venv/bin/activate
    - python3 setup.py build_ext
    - python3 setup.py build_py
    - python3 setup.py sdist

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

