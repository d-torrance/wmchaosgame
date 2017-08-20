# wmchaosgame

Window Maker dockapp displaying a chaos game

## Installation

You will need [libdockapp](http://dockapps.net/libdockapp) to build wmchaosgame.

If building from git (not necessary if using a tarball):

    autoreconf -fvi

Then the usual

    ./configure
    make
    sudo make install

## Usage

    wmchaosgame [OPTIONS]

### Options
    -h, --help                   show this help text and exit
    -v, --version                show program version and exit
    -w, --windowed               run the application in windowed mode
    -n, --vertices <number>      number of vertices (default: 3)
    -r, --fraction <string>      fraction of distance for next pt (default: 0.5)
    -t, --time <number>          time (ms) before drawing next pt (default: 250)
    -m, --maxpoints <number>     max # of points before restarting
                                 (default: 2500)

# Bugs

Please report bugs and feature requests at the
[issues page](https://github.com/d-torrance/wmchaosgame/issues).

# Copyright

Copyright (C) 2017 Doug Torrance <dtorrance@piedmont.edu>

License: [GPL-2+](https://www.gnu.org/licenses/gpl-2.0.html)
