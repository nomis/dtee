Ubuntu
======

Install the :download:`repository public key <repo-key-rsa.asc>`::

    wget https://dtee.bin.uuid.uk/ubuntu/repo-key.asc -O - | apt-key add -

Follow the instructions for your release. If you are using a newer release than
the ones listed then use the builds for the most recent prior version.

Ubuntu 22.04 LTS (Jammy Jellyfish)
----------------------------------

Add the following APT_ data source
:download:`dtee-ubuntu-jammy.list <../../apt/dtee-ubuntu-jammy.list>`
to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dtee.bin.uuid.uk/ubuntu/ jammy main

Run the following commands::

    apt update
    apt install dtee

Ubuntu 20.04 LTS (Focal Fossa)
------------------------------

Add the following APT_ data source
:download:`dtee-ubuntu-focal.list <../../apt/dtee-ubuntu-focal.list>`
to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dtee.bin.uuid.uk/ubuntu/ focal main

Run the following commands::

    apt update
    apt install dtee

Ubuntu 18.04 LTS (Bionic Beaver)
--------------------------------

Add the following APT_ data source
:download:`dtee-ubuntu-bionic.list <../../apt/dtee-ubuntu-bionic.list>`
to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dtee.bin.uuid.uk/ubuntu/ bionic main

Run the following commands::

    apt update
    apt install dtee

.. _APT: https://en.wikipedia.org/wiki/APT_(Debian)
