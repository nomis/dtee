Debian
======

Install the `Bintray public key`_::

    wget https://bintray.com/user/downloadSubjectPublicKey?username=bintray -O - | apt-key add -

Follow the instructions for your release. If you are using a newer release than
the ones listed then use the builds for the most recent prior version.

Debian 8 (jessie)
-----------------

Add the following APT_ data source
:download:`dtee-debian-jessie.list <../../apt/dtee-debian-jessie.list>`
to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dl.bintray.com/dtee/debian/ jessie main

Run the following commands::

    apt install apt-transport-https
    apt update
    apt install dtee

Debian 9 (stretch)
------------------

Add the following APT_ data source
:download:`dtee-debian-stretch.list <../../apt/dtee-debian-stretch.list>`
to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dl.bintray.com/dtee/debian/ stretch main

Run the following commands::

    apt install apt-transport-https
    apt update
    apt install dtee

.. _Bintray public key: https://bintray.com/bintray
.. _APT: https://en.wikipedia.org/wiki/APT_(Debian)
