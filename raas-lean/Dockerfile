FROM ubuntu:14.04

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

ENV DEBIAN_FRONTEND noninteractive
ENV PYTHONPATH /opt/raas/repo/src:/opt/raas/repo/src/raas_example

RUN apt-get update
RUN apt-get install -y git sudo
RUN apt-get install -y python python-dev
RUN apt-get install -y python-numpy python-pip python-celery
RUN pip install pytz

ENV C_FORCE_ROOT true

RUN mkdir /opt/raas
RUN mkdir /opt/raas/code
RUN mkdir /opt/raas/datasets
RUN mkdir /opt/raas/deffiles
RUN mkdir /opt/raas/repo

EXPOSE 8888
EXPOSE 5672
EXPOSE 17218

# git clone --branch dibyo https://github.com/rll/raas.git /opt/raas/repo
# ln -s /opt/raas/repo/scripts /opt/raas/code/bin
# ln -s /opt/raas/repo/src /opt/raas/code/src
