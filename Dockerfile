FROM ubuntu:18.04


RUN mkdir /project
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y make zip curl wget git doxygen


RUN wget https://github.com/Kitware/CMake/releases/download/v3.21.4/cmake-3.21.4-linux-x86_64.sh
RUN mkdir /opt/cmake
RUN sh /cmake-3.21.4-linux-x86_64.sh --prefix=/opt/cmake --skip-license
RUN ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake


RUN wget -O compiler.tar.gz https://github.com/FunkyPancake/KeCommon/releases/download/compiler/mcu-compiler-11.4.tar.gz
RUN tar xf compiler.tar.gz -C /opt
RUN rm compiler.tar.gz
RUN apt-get autoclean -y
RUN apt-get autoremove -y
RUN apt-get clean


ENV PATH=/opt/tools/bin:$PATH


WORKDIR /project
