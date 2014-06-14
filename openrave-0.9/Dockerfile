FROM scivm/scientific-python-2.7

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

ENV DEBIAN_FRONTEND noninteractive
# ENV PYTHONPATH /usr/local/raas/src:/usr/local/raas/src/raas_example

RUN apt-get install -y make cmake

RUN apt-get install -y apt-utils
RUN apt-get install -y python-numpy python-setuptools
RUN easy_install pytz

# RUN apt-get install -y software-properties-common python-software-properties

# RUN add-apt-repository ppa:openrave/release
# RUN apt-get update
# RUN apt-get install -y openrave

# install dependencies
RUN apt-get install -y libpcre++-dev libreadline-dev
RUN apt-get install -y libboost-dev  libboost-date-time-dev libboost-thread-dev
RUN apt-get install -y libqt4-dev qt4-dev-tools libxml2-dev libode-dev
RUN apt-get install -y libsoqt4-dev libcoin60-dev
# RUN apt-get install -y collada-dom  # Optional collada support.

RUN apt-get install -y libpcre3 libpcrecpp0
RUN apt-get install -y libexif-dev

# latest pcre 
ADD ./src/pcre-8.35 /usr/local/src/pcre-8.35
RUN cd /usr/local/src/pcre-8.35; \
        ./configure; \
        make; \
        make install

ADD ./src/collada-dom-2.4.0 /usr/local/src/collada-dom-2.4.0
RUN cd /usr/local/src/collada-dom-2.4.0; \
	mkdir build; \
	cd build; \
	cmake ..; \
	make; \
	make install

# OpenRAVE
RUN git clone --branch master https://github.com/rdiankov/openrave.git /usr/local/src/openrave
ADD ./src/sympy-0.7.5 /usr/local/src/openrave/sympy
RUN cd /usr/local/src/openrave; \
	mkdir build; \
	cd build; \
	cmake ..; \
	make -j3; \
	make install