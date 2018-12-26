<img src="https://gitlab.com/noahhsmith/starid/raw/master/docs/images/nouns%20and%20verbs%20level0.png" align="center" width="426" height="202"/>

[notes](https://gitlab.com/noahhsmith/starid/blob/master/docs/readme.md), [data](https://gitlab.com/noahhsmith/starid/blob/master/data/readme.md), [references](https://gitlab.com/noahhsmith/starid/blob/master/docs/references.md), [about](https://gitlab.com/noahhsmith/starid/blob/master/docs/about.md)

[sky](https://gitlab.com/noahhsmith/starid/blob/master/libstarid/sky.h) generates three-dimensional sky models and two-dimensional images from the nasa skymap star catalog. finds stars near arbitrary points on the sky.

[star triangles](https://gitlab.com/noahhsmith/starid/blob/master/libstarid/startriangles.h) recurrently sets aside stars that disagree geometrically until one remains.

[star languages](https://gitlab.com/noahhsmith/starid/blob/master/starid/starlanguages.py) writes sentences about star patterns using geometry-languages and translates into identifier-languages.

in ubuntu, install or upgrade os-level dependencies

    sudo apt-get -qq update -qy
    sudo apt-get -qq install -y python3.6 python3-venv python3-pip
    sudo apt-get -qq install -y cmake
    sudo apt-get -qq install -y libeigen3-dev

clone the git project, start a virtual environment, install and test

    git clone git@gitlab.com:noahhsmith/starid.git starid
    cd starid
    python3 -m venv venv
    . venv/bin/activate
    pip install -e .
    pytest
    
~/starid$ python starid -h

    usage: starid [-h] [--dirsky DIRSKY] [-d]
    
    optional arguments:
      -h, --help       show this help message and exit
      --dirsky DIRSKY  path to skymap directory
      -d, --demo       demo star image


