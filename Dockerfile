FROM gcc:13.3

RUN echo "\n" >> /etc/apt/sources.list
RUN echo "deb http://archive.debian.org/debian/ jessie main" >> /etc/apt/sources.list
RUN echo "deb-src http://archive.debian.org/debian/ jessie main" >> /etc/apt/sources.list
RUN echo "deb http://archive.debian.org/debian-security/ jessie/updates main" >> /etc/apt/sources.list 
RUN echo "deb-src http://archive.debian.org/debian-security/ jessie/updates main" >> /etc/apt/sources.list

RUN apt-get update || true
RUN apt-get install -y gdb
RUN apt-get install -y --force-yes libgmp-dev

