<p align="center">
  <img src="http://www.plagiocephalyflathead.com/i/tummy_time_for_flat_head.jpg" width="350"/>
</p>

# ![Tummy time image]() Flat Baby Maker


This loops over CMS3 (or CMS4) and create a truly flat baby.

CMS3 looper template.
It uses Ditto.

    # setup ROOT
    mkdir cms3looper
    cd cms3looper
    git clone git@github.com:sgnoohc/mybaby.git
    git clone git@github.com:sgnoohc/Ditto.git ditto
    # The master veresion of Ditto should work.
    # But if for some reason it is broken, there is a tagged version of Ditto.
    # cd ditto
    # git checkout tags/v1.0
    # cd ../
    source ditto/bin/setup_ditto.sh mybaby
    ditto_clean
    ditto_make
    ditto_ana_mybaby /path/to/your/cms3 1000    # the 1000 is there to run over 1000 events only. Remove to run over all events.
    ditto_edit  # opens up relevant files in vim. (of course)

[*] (image taken from http://www.plagiocephalyflathead.com/)
