
# How to batch submit baby jobs

First, create the ```__package``` by running the following script to copy all the relevant files to send to each worker node of condor.

    sh scripts/make_job_inputs.sh

Then, copy the runtemplate to another directory in this parent directory.

    cp -r runtemplate campaign_test1

Then cd into campaign_test1 and run the following,

    python ducks.py
