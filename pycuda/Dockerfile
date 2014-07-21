FROM dibyo/cuda

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

# PyCuda install
RUN wget https://pypi.python.org/packages/source/p/pycuda/pycuda-2013.1.1.tar.gz#md5=acf9319ab2970d9700ed6486aa87b708
RUN tar xzvf pycuda-2013.1.1.tar.gz
RUN cd pycuda-2013.1.1; \
    ./configure.py --cuda-root=/usr/local/cuda-6.0; \
    make install;

# no testing for now
# # run tests
# RUN python /pycuda-2013.1.1/test/test_driver.py
# RUN python /pycuda-2013.1.1/test/test_cumath.py
# RUN python /pycuda-2013.1.1/test/test_gpuarray.py

# sudo docker run -it --privileged=true -v=/dev/dri:/dev/dri:rw -v=/lib/modules/3.8.0-42-generic:/lib/modules/3.8.0-42-generic:rw '--lxc-conf=lxc.cgroup.devices.allow = c 226:* rwm' dibyo/pycuda.12 bin/bash