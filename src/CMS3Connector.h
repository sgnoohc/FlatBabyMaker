// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#ifndef CMS3Connector_h
#define CMS3Connector_h

#include "Ditto.h"

// CORE
#include "CMS3.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"
#include "JetSelections.h"
#include "IsolationTools.h"
#include "MetSelections.h"
#include "Tools/JetCorrector.h"
#include "Tools/jetcorr/FactorizedJetCorrector.h"
#include "Tools/jetcorr/JetCorrectionUncertainty.h"

using namespace Ditto;

extern FactorizedJetCorrector* jet_corrector;
extern JetCorrectionUncertainty* jetcorr_uncertainty;

// CMS3 to Ditto
ObjUtil::Lepton getElectron(int idx);
ObjUtil::Leptons getElectrons();
ObjUtil::Lepton getMuon(int idx);
ObjUtil::Leptons getMuons();
//ObjUtil::Lepton getMuon(int idx);
//ObjUtil::Jet getJet(int idx);

ObjUtil::Leptons getLeptons();
ObjUtil::Jets getJets();
ObjUtil::Jets getBJets();
ObjUtil::METs getMETs();
ObjUtil::MET getMET();
ObjUtil::Truths getTruths();

void initJetCorrection();

#endif
//eof
