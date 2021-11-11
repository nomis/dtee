dtee (double tee)
=================

.. image:: logo.svg
   :alt: Logo (standard output and standard error represented as two separate
         T-junctions that split off the content to each side while the content
         also flows normally)

Description
-----------

Run a program with standard output and standard error copied to files while
maintaining the original standard output and standard error as normal.

Purpose
-------

To run programs from cron, suppressing all output unless the process outputs an
error message or has a non-zero exit status whereupon the original output will
be written as normal and the exit code will be appended to standard error.

It will do this by default when invoked as ``cronty``, providing an alternative
to |cronic|_ but without splitting up the output.

(Also, to do |tee(1)|_ with standard output and standard error at the same
time.)

Contents
--------

.. toctree::
   :titlesonly:
   :maxdepth: 1

   architecture
   dependencies
   limitations
   install
   manual

Resources
---------

.. toctree::
   :titlesonly:
   :maxdepth: 1

   Source code <https://github.com/nomis/dtee>
   Releases <https://github.com/nomis/dtee/releases>
   changelog
   packages
   Issue tracker <https://github.com/nomis/dtee/issues>

Pronunciation
-------------

dtee
   /diː.ˈtiː/

cronty
   /ˈkrɒn.tiː/

.. |cronic| replace:: ``cronic``
.. _cronic: https://habilis.net/cronic/

.. |tee(1)| replace:: ``tee(1)``
.. _tee(1): https://man7.org/linux/man-pages/man1/tee.1.html
