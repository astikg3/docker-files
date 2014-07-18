FROM ubuntu:12.04

RUN apt-get update
RUN apt-get install build-essential wget
RUN apt-get install python-dev python-setuptools
RUN apt-get install python-numpy
RUN apt-get install libboost-python-dev libboost-thread-dev
RUN wget http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1204/x86_64/cuda-repo-ubuntu1204_6.0-37_amd64.deb

# cuda
RUN sudo sh -c \
    'echo "foreign-architecture armhf" >> /etc/dpkg/dpkg.cfg.d/multiarch'
RUN apt-get update

RUN dpkg -i cuda-repo-ubuntu1204_6.0-37_amd64.deb
RUN apt-get update
RUN apt-get install cuda

RUN export PATH=/usr/local/cuda-6.0/bin:$PATH
RUN export LD_LIBRARY_PATH=/usr/local/cuda-6.0/lib64:$LD_LIBRARY_PATH
RUN export CUDA_ROOT=/usr/local/cuda-6.0/