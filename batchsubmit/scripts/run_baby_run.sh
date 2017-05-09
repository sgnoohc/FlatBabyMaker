#!/bin/bash

# The following are set by the Samples.py from the AutoTwopler
FILEID=${OUTPUT_NAMES}
FILE=${FILENAME}
COPYDIR=${OUTPUT_NAMES}

# Environment setting same as my ditto environment
source /code/osgcode/cmssoft/cmsset_default.sh  > /dev/null 2>&1
export SCRAM_ARCH=slc6_amd64_gcc530   # or whatever scram_arch you need for your desired CMSSW release
export CMSSW_VERSION=CMSSW_8_0_18
cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/cmssw/$CMSSW_VERSION/src
eval `scramv1 runtime -sh`
cd -

# Untar the packages that contains jet energy correction config files and the libCMS3_CORE.so (it's always called packages)
# Also the following tar ball must contain ALL the binaries and every files you need to send except the sweepRoot and merge scripts
echo "Before untarring"
tar xzf package.tar.gz

echo "Before cat"
cat *.sh
cat *.exe

ls -dl /hadoop/cms/store/user/phchang
ls -l /hadoop/cms/store/user/phchang

echo "Before running ditto_ana_mybaby"
# Now run the baby maker!
export LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH
ldd ditto_ana_mybaby
./ditto_ana_mybaby $FILE output 100
