dtee (double tee)
=================

![](logo.svg)

## Description

Run a program with standard output and standard error copied to files
while maintaining the original standard output and standard error as normal.

## Purpose

To run programs from cron, suppressing all output unless the process outputs an
error message or has a non-zero exit status whereupon the original output will
be written as normal and the exit code will be appended to standard error.

It will do this by default when executed as `cronty`, providing an alternative
to [cronic](https://habilis.net/cronic/) but without splitting up the output.

(Also, to do [`tee(1)`](http://man7.org/linux/man-pages/man1/tee.1.html) with
standard output and standard error at the same time.)

## Limitations

Datagram sockets can only process writes as individual packets with a maximum
packet size. Therefore, if the program being run attempts to [`write(2)`](http://man7.org/linux/man-pages/man2/write.2.html)
more than this size in one call the write will fail and part of the output will
be lost.

This is not usually a problem because the default socket buffer size is usually
much higher than the size programs typically write with. For safety, the socket
buffer size will be increased to [`PIPE_BUF`](http://man7.org/linux/man-pages/man0/limits.h.0p.html)
if the default is smaller than this value.

If the process uses [`sendfile(2)`](http://man7.org/linux/man-pages/man2/sendfile.2.html)
then (on Linux) the writes occur in [`PIPE_BUF`](http://man7.org/linux/man-pages/man0/limits.h.0p.html)
sized chunks so it works as normal, but why are you using an interactive program
that outputs such large quantities of data?

For more details read the [architecture](ARCHITECTURE.md) document.
