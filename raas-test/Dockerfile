FROM dibyo/trajopt

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

ENV DEBIAN_FRONTEND noninteractive
ENV PYTHONPATH /usr/local/raas/src:/usr/local/raas/src/raas_example:/usr/local/src/trajopt:/usr/local/src/trajopt/build/lib

RUN apt-get install -y apt-utils
RUN apt-get install -y python-numpy python-setuptools
RUN easy_install pytz
RUN pip install tornado celery scipy

