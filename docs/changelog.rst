Change log
==========

.. Unreleased_
.. -----------

1.0.1_ |--| 2018-12-22
----------------------

Update to avoid causing a trivial memory leak in Boost.

Fixed
~~~~~

* Memory leak in Boost program_options resulting from differing
  ``boost::smart_ptr`` implementations (`boostorg/program_options issue #70
  <https://github.com/boostorg/program_options/issues/70>`_).

1.0.0_ |--| 2018-12-09
----------------------

First stable release.

Added
~~~~~

* Best effort support on Darwin_ (macOS).
* Best effort support on Cygwin_.

Fixed
~~~~~

* Invalid usage messages now use standard error instead of standard output.
* Check build version matches the release version.

0.0.1_ |--| 2018-11-11
----------------------

Update to allow improvements in packaging.

Fixed
~~~~~

* Infinite loop in the test scripts if check variables are undefined (this is
  unlikely).
* Support for `unity builds <https://mesonbuild.com/Unity-builds.html>`_ when
  ``-Wshadow`` is used.

0.0.0_ |--| 2018-11-09
----------------------

Initial development release for packaging.

Added
~~~~~

* Full support on Linux_.
* Best effort support on FreeBSD_ and OpenBSD_.
* Basic support on NetBSD_ and DragonFlyBSD_.
* Compiles on `GNU Hurd`_ (but doesn't work).
* Comprehensive automated tests of all functionality.

.. |--| unicode:: U+2013 .. EN DASH

.. _Linux: https://www.kernel.org/
.. _FreeBSD: https://www.freebsd.org/
.. _OpenBSD: https://www.openbsd.org/
.. _NetBSD: https://www.netbsd.org/
.. _DragonFlyBSD: https://www.dragonflybsd.org/
.. _GNU Hurd: https://www.gnu.org/software/hurd/
.. _Darwin: https://opensource.apple.com/
.. _Cygwin: https://www.cygwin.com/

.. _Unreleased: https://github.com/nomis/dtee/compare/1.0.1...HEAD
.. _1.0.1: https://github.com/nomis/dtee/compare/1.0.0...1.0.1
.. _1.0.0: https://github.com/nomis/dtee/compare/0.0.1...1.0.0
.. _0.0.1: https://github.com/nomis/dtee/compare/0.0.0...0.0.1
.. _0.0.0: https://github.com/nomis/dtee/commits/0.0.0
