#!/bin/bash

#THISDIR=`pwd`
#cd ..
#tar -czf batchsubmit/job_input/package.tar.gz *.so puWeight2016.root btagsf leptonSFs xsec_susy_13tev.root T5ZZ_entries.root TChiHZ_HToBB_ZToLL_entries.root TChiZZ_ZToLL_entries.root TChiWZ_ZToLL_entries.root jetCorrections proces
#cd $THISDIR

mkdir -p job_input
cd job_input
rm *
cp $COREPATH/libCMS3_CORE.so .
cp $DITTOPATH/bin/ditto_ana_mybaby .
cp ../run_mybaby.sh .
ln -s $COREPATH CORE
tar -czf package.tar.gz CORE/Tools/jetcorr/data/run2_25ns/ libCMS3_CORE.so ditto_ana_mybaby run_mybaby.sh
rm CORE
cd ../
cp run_mybaby.sh sweeproot.sh sweeproot_macro.C merge_script.sh merge_macro.C job_input/
