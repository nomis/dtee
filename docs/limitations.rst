Limitations
===========

Datagram sockets can only process writes as individual packets with a maximum
packet size. Therefore, if the program being run attempts to |write(2)|_
more than this size in one call the write will fail and that part of the output
will be lost.

This is not usually a problem because the default socket buffer size is usually
much higher than the size programs typically write with. For safety, the socket
buffer size will be increased to at least |PIPE_BUF|_ and |BUFSIZ|_ if the
default is smaller than these values.

Writes to the socket (on Linux or GNU Hurd) will block until there is capacity
available in the socket buffer. If the process uses |sendfile(2)|_ then (on
Linux) the writes occur in |PIPE_BUF|_ sized chunks so it works as normal, but
why are you using an interactive program that outputs such large quantities of
data?

It is not possible to open ``/dev/stdout`` or ``/dev/stderr`` because they are
sockets. No program would need to do this but like the use of ``/dev/stdin``
it may be desirable in scripts to work around limitations in file descriptor
handling (however this is unlikely for an interactive program).

For more details read the :doc:`architecture <architecture>` document.

FreeBSD/OpenBSD/Darwin
----------------------

Writes to the socket do not block when the receive buffer of the peer socket is
full. The default socket receive buffer is quite small so it will be raised to
512KB (for the send buffer, 256KB). This avoids problems most of the time.

Messages are likely to be lost from programs that write large amounts of data
(more than 128..256KB) very quickly or do so inefficiently (1 byte at a time).

NetBSD
------

Like `FreeBSD/OpenBSD/Darwin`_ but the socket buffer can only be raised to
128KB so messages are very likely to be lost if data is written quickly. Unlike
the other BSDs, this will result in an error on the receive call so it will not
go unreported.

DragonFlyBSD
------------

Like `FreeBSD/OpenBSD/Darwin`_ but even with a 512KB socket buffer it loses
messages of |PIPE_BUF|_ size that are written very quickly. Writes of |BUFSIZ|_
size are ok because they result in fewer messages.

GNU Hurd
--------

Does not currently have support for returning addresses of Unix sockets, so none
of the output works. It may be possible to implement custom pipe-like objects
with three file descriptors in user space.

Writes larger than the page size (4KB) are truncated and there's no way to
increase the size of the socket buffer.

Cygwin
------

Performs as well as Linux but the maximum amount of data that can be streamed
quickly is limited by the size of the socket buffer (which will will be raised
to 2MB).

There are security issues because the underlying implementation of Unix sockets
is a UDP socket on localhost. This presents an opportunity for another process
to bind the same port after ``dtee`` or the command being run exits, which will
allow it to:

* Write additional input for ``dtee`` after the child process has exited (until
  |waitpid(2)|_ is processed).
* Read output from child processes if the program being run forks into the
  background (causing ``dtee`` to exit).
* Read output from child processes if ``dtee`` is killed.

.. |sendfile(2)| replace:: ``sendfile(2)``
.. _sendfile(2): https://man7.org/linux/man-pages/man2/sendfile.2.html

.. |waitpid(2)| replace:: ``waitpid(2)``
.. _waitpid(2): https://man7.org/linux/man-pages/man2/waitpid.2.html

.. |write(2)| replace:: ``write(2)``
.. _write(2): https://man7.org/linux/man-pages/man2/write.2.html

.. |PIPE_BUF| replace:: ``PIPE_BUF``
.. _PIPE_BUF: https://man7.org/linux/man-pages/man0/limits.h.0p.html

.. |BUFSIZ| replace:: ``BUFSIZ``
.. _BUFSIZ: https://man7.org/linux/man-pages/man0/stdio.h.0p.html
