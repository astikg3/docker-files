FROM scivm/scientific-python-2.7

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get install libfribidi-bin
RUN add-apt-repository ppa:uck-team/uck-stable; \
    apt-get update; \
    apt-get install uck-stable

ADD /ubuntu_images /usr/local/images