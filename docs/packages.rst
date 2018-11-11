Packages
========

Source packages for Linux distributions are kept in the `dtee-package repository
<https://github.com/nomis/dtee-package>`_.

Debian binary packages
----------------------

Packages are available on `Bintray <https://bintray.com/dtee>`_ for
`Debian <https://bintray.com/dtee/debian/dtee>`_
and `Ubuntu <https://bintray.com/dtee/ubuntu/dtee>`_.

Install the `Bintray public key <https://bintray.com/bintray>`_::

    wget https://bintray.com/user/downloadSubjectPublicKey?username=bintray -O - | apt-key add -

Follow the instructions for your specific distribution. If you are using a newer
distribution than the ones listed then use the builds for the most recent prior
version.

Debian 8 (jessie)
~~~~~~~~~~~~~~~~~

Add the following APT_ data source to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dl.bintray.com/dtee/debian/ jessie main

Run the following commands:
  1. ``apt install apt-transport-https``
  2. ``apt update``
  3. ``apt install dtee``

Debian 9 (stretch)
~~~~~~~~~~~~~~~~~~

Add the following APT_ data source to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dl.bintray.com/dtee/debian/ stretch main

Run the following commands:
  1. ``apt install apt-transport-https``
  2. ``apt update``
  3. ``apt install dtee``

Ubuntu 16.04 LTS (Xenial Xerus)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Add the following APT_ data source to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dl.bintray.com/dtee/ubuntu/ xenial main

Run the following commands:
  1. ``apt update``
  2. ``apt install dtee``

Ubuntu 18.04 LTS (Bionic Beaver)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Add the following APT_ data source to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dl.bintray.com/dtee/ubuntu/ bionic main

Run the following commands:
  1. ``apt update``
  2. ``apt install dtee``

.. _APT: https://en.wikipedia.org/wiki/APT_(Debian)
