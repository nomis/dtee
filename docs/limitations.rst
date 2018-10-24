Limitations
===========

Datagram sockets can only process writes as individual packets with a maximum
packet size. Therefore, if the program being run attempts to |write(2)|_
more than this size in one call the write will fail and part of the output will
be lost.

This is not usually a problem because the default socket buffer size is usually
much higher than the size programs typically write with. For safety, the socket
buffer size will be increased to |PIPE_BUF|_ if the default is smaller than
this value.

If the process uses |sendfile(2)|_ then (on Linux) the writes occur in
|PIPE_BUF|_ sized chunks so it works as normal, but why are you using an
interactive program that outputs such large quantities of data?

For more details read the :doc:`architecture <architecture>` document.

.. |sendfile(2)| replace:: ``sendfile(2)``
.. _sendfile(2): http://man7.org/linux/man-pages/man2/sendfile.2.html

.. |write(2)| replace:: ``write(2)``
.. _write(2): http://man7.org/linux/man-pages/man2/write.2.html

.. |PIPE_BUF| replace:: ``PIPE_BUF``
.. _PIPE_BUF: http://man7.org/linux/man-pages/man0/limits.h.0p.html
