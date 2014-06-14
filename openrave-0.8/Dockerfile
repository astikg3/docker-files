FROM scivm/scientific-python-2.7

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

ENV DEBIAN_FRONTEND noninteractive
ENV PYTHONPATH /usr/local/raas/src:/usr/local/raas/src/raas_example

RUN apt-get install -y apt-utils
RUN apt-get install -y python-numpy python-setuptools
RUN easy_install pytz

RUN apt-get install -y software-properties-common python-software-properties

RUN add-apt-repository ppa:openrave/release
RUN apt-get update
RUN apt-get install -y openrave
