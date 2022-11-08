Debian
======

Install the :download:`repository public key <repo-key-rsa.asc>`::

    wget https://dtee.bin.uuid.uk/debian/repo-key.asc -O /etc/apt/trusted.gpg.d/dtee.asc

Follow the instructions for your release. If you are using a newer release than
the ones listed then use the builds for the most recent prior version.

Debian 11 (bullseye)
--------------------

Add the following APT_ data source
:download:`dtee-debian-bullseye.list <../../apt/dtee-debian-bullseye.list>`
to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dtee.bin.uuid.uk/debian/ bullseye main

Run the following commands::

    apt update
    apt install dtee

Debian 10 (buster)
------------------

Add the following APT_ data source
:download:`dtee-debian-buster.list <../../apt/dtee-debian-buster.list>`
to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dtee.bin.uuid.uk/debian/ buster main

Run the following commands::

    apt update
    apt install dtee

.. _APT: https://en.wikipedia.org/wiki/APT_(Debian)
