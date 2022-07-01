FROM ubuntu:22.04
LABEL maintainer="Niklas Hauser <niklas.hauser@rwth-aachen.de>, Raphael Lehmann <raphael+docker@rleh.de>"
LABEL Description="Image for building the upstream AVR GNU toolchain for Linux"
WORKDIR /work

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y git wget autoconf2.64 build-essential netpbm libmpc-dev libmpfr-dev libgmp-dev libmpfr6 texinfo doxygen flex bison libexpat1-dev slang-xfig \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*
