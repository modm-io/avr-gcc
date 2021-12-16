# Up-to-date AVR GNU GCC Toolchain from source

## Installation

Download the latest `avr-gcc.tar.bz2` from [Releases](https://github.com/modm-ext/docker-avr-gcc/releases)
and unpack it to `/opt`:

```sh
tar xf avr-gcc.tar.bz2 --directory /opt
```

Add the `bin/` directories of avr-gcc and avr-binutils to your `$PATH`,
e.g. by adding the following line to your `~/.bashrc` file:

```sh
export PATH="/opt/avr-gcc/avr-gcc/bin:/opt/avr-gcc/avr-binutils/bin:$PATH"
```

## Building locally with Docker

There is a Docker image with all prerequisites for building, created from the `Dockerfile` in this repository.
Pull and start the image from Dockerhub with:

```sh
docker run -it modm/avr-gcc-prerequisites
```

Or build the image from the local `Dockerfile` and start it:

```sh
docker build --tag avr-gcc-prerequisites:local .
docker run -it avr-gcc-prerequisites:local
```

Inside the Docker container get this repository

```sh
git clone https://github.com/modm-ext/docker-avr-gcc.git
```

Run the build.sh script

```sh
cd docker-avr-gcc
time ./build.sh
```

The toolchain will be in `/opt/avr-gcc`.

## Building in Github Actions

There is a Github Actions job defined in `.github/workflows/` which builds the
toolchain. For tagged commits, a Github release will be created with the
toolchain put into a downloadable `.tar.bz2` at
[Releases](https://github.com/modm-ext/docker-avr-gcc/releases).
