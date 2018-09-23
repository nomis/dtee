dtee (double tee)
=================

## Description

Run a program with standard output and standard error copied to files
while maintaining the original standard output and standard error as normal.

## Purpose

To run programs from cron, suppressing all normal output unless the process
outputs an error message or has a non-zero exit status whereupon the original
output will be combined and written to standard output. The exit code will be
written to standard error.

It will do this by default when executed as `cronic`, providing an alternative
to [the Bash script of the same name](http://habilis.net/cronic/) but without
splitting up the output.

(Also, to do `tee` with standard output and standard error at the same time.)

## Limitations

Splitting of standard output and standard error while retaining the order of
output is performed using three local datagram sockets. It is not possible to
do this with two pairs of stream sockets. Datagram sockets can only process
writes as individual packets with a maximum packet size. Therefore, if the
program being run attempts to `write()` more than this size in one call the
write will fail and the message will be lost.

This is not usually a problem because the default socket buffer size is usually
much higher than the size programs typically write with. For safety, the socket
buffer size will be increased to `PIPE_BUF` if the default is smaller than this
value.
