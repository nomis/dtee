Change log
==========

Unreleased_
-----------

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

.. _Unreleased: https://github.com/nomis/dtee/compare/0.0.0...HEAD
.. _0.0.0: https://github.com/nomis/dtee/commits/0.0.0
