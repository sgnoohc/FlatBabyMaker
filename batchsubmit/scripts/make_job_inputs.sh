#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "Creating a directory where the binaries and jobs to run are aggregated."
echo "Creating $DIR/../__package"
mkdir -p $DIR/../__package
cd $DIR/../__package
rm -f *
cp $COREPATH/libCMS3_CORE.so .
cp $DITTOPATH/bin/ditto_ana_mybaby .
cp $DIR/../scripts/run_baby_run.sh .
ln -s $COREPATH CORE
tar -czf package.tar.gz CORE/Tools/jetcorr/data/run2_25ns/ libCMS3_CORE.so ditto_ana_mybaby run_baby_run.sh
rm CORE
rm libCMS3_CORE.so ditto_ana_mybaby
cp $DIR/../scripts/sweeproot.sh $DIR/../scripts/sweeproot_macro.C $DIR/../scripts/merge_script.sh $DIR/../scripts/merge_macro.C .

echo "Creatied $DIR/../__package"
ls -l $DIR/../__package

echo ""
echo ""
echo "================================================================"
echo "Done creating __package for condor submission."
echo "Now copy the runtemplate to another directory and submit in there."
echo "================================================================"
echo ""
