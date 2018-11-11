Packages
========

Source packages for Linux distributions are kept in the `dtee-package repository`_.

Debian binary packages
----------------------

Packages are available on Bintray_ for Debian_ and Ubuntu_.

Install the `Bintray public key`_::

    wget https://bintray.com/user/downloadSubjectPublicKey?username=bintray -O - | apt-key add -

Follow the instructions for your specific distribution. If you are using a newer
distribution than the ones listed then use the builds for the most recent prior
version.

Debian 8 (jessie)
~~~~~~~~~~~~~~~~~

Add the following APT_ data source to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dl.bintray.com/dtee/debian/ jessie main

Run the following commands::

    apt install apt-transport-https
    apt update
    apt install dtee

Debian 9 (stretch)
~~~~~~~~~~~~~~~~~~

Add the following APT_ data source to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dl.bintray.com/dtee/debian/ stretch main

Run the following commands::

    apt install apt-transport-https
    apt update
    apt install dtee

Ubuntu 16.04 LTS (Xenial Xerus)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Add the following APT_ data source to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dl.bintray.com/dtee/ubuntu/ xenial main

Run the following commands::

    apt update
    apt install dtee

Ubuntu 18.04 LTS (Bionic Beaver)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Add the following APT_ data source to ``/etc/apt/sources.list.d/dtee.list``::

    deb https://dl.bintray.com/dtee/ubuntu/ bionic main

Run the following commands::

    apt update
    apt install dtee


RPM Packages
------------

Packages are available on Bintray_ for Fedora_ and `RedHat Enterprise Linux`_.

Fedora
~~~~~~

Save the repository configuation file
:download:`dtee-fedora.repo <../yum/dtee-fedora.repo>`
to ``/etc/yum.repos.d/dtee-fedora.repo``.

Run the following commands::

    yum install dtee

RedHat Enterprise Linux 7
~~~~~~~~~~~~~~~~~~~~~~~~~

Save the repository configuration file
:download:`dtee-rhel7.repo <../yum/dtee-rhel7.repo>`
to ``/etc/yum.repos.d/dtee-rhel7.repo``.

Run the following commands::

    yum install dtee

.. _dtee-package repository: https://github.com/nomis/dtee-package
.. _Bintray: https://bintray.com/dtee
.. _Bintray public key: https://bintray.com/bintray
.. _Debian: https://bintray.com/dtee/debian/dtee
.. _Ubuntu: https://bintray.com/dtee/ubuntu/dtee
.. _Fedora: https://bintray.com/dtee/fedora/dtee
.. _RedHat Enterprise Linux: https://bintray.com/dtee/redhat/dtee
.. _APT: https://en.wikipedia.org/wiki/APT_(Debian)
