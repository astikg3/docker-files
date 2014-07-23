FROM ubuntu:12.04

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

ENV DEBIAN_FRONTEND noninteractive

RUN echo "deb http://mirrors.kernel.org/ubuntu precise main universe" >> /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -y build-essential wget
RUN apt-get install -y python-dev python-setuptools
RUN apt-get install -y python-numpy python-mako
RUN apt-get install libboost-python-dev libboost-thread-dev
RUN apt-get install pciutils

# cuda
RUN wget http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1204/x86_64/cuda-repo-ubuntu1204_6.0-37_amd64.deb
RUN dpkg -i cuda-repo-ubuntu1204_6.0-37_amd64.deb
RUN apt-get update

# environment variables
ENV PATH /usr/local/cuda-6.0/bin:$PATH
ENV LD_LIBRARY_PATH /usr/local/cuda-6.0/lib64:$LD_LIBRARY_PATH
ENV CUDA_ROOT /usr/local/cuda-6.0/

# Now execute the following commands inside the docker container
# apt-get install -y cuda