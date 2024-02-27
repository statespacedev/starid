FROM ubuntu:22.04

RUN DEBIAN_FRONTEND="noninteractive" apt-get update && apt-get -y install tzdata

RUN apt-get update \
    && apt-get install -y build-essential \
    gcc \
    g++ \
    gdb \
    clang \
    make \
    ninja-build \
    cmake \
    autoconf \
    automake \
    libtool \
    valgrind \
    locales-all \
    dos2unix \
    rsync \
    tar \
    python3 \
    python3-pip \
    python3-dev \
    && apt-get clean

RUN pip install pybind11 numpy matplotlib
COPY /data /data

