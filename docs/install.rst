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

Cygwin
------

The build and test process makes use of symbolic links. If the repository is
cloned outside of Cygwin these will not be present unless `unprivileged symbolic
links <https://blogs.windows.com/windowsdeveloper/2016/12/02/symlinks-windows-10/>`_
are enabled and all environments are configured to use native symbolic links.

Some of the tests cannot be run from an elevated process and will be skipped.
