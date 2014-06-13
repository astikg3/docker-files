FROM dibyo/openrave

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

RUN apt-get install -y libopenscenegraph-dev cmake libboost-all-dev libeigen3-dev

# add gurobi
ADD ./src/gurobi563 /usr/local/src/gurobi563
RUN cd /usr/local/src/gurobi563/linux64; \
	python setup.py install
ENV GUROBI_HOME /usr/local/src/gurobi563/linux64

# trajopt
RUN git clone https://github.com/joschu/trajopt.git /usr/local/src/trajopt
RUN cd /usr/local/src/trajopt; \
	mkdir build; \
	cd build; \
	cmake ..; \
	make -j

