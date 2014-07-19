FROM ubuntu:12.04

RUN echo "deb http://mirrors.kernel.org/ubuntu precise main universe" >> /etc/apt/sources.list
RUN apt-get update
RUN apt-get install build-essential wget
RUN apt-get install python-dev python-setuptools
RUN apt-get install python-numpy
RUN apt-get install libboost-python-dev libboost-thread-dev

# cuda
RUN wget http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1204/x86_64/cuda-repo-ubuntu1204_6.0-37_amd64.deb
# RUN sudo sh -c \
#     'echo "foreign-architecture armhf" >> /etc/dpkg/dpkg.cfg.d/multiarch'
# RUN apt-get update

RUN dpkg -i cuda-repo-ubuntu1204_6.0-37_amd64.deb
RUN apt-get update
RUN apt-get install cuda

RUN export PATH=/usr/local/cuda-6.0/bin:$PATH
RUN export LD_LIBRARY_PATH=/usr/local/cuda-6.0/lib64:$LD_LIBRARY_PATH
RUN export CUDA_ROOT=/usr/local/cuda-6.0/

RUN wget https://pypi.python.org/packages/source/p/pycuda/pycuda-2013.1.1.tar.gz#md5=acf9319ab2970d9700ed6486aa87b708
RUN tar xzvf pycuda-2013.1.1.tar.gz
RUN cd pycuda-2013.1.1
RUN ./configure.py --cuda-root=/usr/local/cuda --cudadrv-lib-dir=/usr/lib --boost-inc-dir=/usr/include --boost-lib-dir=/usr/lib --boost-python-libname=boost_python-mt --boost-thread-libname=boost_thread-mt --no-use-shipped-boost
RUN make -j 4

# sudo docker run -i -t -v=/dev/dri:/dev/dri:rw '--lxc-conf=lxc.cgroup.devices.allow = c 226:* rwm' dibyo/pycuda bin/bash
