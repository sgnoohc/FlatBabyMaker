// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "CMS3Connector.h"

ObjUtil::Leptons getLeptons(CMS3& mytree)
{
  ObjUtil::Leptons leptons;
  return leptons;
}

ObjUtil::Jets getJets(CMS3& mytree)
{
  ObjUtil::Jets jets;
  return jets;
}

ObjUtil::Jets getBJets(CMS3& mytree)
{
  ObjUtil::Jets jets;
  return jets;
}

ObjUtil::METs getMETs(CMS3& mytree)
{
  ObjUtil::METs mets;
  return mets;
}

ObjUtil::MET getMET(CMS3& mytree)
{
  ObjUtil::MET met;
  return met;
}

//eof

// MadGraph lhe output
// ------------------------------------------------------------------
//  for (unsigned int ip = 0; ip < mytree.P_X->size(); ++ip)
//  {
//    ObjUtil::Jet jet;
//    float px     = mytree.P_X -> at(ip);
//    float py     = mytree.P_Y -> at(ip);
//    float pz     = mytree.P_Z -> at(ip);
//    float energy = mytree.E   -> at(ip);
//    int   pdgId  = mytree.PID -> at(ip);
//    if (abs(pdgId) <= 5 || abs(pdgId) == 21)
//    {
//      jet.id = pdgId;
//      jet.p4.SetXYZT(px, py, pz, energy);
//      jets.push_back(jet);
//    }
//  }
//  std::sort(jets.begin(), jets.end(), comparator_pt<ObjUtil::Jet>);
// ------------------------------------------------------------------
