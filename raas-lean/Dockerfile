FROM ubuntu:14.04

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

ENV DEBIAN_FRONTEND noninteractive
ENV PYTHONPATH /opt/raas/repo/src:/opt/raas/repo/src/raas_example

RUN apt-get update
RUN apt-get install -y git sudo
RUN apt-get install -y python python-dev
RUN apt-get install -y python-numpy python-pip python-celery python-couchdb
RUN pip install pytz
RUN apt-get remove -y python-librabbitmq
RUN apt-get autoremove -y

ENV C_FORCE_ROOT true

RUN mkdir /opt/raas
RUN mkdir /opt/raas/code
RUN mkdir /opt/raas/datasets
RUN mkdir /opt/raas/deffiles
RUN mkdir /opt/raas/repo
RUN ln -s /opt/raas/repo/scripts /opt/raas/code/bin
RUN ln -s /opt/raas/repo/src /opt/raas/code/src

# EXPOSE 8888
# EXPOSE 5672
# EXPOSE 17218
# EXPOSE 5984

# git clone --branch benk https://github.com/rll/raas.git /opt/raas/repo
