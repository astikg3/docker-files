FROM ubuntu:12.04

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

ENV DEBIAN_FRONTEND noninteractive

RUN echo "deb http://mirrors.kernel.org/ubuntu precise main universe" >> /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -y build-essential wget apt-utils
RUN apt-get install -y python-dev python-setuptools
RUN apt-get install -y python-numpy python-mako
RUN apt-get install -y libboost-python-dev libboost-thread-dev
RUN apt-get install -y pciutils

# cuda
RUN wget http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1204/x86_64/cuda-repo-ubuntu1204_6.0-37_amd64.deb
RUN dpkg -i cuda-repo-ubuntu1204_6.0-37_amd64.deb
RUN apt-get update
# RUN apt-get install -y cuda

# environment variables
ENV PATH /usr/local/cuda-6.0/bin:$PATH
ENV LD_LIBRARY_PATH /usr/local/cuda-6.0/lib64:$LD_LIBRARY_PATH
ENV CUDA_ROOT /usr/local/cuda-6.0/

# no PyCuda for now
# # PyCuda install
# RUN wget https://pypi.python.org/packages/source/p/pycuda/pycuda-2013.1.1.tar.gz#md5=acf9319ab2970d9700ed6486aa87b708
# RUN tar xzvf pycuda-2013.1.1.tar.gz
# RUN cd pycuda-2013.1.1; \
#     ./configure.py --cuda-root=/usr/local/cuda-6.0; \
#     make install;

# no testing for now
# # run tests
# RUN python /pycuda-2013.1.1/test/test_driver.py
# RUN python /pycuda-2013.1.1/test/test_cumath.py
# RUN python /pycuda-2013.1.1/test/test_gpuarray.py

# sudo docker run -it --privileged=true -v=/dev/dri:/dev/dri:rw -v=/lib/modules/3.8.0-42-generic:/lib/modules/3.8.0-42-generic:rw '--lxc-conf=lxc.cgroup.devices.allow = c 226:* rwm' dibyo/pycuda.12 bin/bash
