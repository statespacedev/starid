<img src="https://gitlab.com/noahhsmith/starid/raw/master/docs/images/nouns%20and%20verbs%20level0.png" align="center" width="426" height="202"/>

[references](https://gitlab.com/noahhsmith/starid/blob/master/docs/references.md), [about](https://gitlab.com/noahhsmith/starid/blob/master/docs/about.md)

[sky](https://gitlab.com/noahhsmith/starid/blob/master/libstarid/sky.h) generates a three-dimensional sky model and two-dimensional images from the nasa skymap star catalog, finds stars near arbitrary points on the sky

[star triangles](https://gitlab.com/noahhsmith/starid/blob/master/libstarid/startriangles.h) recurrently sets aside stars that disagree geometrically until one remains

[star patterns](https://gitlab.com/noahhsmith/starid/blob/master/starid/starpatterns.py) are relatively sensitive to rotation

[star sequences](https://gitlab.com/noahhsmith/starid/blob/master/starid/starsequences.py) can be less sensitive to rotation

[star languages](https://gitlab.com/noahhsmith/starid/blob/master/starid/starlanguages.py) writes sentences about star patterns using a geometrical language and translates into an identifier language

in ubuntu, install or upgrade os-level dependencies

    sudo apt-get -qq update -qy
    sudo apt-get -qq install -y python3.6 python3-venv python3-pip
    sudo apt-get -qq install -y cmake
    sudo apt-get -qq install -y libeigen3-dev

clone the git project, start a venv virtual environment, install the package, and test

    git clone git@gitlab.com:noahhsmith/starid.git starid
    cd starid
    python3 -m venv venv
    . venv/bin/activate (same as source venv/bin/activate)
    python3 setup.py install
    python3 starid --wsky (preps the nasa skymap catalog)
    python3 starid -t (generates and plots a star image using the nasa skymap catalog)
    
usage hints    

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
