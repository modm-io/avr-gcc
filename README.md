# Build AVR GNU GCC Toolchain from source

[![CircleCI](https://circleci.com/gh/salkinium/docker-avr-gcc-7.svg?style=svg)](https://circleci.com/gh/salkinium/docker-avr-gcc-7)

This work is based on the work of
[@strongly-typed](https://github.com/strongly-typed/build-atmel-avr-gnu-toolchain).

## Building locally with Docker

There is a Docker image with all prerequisites for building. Start it with:

```sh
    docker run -it salkinium/avr-gcc-prerequisites bash
```

Inside the Docker container get this repository

```sh
    git clone https://github.com/salkinium/docker-avr-gcc-7.git
```

Run the build.sh script

```sh
    cd docker-avr-gcc-7
    time bash build.sh
```

The toolchain will be in `avr-gcc`.

## Building in CircleCI

There is a CircleCI job defined in `.circleci/config.yml` which builds the
toolchain using the Docker container. For tagged commits, a Github release
will be created with the toolchain put into a downloadable `.tar.bz2` at
[Releases](https://github.com/salkinium/docker-avr-gcc-7/releases).