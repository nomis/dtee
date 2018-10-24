Build and install
=================

The `meson <https://mesonbuild.com/>`_ build system is used to build and install dtee::

    meson build/release             # configure dtee build
    ninja -C build/release          # compile
    ninja -C build/release test     # run the tests
    ninja -C build/release install  # install to default locations

A ``Makefile`` that calls ``meson`` and ``ninja`` is provided for convenience.

See :doc:`the list of dependencies <dependencies>` for more information on
build, test and runtime requirements.
