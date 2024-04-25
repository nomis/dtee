Ubuntu
======

Follow the instructions for your release. If you are using a newer release than
the ones listed then use the builds for the most recent prior version.

Ubuntu 24.04 LTS (Noble Numbat)
-------------------------------

Run the following commands to install the :download:`repository public key
<repo-key-rsa.gpg>`, APT_ data source :download:`dtee-ubuntu-noble.list
<../../apt/dtee-ubuntu-noble.list>` and then ``dtee``::

    wget https://dtee.bin.uuid.uk/ubuntu/repo-key.gpg \
        -O /etc/apt/keyrings/dtee.gpg

    echo "deb [signed-by=/etc/apt/keyrings/dtee.gpg]" \
        "https://dtee.bin.uuid.uk/ubuntu/ noble main" \
        >/etc/apt/sources.list.d/dtee.list

    apt update
    apt install dtee

Ubuntu 22.04 LTS (Jammy Jellyfish)
----------------------------------

Run the following commands to install the :download:`repository public key
<repo-key-rsa.gpg>`, APT_ data source :download:`dtee-ubuntu-jammy.list
<../../apt/dtee-ubuntu-jammy.list>` and then ``dtee``::

    wget https://dtee.bin.uuid.uk/ubuntu/repo-key.gpg \
        -O /etc/apt/keyrings/dtee.gpg

    echo "deb [signed-by=/etc/apt/keyrings/dtee.gpg]" \
        "https://dtee.bin.uuid.uk/ubuntu/ jammy main" \
        >/etc/apt/sources.list.d/dtee.list

    apt update
    apt install dtee

Ubuntu 20.04 LTS (Focal Fossa)
------------------------------

Run the following commands to install the :download:`repository public key
<repo-key-rsa.gpg>`, APT_ data source :download:`dtee-ubuntu-focal.list
<../../apt/dtee-ubuntu-focal.list>` and then ``dtee``::

    mkdir -m 0755 -p /etc/apt/keyrings

    wget https://dtee.bin.uuid.uk/ubuntu/repo-key.gpg \
        -O /etc/apt/keyrings/dtee.gpg

    echo "deb [signed-by=/etc/apt/keyrings/dtee.gpg]" \
        "https://dtee.bin.uuid.uk/ubuntu/ focal main" \
        >/etc/apt/sources.list.d/dtee.list

    apt update
    apt install dtee

.. _APT: https://en.wikipedia.org/wiki/APT_(Debian)
