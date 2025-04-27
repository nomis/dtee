Debian
======

Follow the instructions for your release. If you are using a newer release than
the ones listed then use the builds for the most recent prior version.

Debian 12 (bookworm)
--------------------

Run the following commands to install the :download:`repository public key
<repo-key-rsa.gpg>`, APT_ data source :download:`dtee-debian-bookworm.list
<../../apt/dtee-debian-bookworm.list>` and then ``dtee``::

    wget https://dtee.bin.uuid.uk/debian/repo-key.gpg \
        -O /etc/apt/keyrings/dtee.gpg

    echo "deb [signed-by=/etc/apt/keyrings/dtee.gpg]" \
        "https://dtee.bin.uuid.uk/debian/ bookworm main" \
        >/etc/apt/sources.list.d/dtee.list

    apt update
    apt install dtee

Debian 11 (bullseye)
--------------------

Run the following commands to install the :download:`repository public key
<repo-key-rsa.gpg>`, APT_ data source :download:`dtee-debian-bullseye.list
<../../apt/dtee-debian-bullseye.list>` and then ``dtee``::

    mkdir -m 0755 -p /etc/apt/keyrings

    wget https://dtee.bin.uuid.uk/debian/repo-key.gpg \
        -O /etc/apt/keyrings/dtee.gpg

    echo "deb [signed-by=/etc/apt/keyrings/dtee.gpg]" \
        "https://dtee.bin.uuid.uk/debian/ bullseye main" \
        >/etc/apt/sources.list.d/dtee.list

    apt update
    apt install dtee

.. _APT: https://en.wikipedia.org/wiki/APT_(Debian)
