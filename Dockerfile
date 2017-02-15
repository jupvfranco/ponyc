FROM ubuntu:16.04

RUN apt-get update \
 && apt-get install -y make g++ git wget xz-utils \
                       zlib1g-dev libncurses5-dev libssl-dev \
                       libpcre2-dev \
 && rm -rf /var/lib/apt/lists/* \
 && wget http://llvm.org/releases/3.9.1/clang+llvm-3.9.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz \
 && tar xf clang+llvm-3.9.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz \
 && cp -r clang+llvm-3.9.1-x86_64-linux-gnu-ubuntu-16.04/* /usr/local \
 && rm -rf /clang*

WORKDIR /src/ponyc
COPY Makefile LICENSE VERSION /src/ponyc/
COPY src      /src/ponyc/src
COPY lib      /src/ponyc/lib
COPY test     /src/ponyc/test
COPY packages /src/ponyc/packages

RUN make \
 && make install \
 && rm -rf /src/ponyc/build

RUN mkdir /src/main
WORKDIR /src/main

CMD ponyc
