# Mostly stolen from Github-User modm-io/avr-gcc (https://github.com/modm-io/avr-gcc.git)
# PLUS
# Added Asuro-Library

## Installation

## Clone into this repo

````sh
git clone https://github.com/WuTangTim/avr-gcc.git
````

## Building locally with Docker

There is a Docker image with all prerequisites for building, created from the `Dockerfile` in this repository.
Build the image from the local `Dockerfile` and start it:

```sh
docker build --tag avr-gcc-prerequisites:local .
docker run -it avr-gcc-prerequisites:local
```

Inside the Docker container get this repository

```sh
git clone https://github.com/modm-io/avr-gcc.git
```

Run the build.sh script

```sh
cd avr-gcc
./build.sh
```

The toolchain will be in `/opt/avr-gcc`.

## Building in Github Actions

There is a Github Actions job defined in `.github/workflows/` which builds the
toolchain. For tagged commits, a Github release will be created with the
toolchain put into a downloadable `.tar.bz2` at
[Releases](https://github.com/modm-ext/docker-avr-gcc/releases).
