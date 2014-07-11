FROM scivm/scientific-python-2.7

MAINTAINER Dibyo Majumdar <dibyo.majumdar@gmail.com>

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get install -y libfribidi0
RUN add-apt-repository ppa:uck-team/uck-stable; \
    apt-get update; \
    apt-get install -y uck

ADD /ubuntu_images /usr/local/images

# apt-get install gcc g++ bcc iasl xsltproc uuid-dev zlib1g-dev libidl-dev \
                # libsdl1.2-dev libxcursor-dev libasound2-dev libstdc++5 \
                # libhal-dev libpulse-dev libxml2-dev libxslt1-dev \
                # python-dev libqt4-dev qt4-dev-tools libcap-dev \
                # libxmu-dev mesa-common-dev libglu1-mesa-dev \
                # linux-kernel-headers libcurl4-openssl-dev libpam0g-dev \
                # libxrandr-dev libxinerama-dev libqt4-opengl-dev makeself \
                # libdevmapper-dev default-jdk python-central \
                # texlive-latex-base \
                # texlive-latex-extra texlive-latex-recommended \
                # texlive-fonts-extra texlive-fonts-recommended
