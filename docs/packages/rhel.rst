Red Hat Enterprise Linux
========================

Yum will prompt to confirm the repository
:download:`public key <repo-key-rsa.asc>`:

.. code-block:: none

   4784 9A12 DAF9 BD2A F550 5FBB 4FF8 86F3 1820 6BD9

Red Hat Enterprise Linux 6
--------------------------

Boost 1.60 from the Red Hat Software Collections is required.

Save the repository configuration file
:download:`dtee-rhel6.repo <../../yum/dtee-rhel6.repo>`
to ``/etc/yum.repos.d/dtee-rhel6.repo``.

Run the following commands::

    subscription-manager repos --enable "rhel-*-rhscl-6-rpms"
    yum install dtee

Red Hat Enterprise Linux 7
--------------------------

Save the repository configuration file
:download:`dtee-rhel7.repo <../../yum/dtee-rhel7.repo>`
to ``/etc/yum.repos.d/dtee-rhel7.repo``.

Run the following command::

    yum install dtee

Red Hat Enterprise Linux 8
--------------------------

Save the repository configuration file
:download:`dtee-rhel8.repo <../../yum/dtee-rhel8.repo>`
to ``/etc/yum.repos.d/dtee-rhel8.repo``.

Run the following command::

    yum install dtee
