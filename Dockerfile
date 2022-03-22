FROM ubuntu:focal
LABEL Description="build environment"

ENV HOME /tmp
SHELL ["/bin/bash", "-c"]

RUN apt-get -qq update -qy
RUN apt-get -qq install -y cmake wget

RUN apt-get -qq install -y libeigen3-dev && \
    ln -s /usr/include/eigen3/Eigen /usr/include/Eigen

RUN cd ${HOME} && \
    wget --no-check-certificate --quiet https://github.com/USCiLab/cereal/archive/refs/tags/v1.3.2.tar.gz && \
    tar xzf ./v1.3.2.tar.gz && \
    cp -r cereal-1.3.2/include/cereal /usr/local/include/

RUN cd ${HOME} && \
    wget --no-check-certificate --quiet https://github.com/pybind/pybind11/archive/refs/tags/v2.9.1.tar.gz && \
    tar xzf ./v2.9.1.tar.gz && \
    ln -s ./pybind11-2.9.1 ./pybind11

RUN apt-get -qq install -y python3.8 python3-setuptools python3-venv python3-pip python3-tk

COPY libstarid ${HOME}/libstarid
COPY starid ${HOME}/starid
COPY data ${HOME}/data
COPY CMakeLists.txt requirements.txt setup.py README.md MANIFEST.in LICENSE ${HOME}/

RUN cd ${HOME} && \
    python3 setup.py build_ext && \
    python3 setup.py build_py && \
    python3 setup.py sdist && \
    pip3 install -e .
