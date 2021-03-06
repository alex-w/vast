.. File Config.rst

.. include:: global.rst

.. _config_file:

The configuration file
----------------------

Each time it is run, |SExtractor| looks for a configuration file.
If no configuration file is specified in the command-line, it is assumed to be called :file:`default.sex` and to reside in the current directory.
If no configuration file is found, |SExtractor| will use its own internal default configuration.

Creating a configuration file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SExtractor can generate an ASCII dump of its internal default configuration, using the ``-d`` option.
By redirecting the standard output of SExtractor to a file, one creates a configuration file that can easily be modified afterwards:

.. code-block:: console

  $ sex -d > default.sex

A more extensive dump with less commonly used parameters can be generated by using the ``-dd`` option.

Format of the configuration file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The format is ASCII. There must be only one parameter set per line, following the form::

 Config-parameter      Value(s)

Extra spaces or linefeeds are ignored.
Comments must begin with a ``#`` and end with a linefeed.
Values can be of different types: strings (can be enclosed between double quotes), floats, integers, keywords or Boolean (`Y`/`y` or `N`/`n`).
Some parameters accept zero or several values, which must then be separated by commas.
Integers can be given as decimals, in octal form (preceded by digit O), or in hexadecimal (preceded by `0x`).
The hexadecimal format is particularly convenient for writing multiplexed bit values such as binary masks.
Environment variables, written as ``$HOME`` or ``${HOME}`` are expanded.

.. _param_list:

Configuration parameter list
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Here is a complete list of all the configuration parameters known to |SExtractor|.
Please refer to the next sections for a detailed description of their meaning.


