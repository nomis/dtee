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

RPM Packages
------------

Packages are available on Bintray for
`Fedora <https://bintray.com/dtee/fedora/dtee>`_.
and `RedHat Enterprise Linux <https://bintray.com/dtee/redhat/dtee>`_

Fedora
~~~~~~

Save the repository configuation file
`dtee-fedora.repo <_static/dtee-fedora.repo>`_
to ``/etc/yum.repos.d/dtee-fedora.repo``::

    [dtee]
    name=dtee ($releasever/$basearch)
    baseurl=https://dl.bintray.com/dtee/fedora/$releasever/$basearch
    enabled=1
    gpgcheck=0
    repo_gpgcheck=1
    gpgkey=https://dl.bintray.com/dtee/fedora/$releasever/$basearch/repodata/repomd.xml.key
    
    [dtee-source]
    name=dtee ($releasever/Source)
    baseurl=https://dl.bintray.com/dtee/fedora/$releasever/source
    enabled=0
    gpgcheck=0
    repo_gpgcheck=1
    gpgkey=https://dl.bintray.com/dtee/fedora/$releasever/source/repodata/repomd.xml.key

Run the following commands:
  1. ``yum install dtee``

RedHat Enterprise Linux 7
~~~~~~~~~~~~~~~~~~~~~~~~~

Save the repository configuration file
`dtee-rhel7.repo <_static/dtee-rhel7.repo>`_
to ``/etc/yum.repos.d/dtee-rhel7.repo``::

    [dtee]
    name=dtee (7/$basearch)
    baseurl=https://dl.bintray.com/dtee/redhat/7/$basearch
    enabled=1
    gpgcheck=0
    repo_gpgcheck=1
    gpgkey=https://dl.bintray.com/dtee/redhat/7/$basearch/repodata/repomd.xml.key
    
    [dtee-source]
    name=dtee (7/Source)
    baseurl=https://dl.bintray.com/dtee/redhat/7/source
    enabled=0
    gpgcheck=0
    repo_gpgcheck=1
    gpgkey=https://dl.bintray.com/dtee/redhat/7/source/repodata/repomd.xml.key

Run the following commands:
  1. ``yum install dtee``

.. _APT: https://en.wikipedia.org/wiki/APT_(Debian)
