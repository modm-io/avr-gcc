# Up-to-date AVR GNU GCC Toolchain from source

## Installation

Download the latest `modm-avr-gcc.tar.bz2` from [Releases](https://github.com/modm-ext/docker-avr-gcc/releases)
and unpack it to `/opt`:

```sh
tar xf modm-avr-gcc.tar.bz2 --directory /opt
```

Add the `bin/` directory to your `$PATH`,
e.g. by adding the following line to your `~/.bashrc` file:

```sh
export PATH="/opt/avr-gcc/bin:$PATH"
```

## Building in Github Actions

There is a Github Actions job defined in `.github/workflows/` which builds the
toolchain. For tagged commits, a Github release will be created with the
toolchain put into a downloadable `.tar.bz2` at
[Releases](https://github.com/modm-ext/docker-avr-gcc/releases).
