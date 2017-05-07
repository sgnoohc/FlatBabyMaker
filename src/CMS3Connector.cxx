// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "CMS3Connector.h"

ObjUtil::Leptons getLeptons(CMS3& mytree)
{
  ObjUtil::Leptons leptons;
  return leptons;
}

ObjUtil::Leptons getElectrons(CMS3& mytree)
{
  for(unsigned int iEl = 0; iEl < cms3.els_p4().size(); iEl++)
  {
    if (cms3.els_p4().at(iEl).pt() < 5.0) continue;
    if (fabs(cms3.els_p4().at(iEl).eta()) > 2.4) continue;
    if (!electronID(iEl,id_level_t::HAD_veto_noiso_v4)) continue;
    bool pass_iso = electronID(iEl,id_level_t::HAD_veto_v4);
    bool pass_isoLoose = elMiniRelIsoCMS3_EA(iEl,1) < 0.5;
    if (!pass_isoLoose) continue;
  }
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

ObjUtil::Truths getTruths(CMS3& mytree)
{

  ObjUtil::Truths truths;

  for(unsigned int iGen = 0; iGen < cms3.genps_p4().size(); iGen++)
  {

    ObjUtil::Truth truth;

    //genPart_pt[ngenPart]            = cms3.genps_p4().at(iGen).pt();
    //genPart_eta[ngenPart]           = cms3.genps_p4().at(iGen).eta();
    //genPart_phi[ngenPart]           = cms3.genps_p4().at(iGen).phi();
    //genPart_mass[ngenPart]          = cms3.genps_mass().at(iGen);
    //genPart_pdgId[ngenPart]         = cms3.genps_id().at(iGen);
    //genPart_status[ngenPart]        = cms3.genps_status().at(iGen);
    //genPart_charge[ngenPart]        = cms3.genps_charge().at(iGen);
    //genPart_motherId[ngenPart]      = cms3.genps_id_simplemother().at(iGen);
    //genPart_grandmotherId[ngenPart] = cms3.genps_id_simplegrandma().at(iGen);

    float pt      = cms3.genps_p4().at(iGen).pt();
    float eta     = cms3.genps_p4().at(iGen).eta();
    float phi     = cms3.genps_p4().at(iGen).phi();
    float mass    = cms3.genps_mass().at(iGen);
    int pdgId     = cms3.genps_id().at(iGen);
    int status    = cms3.genps_status().at(iGen);
    int motherId  = cms3.genps_id_simplemother().at(iGen);
    int grandmaId = cms3.genps_id_simplegrandma().at(iGen);

    bool is_good_status = (status == 21 || status == 22 || status == 23 || status == 1 || status == 2);
    bool is_hardscatter_related = (iGen >= 4 && iGen <= 7) || (abs(motherId) > 1000000) || (abs(motherId) == 23) || (abs(motherId) == 24);

    if ( !(is_good_status && is_hardscatter_related) )
      continue;

    truth.p4.SetPtEtaPhiM(pt, eta, phi, mass);
    truth.pdgId = pdgId;
    truth.status = status;
    truth.motherId = motherId;
    truth.grandmotherId = grandmaId;

    truths.push_back(truth);

  }

  return truths;

}

//eof
