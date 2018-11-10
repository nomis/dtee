dtee (double tee) [![](https://travis-ci.org/nomis/dtee.svg?branch=master)](https://travis-ci.org/nomis/dtee "Build Status") [![Download](https://api.bintray.com/packages/nomis/dtee/dtee/images/download.svg)](https://bintray.com/nomis/dtee/dtee/_latestVersion "Download")
=================

![](docs/logo.svg)

## Description

Run a program with standard output and standard error copied to files while
maintaining the original standard output and standard error as normal.

## Purpose

To run programs from cron, suppressing all output unless the process outputs an
error message or has a non-zero exit status whereupon the original output will
be written as normal and the exit code will be appended to standard error.

It will do this by default when invoked as `cronty`, providing an alternative
to [`cronic`](https://habilis.net/cronic/) but without splitting up the output.

(Also, to do [`tee(1)`](http://man7.org/linux/man-pages/man1/tee.1.html) with
standard output and standard error at the same time.)

# Documentation

[Read the documentation](https://dtee.readthedocs.io/) generated from the
[docs](docs/) directory.
