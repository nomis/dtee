The [meson](https://mesonbuild.com/) build system is used to build and install dtee.

    meson build/release               # configure dtee build using the build/release directory
    ninja -C build/release            # compile
    ninja -C build/release test       # run the tests
    ninja -C build/release install    # install to default locations

A `Makefile` that calls `meson` and `ninja` is provided for convenience.

See [the list of dependencies](docs/dependencies.rst) for more information.
