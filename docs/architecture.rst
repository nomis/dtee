Architecture
============

Background
----------

Commands on Unix systems output messages to two logical streams: standard output
(for normal messages) and standard error (for error messages). In the normal
execution of the command there may be messages of both types being output and
they are usually related so it's important that they appear in the correct
order. This is normally achieved by having the file descriptors of both
streams be the same underlying destination (a terminal, pipe to another
process, a file). There is no distinction between streams by the operating
system when the message is written.

Problem
-------

Commands are not always run separately by a user on a terminal. They may be run
from a script or unattended from |cron(8)|_.

When this happens it can be useful to know if the process wrote any error
messages, so typically the file descriptors for standard output and standard
error would be different destinations (two separate log files). The script can
then easily distinguish between normal messages and error messages.

This works well to determine the outcome of the command and examine any
messages it outputs. A problem arises when it is necessary to provide the
original output of the command to a user. It is not possible to guarantee the
reading of messages from two file descriptors in the correct order and there
is no assistance provided by the operating system for doing this.

The output from commands can be confusing if the messages are no longer in the
original order. Splitting the output up into two blocks (normal and error) is
not helpful.

Workarounds
~~~~~~~~~~~

One option has been to use |LD_PRELOAD|_ to modify the behaviour of the process
and identify the destination stream as the messages are being written. This is
error-prone because there are lots of different library functions for outputting
to standard streams as well as functions within the C library that may bypass
their external API and output messages directly.

Processes may also write directly to the file descriptors using system calls
or their executables may be statically linked, preventing preloading from
working. They may have multiple threads to complicate the manipulation of
messages. For security reasons it's not possible to preload libraries into
setuid executables using |LD_PRELOAD|_ so this option doesn't work for those
commands.

Solution
--------

Splitting of standard output and standard error while retaining the order of
output can be performed using three |unix(7)|_ datagram sockets. A single input
socket is used (so that messages can be read in order) and two output sockets
are connected to the same input socket (so that they share the same reliable
ordered buffer).

The source address of each message is provided by the operating system on every
read so it is possible to identify which output stream was used by binding to
different paths for each stream.

Alternatives
~~~~~~~~~~~~

A new flag ``O_SYNCPIPE`` for |pipe2(2)|_ that creates a pair of pipes (4 file
descriptors in total) where the read order of data is linked across the pair.
This would  have behaviour like ``O_DIRECT`` when transitioning from one stream
to the other and only one of the pipes would be available for reading at any one
time.

.. |cron(8)| replace:: ``cron(8)``
.. _cron(8): https://man7.org/linux/man-pages/man8/cron.8.html

.. |pipe2(2)| replace:: ``pipe2(2)``
.. _pipe2(2): https://man7.org/linux/man-pages/man2/pipe2.2.html

.. |unix(7)| replace:: ``unix(7)``
.. _unix(7): https://man7.org/linux/man-pages/man7/unix.7.html

.. |LD_PRELOAD| replace:: ``LD_PRELOAD``
.. _LD_PRELOAD: https://man7.org/linux/man-pages/man8/ld.so.8.html
