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

ObjUtil::Leptons getLeptons(CMS3& mytree);
ObjUtil::Leptons getElectrons(CMS3& mytree);
ObjUtil::Jets getJets(CMS3& mytree);
ObjUtil::Jets getBJets(CMS3& mytree);
ObjUtil::METs getMETs(CMS3& mytree);
ObjUtil::MET getMET(CMS3& mytree);
ObjUtil::Truths getTruths(CMS3& mytree);

#endif
//eof
