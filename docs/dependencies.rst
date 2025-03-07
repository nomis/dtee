Dependencies
============

The following tools and libraries are required as part of the build process, to
run dtee or to produce documentation.

Build
-----

Compile
~~~~~~~
  * `Boost <https://www.boost.org/>`_ 1.66+ (1.82+ preferred)
  * `Clang <https://clang.llvm.org/>`_ 6+ or `GNU GCC <https://www.gnu.org/software/gcc/>`_ 7+
  * `GNU gettext <https://www.gnu.org/software/gettext/>`_
  * `GNU Make <https://www.gnu.org/software/make/>`_ 3.80+ (optional)
  * `Meson <https://mesonbuild.com/>`_ 0.63+

    * `Ninja <https://ninja-build.org/>`_
    * `Python 3 <https://www.python.org/>`_

..
   Version upgrades must be satisfied by all supported Linux distributions.

Static Analysis
~~~~~~~~~~~~~~~
  * `Clang Static Analyzer <https://clang-analyzer.llvm.org/>`_
  * `Cppcheck <https://cppcheck.sourceforge.io/>`_

Test
~~~~
  * `GNU Bash <https://www.gnu.org/software/bash/>`_ 4.1+
  * `GNU Core Utilities <https://www.gnu.org/software/coreutils/>`_
  * `GNU Diff Utilities <https://www.gnu.org/software/diffutils/>`_
  * `GNU Find Utilities <https://www.gnu.org/software/findutils/>`_
  * `GNU GCC gcov <https://gcc.gnu.org/onlinedocs/gcc/Gcov.html>`_
  * `lcov <https://github.com/linux-test-project/lcov>`_

Runtime
-------
  * `Boost <https://www.boost.org/>`_
  * `GNU libintl <https://www.gnu.org/software/gettext/>`_
  * UNIX domain sockets

Documentation
-------------
  * `Sphinx <https://www.sphinx-doc.org/>`_ 4.1.0+
