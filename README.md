# mybaby

CMS3 looper template.
It uses Ditto.

    # setup ROOT
    mkdir cms3looper
    cd cms3looper
    git clone git@github.com:sgnoohc/mybaby.git
    git clone git@github.com:sgnoohc/Ditto.git ditto
    source ditto/bin/setup_ditto.sh mybaby
    ditto_clean
    ditto_make
    ditto_ana_mybaby /path/to/your/cms3 1000    # the 1000 is there to run over 1000 events only. Remove to run over all events.
    ditto_edit  # opens up relevant files in vim. (of course)
