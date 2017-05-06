// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#ifndef CMS3Looper_h
#define CMS3Looper_h

// ROOT
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TString.h>
#include <TTree.h>

// C++
#include <iostream>

// Ditto
#include "Ditto.h"
#include "CMS3.h"
#include "CMS3Connector.h"

using namespace Ditto;

//extern CMS3 mytree;
extern Analyses::AnalysisData ana_data;
extern TString output_name;

int CMS3Looper(TChain* chain, TString output_name, int nEvents);
void loop();
void beforeLoop(TChain* chain, TString output_name, int nevents);
void afterLoop();
void initCMS3();
void loadCMS3Event();
void processCMS3Event();
void getObjects();

#endif
//eof
