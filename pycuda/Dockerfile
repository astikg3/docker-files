FROM ubuntu:12.04

RUN echo "deb http://mirrors.kernel.org/ubuntu precise main universe" >> /etc/apt/sources.list
RUN apt-get update
RUN apt-get install build-essential wget
RUN apt-get install python-dev python-setuptools
RUN apt-get install python-numpy
RUN apt-get install libboost-python-dev libboost-thread-dev
RUN apt-get install pciutils

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


# echo "deb http://mirrors.kernel.org/ubuntu precise main universe" >> /etc/apt/sources.list
# apt-get update
# apt-get install build-essential wget
# apt-get install python-dev python-setuptools
# apt-get install python-numpy
# apt-get install libboost-python-dev libboost-thread-dev

# # cuda
# wget http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1204/x86_64/cuda-repo-ubuntu1204_6.0-37_amd64.deb

# dpkg -i cuda-repo-ubuntu1204_6.0-37_amd64.deb
# apt-get update
# apt-get install cuda
# export PATH=/usr/local/cuda-6.0/bin:$PATH
# export LD_LIBRARY_PATH=/usr/local/cuda-6.0/lib64:$LD_LIBRARY_PATH
# export CUDA_ROOT=/usr/local/cuda-6.0/
# wget https://pypi.python.org/packages/source/p/pycuda/pycuda-2013.1.1.tar.gz#md5=acf9319ab2970d9700ed6486aa87b708
# tar xzvf pycuda-2013.1.1.tar.gz
# cd pycuda-2013.1.1
# ./configure.py --cuda-root=/usr/local/cuda --cudadrv-lib-dir=/usr/lib --boost-inc-dir=/usr/include --boost-lib-dir=/usr/lib --boost-python-libname=boost_python-mt --boost-thread-libname=boost_thread-mt --no-use-shipped-boost
# make -j 4


# 01:00.0 VGA compatible controller: NVIDIA Corporation GF116 [GeForce GTX 550 Ti] (rev a1) (prog-if 00 [VGA controller])
#     Subsystem: eVga.com. Corp. Device 1556
#     Flags: bus master, fast devsel, latency 0, IRQ 75
#     Memory at f8000000 (32-bit, non-prefetchable) [size=32M]
#     Memory at e8000000 (64-bit, prefetchable) [size=128M]
#     Memory at f0000000 (64-bit, prefetchable) [size=64M]
#     I/O ports at e000 [size=128]
#     [virtual] Expansion ROM at fa000000 [disabled] [size=512K]
#     Capabilities: [60] Power Management version 3
#     Capabilities: [68] MSI: Enable+ Count=1/1 Maskable- 64bit+
#     Capabilities: [78] Express Endpoint, MSI 00
#     Capabilities: [b4] Vendor Specific Information: Len=14 <?>
#     Capabilities: [100] Virtual Channel
#     Capabilities: [128] Power Budgeting <?>
#     Capabilities: [600] Vendor Specific Information: ID=0001 Rev=1 Len=024 <?>
#     Kernel driver in use: nvidia
#     Kernel modules: nvidia_331, nouveau, nvidiafb

