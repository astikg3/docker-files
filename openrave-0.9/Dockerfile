FROM scivm/scientific-python-2.7

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

ENV DEBIAN_FRONTEND noninteractive
# ENV PYTHONPATH /usr/local/raas/src:/usr/local/raas/src/raas_example

RUN apt-get install -y apt-utils
RUN apt-get install -y python-numpy python-setuptools
RUN easy_install pytz

# RUN apt-get install -y software-properties-common python-software-properties

# RUN add-apt-repository ppa:openrave/release
# RUN apt-get update
# RUN apt-get install -y openrave

# install dependencies
RUN apt-get install -y libboost-dev  libboost-date-time-dev libboost-thread-dev
RUN apt-get install -y libqt4-dev qt4-dev-tools libxml2-dev libode-dev
RUN apt-get install -y libsoqt4-dev libcoin60-dev
# RUN apt-get install -y collada-dom  # Optional collada support.

# OpenRAVE
RUN git clone --branch master https://github.com/rdiankov/openrave.git /usr/local/src/openrave
RUN cd /usr/local/src/openrave; \
	mkdir build; \
	cd build; \
	cmake ..; \
	make -j3; \
	make install