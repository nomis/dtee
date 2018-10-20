## Background

Commands on Unix systems output messages to two logical streams: standard output
(for normal messages) and standard error (for error messages). In the normal
execution of the command there may be messages of both types being output and
they are usually related so it's important that they appear in the correct
order. This is normally achieved by having the file descriptors of both
streams be the same underlying destination (a terminal, pipe to another
process, a file). There is no distinction between streams by the operating
system when the message is written.

## Problem

Commands are not always run separately by a user on a terminal. They may be run
from a script or unattended from [`cron(8)`](http://man7.org/linux/man-pages/man8/cron.8.html).
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

### Workarounds

One option has been to use [`LD_PRELOAD`](http://man7.org/linux/man-pages/man8/ld.so.8.html)
to modify the behaviour of the process and identify the destintion stream as
the messages are being written. This is error-prone because there are lots of
different library functions for outputting to standard streams as well as
functions within the C library that may bypass their external API and output
messages directly.

Processes may also write directly to the file descriptors using system calls
or their executables may be statically linked, preventing preloading from
working. They may have multiple threads to complicate the manipulation of
messages. For security reasons it's not possible to preload libraries into
setuid executables using `LD_PRELOAD` so this option doesn't work for those
commands.

## Solution

Splitting of standard output and standard error while retaining the order of
output can be performed using three [`unix(7)`](http://man7.org/linux/man-pages/man7/unix.7.html)
datagram sockets. A single input socket is used (so that messages can be read in
order) and two output sockets are connected to the same input socket (so that
they share the same reliable ordered buffer).

The source address of each message is provided by the operating system on every
read so it is possible to identify which output stream was used by binding to
different paths for each stream.

It would be preferable to use sequenced-packet sockets instead but that would
require two pairs of sockets because it is not possible to have one socket
connected to two peers. Perhaps there will be a `sockettriple` function in the
future that can do this.

### Alternatives

It could be possible to use [`sctp(7)`](http://man7.org/linux/man-pages/man7/sctp.7.html)
instead but that may not be available on all platforms and would require using
the local network interface for communication.
