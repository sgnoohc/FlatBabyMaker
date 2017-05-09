// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#ifndef CMS3Connector_h
#define CMS3Connector_h

#include "Ditto.h"
#include "CMS3.h"
#include "ElectronSelections.h"
#include "IsolationTools.h"

using namespace Ditto;

// CMS3 to Ditto
ObjUtil::Lepton getElectron(int idx);
ObjUtil::Leptons getElectrons();
//ObjUtil::Lepton getMuon(int idx);
//ObjUtil::Jet getJet(int idx);

ObjUtil::Leptons getLeptons();
ObjUtil::Jets getJets();
ObjUtil::Jets getBJets();
ObjUtil::METs getMETs();
ObjUtil::MET getMET();
ObjUtil::Truths getTruths();

#endif
//eof
