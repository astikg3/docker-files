FROM ubuntu:14.04

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

ENV DEBIAN_FRONTEND noninteractive
ENV PYTHONPATH /opt/raas/repo/src:/opt/raas/repo/src/raas_example

RUN apt-get install -y git sudo
RUN apt-get install -y python2.7 python2.7-dev

RUN mkdir /opt/raas
RUN mkdir /opt/raas/code
RUN mkdir /opt/raas/datasets
RUN mkdir /opt/raas/deffiles
RUN mkdir /opt/raas/repo

# git clone --branch dibyo https://github.com /rll/raas.git /opt/raas/repo
# ln -s /opt/raas/repo/scripts /opt/raas/code/bin
# ln -s /opt/raas/repo/src /opt/raas/code/src
