FROM ubuntu:12.04

RUN apt-get update
RUN apt-get install python-dev
RUN apt-get install python-numpy
RUN apt-get install libboost-dev
RUN apt-get install wget
RUN wget http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1204/x86_64/cuda-repo-ubuntu1204_6.0-37_amd64.deb



# cuda
RUN sudo sh -c \
    'echo "foreign-architecture armhf" >> /etc/dpkg/dpkg.cfg.d/multiarch'
RUN apt-get update

RUN dpkg -i cuda-repo-ubuntu1204_6.0-37_amd64.deb
RUN apt-get update
RUN apt-get install cuda
RUN apt-get install cuda-drivers