export SCRAM_ARCH=slc6_amd64_gcc530   # or whatever scram_arch you need for your desired CMSSW release
export CMSSW_VERSION=CMSSW_8_0_18
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/cmssw/$CMSSW_VERSION/src
eval `scramv1 runtime -sh`
cd -

source unset_ditto
source ~/ditto/bin/thisditto.sh
source setup_ditto /home/users/phchang/analyses/mybabyframework/mybabymaker/ QUIET
source ditto_include_core ~/cmstas/core

pushd ~/cmstas/NtupleTools/AutoTwopler/
source setup.sh
popd
