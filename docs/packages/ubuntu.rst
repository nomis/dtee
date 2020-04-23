Ubuntu
======

Install the `Bintray public key`_::

    wget https://bintray.com/user/downloadSubjectPublicKey?username=bintray -O - | apt-key add -

Follow the instructions for your release. If you are using a newer release than
the ones listed then use the builds for the most recent prior version.

Ubuntu 16.04 LTS (Xenial Xerus)
-------------------------------

Add the following APT_ data source
:download:`dtee-ubuntu-xenial.list <../../apt/dtee-ubuntu-xenial.list>`
to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dl.bintray.com/dtee/ubuntu/ xenial main

Run the following commands::

    apt update
    apt install dtee

Ubuntu 18.04 LTS (Bionic Beaver)
--------------------------------

Add the following APT_ data source
:download:`dtee-ubuntu-bionic.list <../../apt/dtee-ubuntu-bionic.list>`
to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dl.bintray.com/dtee/ubuntu/ bionic main

Run the following commands::

    apt update
    apt install dtee

.. _Bintray public key: https://bintray.com/bintray
.. _APT: https://en.wikipedia.org/wiki/APT_(Debian)
