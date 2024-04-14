Change log
==========

Unreleased_
-----------

Changed
~~~~~~~

* Use the new Boost.Asio interface for setting the ``SA_RESTART`` flag on signal
  handlers when using Boost 1.82+.
* Disable "error location" in exception messages when building with Boost.Asio
  1.79 because these exceptions are already caught and properly identified.
* Configure standard output and standard error to be blocking.
* Use ``std::filesystem`` from C++17 instead of ``boost::filesystem``.

Fixed
~~~~~

* Error message checks in tests when building with Boost 1.78.
* Locale handling in i18n tests when running on glibc 2.39.

1.1.0_ |--| 2021-05-30
----------------------

Bug fixes and support for internationalisation.

Added
~~~~~

* Internationalisation of output text messages.

Changed
~~~~~~~

* Normalise error messages.
* Make signals non-interrupting by adding the ``SA_RESTART`` flag after
  Boost.Asio sets the signal handler (`chriskohlhoff/asio issue #646
  <https://github.com/chriskohlhoff/asio/issues/646>`_)  instead of trying to
  wrap calls to |sigaction(2)|_.
* Trivial performance improvements.
* Improved robustness when signals are received between forking a child process
  and executing the command.
* Errors writing to additional output files will always be reflected in the exit
  status even in cron mode.

Fixed
~~~~~

* Race condition between the command immediately exiting and being ready to
  handle ``SIGCHLD``.
* Handle errors when the temporary directory name is too long or unusable.

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

.. |sigaction(2)| replace:: ``sigaction(2)``
.. _sigaction(2): https://man7.org/linux/man-pages/man2/sigaction.2.html

.. _Unreleased: https://github.com/nomis/dtee/compare/1.1.0...HEAD
.. _1.1.0: https://github.com/nomis/dtee/compare/1.0.1...1.1.0
.. _1.0.1: https://github.com/nomis/dtee/compare/1.0.0...1.0.1
.. _1.0.0: https://github.com/nomis/dtee/compare/0.0.1...1.0.0
.. _0.0.1: https://github.com/nomis/dtee/compare/0.0.0...0.0.1
.. _0.0.0: https://github.com/nomis/dtee/commits/0.0.0
