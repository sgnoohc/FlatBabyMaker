// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "CMS3Connector.h"

ObjUtil::Lepton getElectron(int idx)
{
  ObjUtil::Lepton lepton;
  float pt = cms3.els_p4().at(idx).pt();
  float eta = cms3.els_p4().at(idx).eta();
  float phi = cms3.els_p4().at(idx).phi();
  float mass = cms3.els_mass().at(idx);
  lepton.p4.SetPtEtaPhiM(pt, eta, phi, mass);
  lepton.etaSC = cms3.els_etaSC().at(idx);
  lepton.sigmaIEtaIEta_full5x5 = cms3.els_sigmaIEtaIEta_full5x5().at(idx);
  lepton.hOverE = cms3.els_hOverE().at(idx);
  lepton.dEtaIn = cms3.els_dEtaIn().at(idx);
  lepton.dPhiIn = cms3.els_dPhiIn().at(idx);
  lepton.epRatio = fabs( (1.0/cms3.els_ecalEnergy().at(idx)) - (cms3.els_eOverPIn().at(idx)/cms3.els_ecalEnergy().at(idx)) );
  lepton.nmiss = cms3.els_exp_innerlayers().at(idx);
  lepton.dxy = cms3.els_dxyPV().at(idx);
  lepton.dz = cms3.els_dzPV().at(idx);
  lepton.tightcharge = tightChargeEle(idx);
  lepton.convVeto = cms3.els_conv_vtx_flag().at(idx);
  lepton.ip3d = cms3.els_ip3d().at(idx);
  lepton.sip3d = fabs(cms3.els_ip3d().at(idx))/cms3.els_ip3derr().at(idx);
  lepton.mva = cms3.els_VIDNonTrigMvaValue().at(idx) ;
  // needed for isolation calculation
  const LorentzVector& lep_p4 = cms3.els_p4().at(idx);
  const LorentzVector& jet_p4 = closestJet(lep_p4, 0.4, 3.0, /*whichCorr=*/2);
  float closeJetPt = jet_p4.pt();
  lepton.ptRatio = ( closeJetPt>0. ? lep_p4.pt()/closeJetPt : 1.);
  lepton.ptRel = ptRel(lep_p4, jet_p4, true);
  lepton.relIso03 = eleRelIso03_noCorr(idx);
  lepton.relIso03DB = eleRelIso03DB(idx);
  lepton.relIso03EA = eleRelIso03EA(idx);
  lepton.miniRelIsoCMS3_EA = elMiniRelIsoCMS3_EA(idx, /*eaversion=*/1);
  lepton.miniRelIsoCMS3_DB = elMiniRelIsoCMS3_DB(idx);
  lepton.charge = cms3.els_charge().at(idx);
  lepton.pdgId = (-11)*lepton.charge;
  lepton.id = 0;
  return lepton;
}

ObjUtil::Leptons getElectrons()
{
  ObjUtil::Leptons leptons;
  for(unsigned int iEl = 0; iEl < cms3.els_p4().size(); iEl++)
  {
    ObjUtil::Lepton lepton = getElectron(iEl);
    leptons.push_back(lepton);
  }
  return leptons;
}

ObjUtil::Leptons getLeptons()
{
  ObjUtil::Leptons leptons;
  ObjUtil::Leptons electrons = getElectrons();
  leptons.insert(leptons.end(), electrons.begin(), electrons.end());
  std::sort(leptons.begin(), leptons.end(), comparator_pt<ObjUtil::Lepton>);
  return leptons;
}

ObjUtil::Jets getJets()
{
  ObjUtil::Jets jets;
  return jets;
}

ObjUtil::Jets getBJets()
{
  ObjUtil::Jets jets;
  return jets;
}

ObjUtil::METs getMETs()
{
  ObjUtil::METs mets;
  return mets;
}

ObjUtil::MET getMET()
{
  ObjUtil::MET met;
  return met;
}

ObjUtil::Truths getTruths()
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
