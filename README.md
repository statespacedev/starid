<img src="https://gitlab.com/noahhsmith/starid/raw/master/docs/images/nouns%20and%20verbs%20level0.png" align="center" width="426" height="202"/>

[![pipeline](https://gitlab.com/noahhsmith/starid/badges/master/pipeline.svg)](https://gitlab.com/noahhsmith/starid/pipelines)
[![pypi](https://img.shields.io/badge/pypi-latest-brightgreen.svg)](https://pypi.org/project/starid/)

[data](https://gitlab.com/noahhsmith/starid/tree/master/data), [blog](https://gitlab.com/noahhsmith/starid/blob/master/docs/readme.md), [about](https://gitlab.com/noahhsmith/starid/blob/master/docs/about.md), [references](https://gitlab.com/noahhsmith/starid/blob/master/docs/references.md)

[sky](https://gitlab.com/noahhsmith/starid/blob/master/libstarid/sky.h) generates three-dimensional sky models and two-dimensional images from the nasa skymap star catalog. finds stars near arbitrary points on the sky.

[star triangles](https://gitlab.com/noahhsmith/starid/blob/master/libstarid/startriangles.h) recurrently sets aside stars that disagree geometrically until one remains.

[star languages](https://gitlab.com/noahhsmith/starid/blob/master/starid/starlanguages.py) writes sentences about star patterns using geometry-languages and translates into identifier-languages.

automated build-test-deploy to [pypi](https://pypi.org/project/starid) is mostly a placeholder, ubuntu clone-install-develop of the [repo](https://gitlab.com/noahhsmith/starid) is assumed for now.

    sudo apt-get -qq update -qy
    sudo apt-get -qq install -y python3.6 python3-venv python3-pip
    sudo apt-get -qq install -y cmake
    sudo apt-get -qq install -y libeigen3-dev
    git clone git@gitlab.com:noahhsmith/starid.git starid
    cd starid
    python3 -m venv venv
    . venv/bin/activate
    python3 setup.py develop
    pytest
    python3 starid --demo
