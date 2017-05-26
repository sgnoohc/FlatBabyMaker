// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "CMS3Looper.h"

//CMS3 mytree;
ObjUtil::AnalysisData ana_data;
TString output_name;
TFile* baby_output_file;
TTree* baby_output_tree;

//______________________________________________________________________________________
int CMS3Looper(TChain* chain, TString output_name, int nevents)
{

  // Before the loop set up some configuration, histograms and etc.
  beforeLoop(chain, output_name, nevents);

  // During the event loop process each event one by one
  loop();

  // After the loop is done, save histograms and etc.
  afterLoop();

  return 0;
}

//______________________________________________________________________________________
void beforeLoop(TChain* chain, TString output_name_, int nevents)
{

  // Fun start ASCII art
  PrintUtil::start();

  // Initialize configurations for event looping
  LoopUtil::resetLoopCondition(chain, nevents);

  output_name = output_name_;

  baby_output_file = new TFile(output_name+"_tree.root", "recreate");
  baby_output_tree = new TTree("MyBaby", "MyBaby");

  TreeUtil::createTruthBranch(baby_output_tree, "truth");
  TreeUtil::createLeptonBranch(baby_output_tree, "lep");
  TreeUtil::createJetBranch(baby_output_tree, "jet");
  TreeUtil::createMETBranch(baby_output_tree, "met");
  TreeUtil::createEventInfoBranch(baby_output_tree, "evt");
  TreeUtil::createFloatBranch(baby_output_tree, "gen_ht");

  // Print progress bar
  LoopUtil::doprintprogressbar = true;

}


//______________________________________________________________________________________
void loop()
{

  // Loop over file:ttree -> and loop over events in ttree
  // (usually I only have one file each.. so kinda redundant)
  while (LoopUtil::nextFileAndLoadTTreeWithName("Events"))
  {

    initCMS3();
    initJetCorrection();
    initElectronMVA();

    // Loop over the TTree
    while (LoopUtil::nextEvent())
    {

      if (LoopUtil::isAllEventsProcessed()) break;

      loadCMS3Event();

      TreeUtil::initTreeData();

      //=================================================================
      // <3 of the code
      // Do whatever you want to in the following function for each event
      //=================================================================
      processCMS3Event();


    } // End TTree loop

  } // End Loop over files
}

//______________________________________________________________________________________
void initCMS3()
{
  // Init the Class
  cms3.Init(LoopUtil::getCurrentTTree());
}

//______________________________________________________________________________________
void loadCMS3Event()
{
  //cms3.LoadTree(LoopUtil::getCurrentTTreeEventIndex());
  cms3.GetEntry(LoopUtil::getCurrentTTreeEventIndex());
}

//______________________________________________________________________________________
void processCMS3Event()
{

  /// Set objects
  getObjects();

  /// Fill histograms
  // HistUtil::fillStdHistograms("", ana_data);

  TreeUtil::setTruths(ana_data, "truth");
  TreeUtil::setLeptons(ana_data, "lep");
  TreeUtil::setJets(ana_data, "jet");
  TreeUtil::setMET(ana_data, "met");
  TreeUtil::setEventInfo(ana_data, "evt");
  TreeUtil::setFloatBranch("gen_ht", gen_ht());

  baby_output_tree->Fill();
}

//______________________________________________________________________________________
void getObjects()
{
  /// Get objects
  ana_data.leptons   = getLeptons();
  ana_data.jets      = getJets();
  ana_data.met       = getMETs().at(0);
  ana_data.eventinfo = getEventInfo();
  ana_data.wgt       = cms3.evt_scale1fb();
  ana_data.truths    = getTruths();
}

//______________________________________________________________________________________
void afterLoop()
{

  baby_output_file->cd();
  baby_output_tree->Write();

  // Save histograms
  PlotUtil::savePlots(ana_data.hist_db, (output_name+"_hist.root").Data());

  // Fun exit
  PrintUtil::exit();
}

//______________________________________________________________________________________
bool isdata()
{
  return cms3.evt_isRealData();
}
//eof
