FROM dibyo/openrave

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

RUN apt-get install -y libopenscenegraph-dev cmake libboost-all-dev libeigen3-dev

# add gurobi
ADD ./src/gurobi563 /usr/local/src/gurobi563
RUN cd /usr/local/src/gurobi563/linux64; \
	python setup.py install
ENV GUROBI_HOME /usr/local/src/gurobi563/linux64
ENV PATH ${PATH}:${GUROBI_HOME}/bin 
ENV LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${GUROBI_HOME}/lib 
RUN	grbgetkey c8e62080-0fc1-7665-e7ca-539b91f53fcf

# trajopt
RUN git clone https://github.com/joschu/trajopt.git /usr/local/src/trajopt
RUN cd /usr/local/src/trajopt; \
	mkdir build; \
	cd build; \
	cmake ..; \
	make -j
ENV PYTHONPATH $PYTHONPATH:/usr/local/src/trajopt:/usr/local/src/trajopt/build/lib

# test
RUN cd /usr/local/src/trajopt/build; \
	ctest -V
