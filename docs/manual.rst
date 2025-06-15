Manual page
===========

Synopsis
--------

**dtee** [*option*]... <*command*> [*arguments*]...

**cronty** [*option*]... <*command*> [*arguments*]...

Description
-----------

Run *command* with standard output and standard error copied to files while
maintaining the original standard output and standard error in the original
order.

Options
-------

Output files
~~~~~~~~~~~~

Standard streams can be written to any number of specified files, in
addition to normal output. Output is not line buffered.

-o <filename>, --out-append=<filename>             Append standard output to *filename*,
                                                   creating the file if it does not exist.
-O <filename>, --out-overwrite=<filename>          Copy standard output to *filename*,
                                                   truncating and overwriting existing content.

-e <filename>, --err-append=<filename>             Append standard error to *filename*,
                                                   creating the file if it does not exist.
-E <filename>, --err-overwrite=<filename>          Copy standard error to *filename*,
                                                   truncating and overwriting existing content.

-c <filename>, --combined-append=<filename>        Append standard output and standard error to
                                                   *filename*, creating the file if it does not
                                                   exist.
-C <filename>, --combined-overwrite=<filename>     Copy standard output and standard error to
                                                   *filename*, truncating and overwriting existing
                                                   content.

General options
~~~~~~~~~~~~~~~

-i, --ignore-interrupts     Ignore keyboard interrupt signals (SIGINT).
                            This does not prevent the command being executed
                            (and other processes in the same progress group)
                            from receiving the signal.

-q, --cron                  Operate in cron mode (this is implied when invoked
                            as **cronty**). Suppresses all output unless the
                            process outputs an error message or has a non-zero
                            exit status whereupon the original output will be
                            written in the original order and the exit code
                            will be appended to standard error.

Miscellaneous
~~~~~~~~~~~~~

-h, --help                  Display usage information and exit.

-V, --version               Output version information and exit.

See also
--------

`Full documentation <https://dtee.readthedocs.io/>`_
