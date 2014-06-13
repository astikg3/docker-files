FROM dibyo/openrave

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

RUN apt-get install -y libopenscenegraph-dev cmake libboost-all-dev libeigen3-dev

# add gurobi
ADD ./src/gurobi563 /usr/local/src/gurobi563
RUN python /usr/local/src/gurobi563/linux64/setup.py install
ENV GUROBI_HOME /usr/local/src/gurobi563/linux64
