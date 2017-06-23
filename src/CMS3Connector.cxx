// vim: tabstop=4:softtabstop=4:shiftwidth=4:expandtab

//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "CMS3Connector.h"
#include "ElectronSelections.cc" // to get globalEleMVAreader

// global variables
FactorizedJetCorrector* jet_corrector = 0;
JetCorrectionUncertainty* jetcorr_uncertainty = 0;

ObjUtil::Lepton getElectron(int idx)
{
    // Create new object
    ObjUtil::Lepton lepton;

    // Set basic kinematics
    float pt   = cms3.els_p4().at(idx).pt();
    float eta  = cms3.els_p4().at(idx).eta();
    float phi  = cms3.els_p4().at(idx).phi();
    float mass = cms3.els_p4().at(idx).mass();
    lepton.p4.SetPtEtaPhiM(pt, eta, phi, mass);

    // Set electron properties
    lepton.elEtaSC                 = cms3.els_etaSC()                 .at(idx);
    lepton.elSigmaIEtaIEta_full5x5 = cms3.els_sigmaIEtaIEta_full5x5() .at(idx);
    lepton.elHOverE                = cms3.els_hOverE()                .at(idx);
    lepton.elDEtaIn                = cms3.els_dEtaIn()                .at(idx);
    lepton.elDPhiIn                = cms3.els_dPhiIn()                .at(idx);
    lepton.elNmiss                 = cms3.els_exp_innerlayers()       .at(idx);
    lepton.elMvaRaw                = cms3.els_VIDNonTrigMvaValue()    .at(idx);
    lepton.elConvVeto              = cms3.els_conv_vtx_flag()         .at(idx);
    lepton.elMva                   = globalEleMVAreader->MVA(idx);
    lepton.elEpRatio               = fabs( (1.0/cms3.els_ecalEnergy().at(idx)) - (cms3.els_eOverPIn().at(idx)/cms3.els_ecalEnergy().at(idx)) );
    lepton.tightcharge             = tightChargeEle(idx);

    // Set Lepton properties
    lepton.dxy    = cms3.els_dxyPV() .at(idx);
    lepton.dz     = cms3.els_dzPV()  .at(idx);
    lepton.ip3d   = cms3.els_ip3d()  .at(idx);
    lepton.charge = cms3.els_charge().at(idx);
    lepton.sip3d  = fabs(cms3.els_ip3d().at(idx))/cms3.els_ip3derr().at(idx);
    lepton.pdgId  = (-11)*lepton.charge;
    lepton.id = 0;
    lepton.isFromX = 0;
    if (!cms3.evt_isRealData())
    {
        if (isFromW        (11, idx)) lepton.isFromX |= (1<<0);
        if (isFromZ        (11, idx)) lepton.isFromX |= (1<<1);
        if (isFromB        (11, idx)) lepton.isFromX |= (1<<2);
        if (isFromC        (11, idx)) lepton.isFromX |= (1<<3);
        if (isFromLight    (11, idx)) lepton.isFromX |= (1<<4);
        if (isFromLightFake(11, idx)) lepton.isFromX |= (1<<5);
    }

    // needed for isolation calculation
    const LorentzVector& lep_p4 = cms3.els_p4().at(idx);
    const LorentzVector& jet_p4 = closestJet(lep_p4, 0.4, 3.0, /*whichCorr = */2);
    float closeJetPt            = jet_p4.pt();
    lepton.ptRatio             = (closeJetPt>0. ? lep_p4.pt()/closeJetPt : 1.);
    lepton.ptRel               = ptRel(lep_p4, jet_p4, true);
    lepton.relIso03            = eleRelIso03_noCorr(idx);
    lepton.relIso03DB          = eleRelIso03DB(idx);
    lepton.relIso03EA          = eleRelIso03EA(idx, /*eaversion=*/1);
    lepton.relIso03EAv2        = eleRelIso03EA(idx, /*eaversion=*/2);
    lepton.relIso04            = elRelIsoCustomCone(idx, 0.4, false, 0.0, /*useDBCorr=*/false);;
    lepton.relIso04DB          = elRelIsoCustomCone(idx, 0.4, false, 0.0, /*useDBCorr=*/true);;
    lepton.relIso04EA          = elRelIsoCustomCone(idx, 0.4, false, 0.0, /*useDBCorr=*/false, /*useEACorr=*/true, /*mindr=*/-1, /*eaversion=*/1);
    lepton.relIso04EAv2        = elRelIsoCustomCone(idx, 0.4, false, 0.0, /*useDBCorr=*/false, /*useEACorr=*/true, /*mindr=*/-1, /*eaversion=*/2);
    lepton.relIso005EAstudy    = elRelIsoCustomCone(idx, 0.05, false, 0.0, false, true, -1, 1);
    lepton.relIso010EAstudy    = elRelIsoCustomCone(idx, 0.10, false, 0.0, false, true, -1, 1);
    lepton.relIso015EAstudy    = elRelIsoCustomCone(idx, 0.15, false, 0.0, false, true, -1, 1);
    lepton.relIso020EAstudy    = elRelIsoCustomCone(idx, 0.20, false, 0.0, false, true, -1, 1);
    lepton.relIso025EAstudy    = elRelIsoCustomCone(idx, 0.25, false, 0.0, false, true, -1, 1);
    lepton.relIso030EAstudy    = elRelIsoCustomCone(idx, 0.30, false, 0.0, false, true, -1, 1);
    lepton.relIso035EAstudy    = elRelIsoCustomCone(idx, 0.35, false, 0.0, false, true, -1, 1);
    lepton.relIso040EAstudy    = elRelIsoCustomCone(idx, 0.40, false, 0.0, false, true, -1, 1);
    lepton.relIso045EAstudy    = elRelIsoCustomCone(idx, 0.45, false, 0.0, false, true, -1, 1);
    lepton.relIso050EAstudy    = elRelIsoCustomCone(idx, 0.50, false, 0.0, false, true, -1, 1);
    lepton.relIso005EAstudyv2  = elRelIsoCustomCone(idx, 0.05, true, 0.0, false, true, -1, 1);
    lepton.relIso010EAstudyv2  = elRelIsoCustomCone(idx, 0.10, true, 0.0, false, true, -1, 1);
    lepton.relIso015EAstudyv2  = elRelIsoCustomCone(idx, 0.15, true, 0.0, false, true, -1, 1);
    lepton.relIso020EAstudyv2  = elRelIsoCustomCone(idx, 0.20, true, 0.0, false, true, -1, 1);
    lepton.relIso025EAstudyv2  = elRelIsoCustomCone(idx, 0.25, true, 0.0, false, true, -1, 1);
    lepton.relIso030EAstudyv2  = elRelIsoCustomCone(idx, 0.30, true, 0.0, false, true, -1, 1);
    lepton.relIso035EAstudyv2  = elRelIsoCustomCone(idx, 0.35, true, 0.0, false, true, -1, 1);
    lepton.relIso040EAstudyv2  = elRelIsoCustomCone(idx, 0.40, true, 0.0, false, true, -1, 1);
    lepton.relIso045EAstudyv2  = elRelIsoCustomCone(idx, 0.45, true, 0.0, false, true, -1, 1);
    lepton.relIso050EAstudyv2  = elRelIsoCustomCone(idx, 0.50, true, 0.0, false, true, -1, 1);
    lepton.miniRelIsoCMS3_EA   = elMiniRelIsoCMS3_EA(idx, /*eaversion=*/1);
    lepton.miniRelIsoCMS3_EAv2 = elMiniRelIsoCMS3_EA(idx, /*eaversion=*/2);
    lepton.miniRelIsoCMS3_DB   = elMiniRelIsoCMS3_DB(idx);
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

ObjUtil::Lepton getMuon(int idx)
{
    // Create new object
    ObjUtil::Lepton lepton;

    // Set basic kinematics
    float pt   = cms3.mus_p4().at(idx).pt();
    float eta  = cms3.mus_p4().at(idx).eta();
    float phi  = cms3.mus_p4().at(idx).phi();
    float mass = cms3.mus_p4().at(idx).mass();
    lepton.p4.SetPtEtaPhiM(pt, eta, phi, mass);

    // Set muon properties
    lepton.muPOverP            = cms3.mus_ptErr().at(idx) / cms3.mus_trk_p4().at(idx).pt();
    lepton.muPidPFMuon         = cms3.mus_pid_PFMuon().at(idx);
    lepton.muType              = cms3.mus_type().at(idx);
    lepton.muChi2OverNDof      = cms3.mus_gfit_chi2().at(idx) / cms3.mus_gfit_ndof().at(idx);
    //	  lepton.muChi2OverNDof      = cms3.mus_gfit_chi2().at(idx) / cms3.mus_gfit_ndof_float().at(idx);
    lepton.muChi2LocalPosition = cms3.mus_chi2LocalPosition().at(idx);
    lepton.muTrkKink           = cms3.mus_trkKink().at(idx);
    lepton.muValidHitFraction  = cms3.mus_validHits().at(idx)/(double)(cms3.mus_validHits().at(idx)+cms3.mus_lostHits().at(idx)+cms3.mus_exp_innerlayers().at(idx)+cms3.mus_exp_outerlayers().at(idx));
    lepton.muSegmCompatibility = cms3.mus_segmCompatibility().at(idx);
    lepton.muGFitValidSTAHits  = cms3.mus_gfit_validSTAHits().at(idx);
    lepton.muNMatchedStations  = cms3.mus_numberOfMatchedStations().at(idx);
    lepton.muValidPixelHits    = cms3.mus_validPixelHits().at(idx);
    lepton.muNLayers           = cms3.mus_nlayers().at(idx);
    lepton.tightcharge         = tightChargeMuon(idx);

    // Set Lepton properties
    lepton.dxy    = cms3.mus_dxyPV().at(idx);
    lepton.dz     = cms3.mus_dzPV().at(idx);
    lepton.ip3d   = cms3.mus_ip3d().at(idx);
    lepton.charge = cms3.mus_charge().at(idx);
    lepton.sip3d  = fabs(cms3.mus_ip3d().at(idx))/cms3.mus_ip3derr().at(idx);
    lepton.pdgId  = (-13)*lepton.charge;
    lepton.id = 0;
    lepton.isFromX = 0;
    if (!cms3.evt_isRealData())
    {
        if (isFromW        (11, idx)) lepton.isFromX |= (1<<0);
        if (isFromZ        (11, idx)) lepton.isFromX |= (1<<1);
        if (isFromB        (11, idx)) lepton.isFromX |= (1<<2);
        if (isFromC        (11, idx)) lepton.isFromX |= (1<<3);
        if (isFromLight    (11, idx)) lepton.isFromX |= (1<<4);
        if (isFromLightFake(11, idx)) lepton.isFromX |= (1<<5);
    }

    // needed for isolation calculation
    const LorentzVector& lep_p4 = cms3.mus_p4().at(idx);
    const LorentzVector& jet_p4 = closestJet(lep_p4, 0.4, 3.0, /*whichCorr = */2);
    float closeJetPt            = jet_p4.pt();
    lepton.ptRatio             = (closeJetPt>0. ? lep_p4.pt()/closeJetPt : 1.);
    lepton.ptRel               = ptRel(lep_p4, jet_p4, true);
    lepton.relIso03            = muRelIso03_noCorr(idx);
    lepton.relIso03DB          = muRelIso03DB(idx);
    lepton.relIso03EA          = muRelIso03EA(idx,/*eaversion=*/1);
    lepton.relIso03EAv2        = muRelIso03EA(idx,/*eaversion=*/2);
    lepton.relIso04            = muRelIsoCustomCone(idx, 0.4, /*useVetoCones=*/false, 0.5, false);
    lepton.relIso04DB          = muRelIso04DB(idx);
    lepton.relIso04EA          = muRelIsoCustomCone(idx, 0.4, /*useVetoCones=*/false, 0.5, false, true, -1, 1);
    lepton.relIso04EAv2        = muRelIsoCustomCone(idx, 0.4, /*useVetoCones=*/false, 0.5, false, true, -1, 2);
    lepton.relIso005EAstudy    = muRelIsoCustomCone(idx, 0.05, true, 0.5, false, true, -1, 1);
    lepton.relIso010EAstudy    = muRelIsoCustomCone(idx, 0.10, true, 0.5, false, true, -1, 1);
    lepton.relIso015EAstudy    = muRelIsoCustomCone(idx, 0.15, true, 0.5, false, true, -1, 1);
    lepton.relIso020EAstudy    = muRelIsoCustomCone(idx, 0.20, true, 0.5, false, true, -1, 1);
    lepton.relIso025EAstudy    = muRelIsoCustomCone(idx, 0.25, true, 0.5, false, true, -1, 1);
    lepton.relIso030EAstudy    = muRelIsoCustomCone(idx, 0.30, true, 0.5, false, true, -1, 1);
    lepton.relIso035EAstudy    = muRelIsoCustomCone(idx, 0.35, true, 0.5, false, true, -1, 1);
    lepton.relIso040EAstudy    = muRelIsoCustomCone(idx, 0.40, true, 0.5, false, true, -1, 1);
    lepton.relIso045EAstudy    = muRelIsoCustomCone(idx, 0.45, true, 0.5, false, true, -1, 1);
    lepton.relIso050EAstudy    = muRelIsoCustomCone(idx, 0.50, true, 0.5, false, true, -1, 1);
    lepton.relIso005EAstudyv2  = muRelIsoCustomCone(idx, 0.05, true, 0.5, false, false, -1, 1);
    lepton.relIso010EAstudyv2  = muRelIsoCustomCone(idx, 0.10, true, 0.5, false, false, -1, 1);
    lepton.relIso015EAstudyv2  = muRelIsoCustomCone(idx, 0.15, true, 0.5, false, false, -1, 1);
    lepton.relIso020EAstudyv2  = muRelIsoCustomCone(idx, 0.20, true, 0.5, false, false, -1, 1);
    lepton.relIso025EAstudyv2  = muRelIsoCustomCone(idx, 0.25, true, 0.5, false, false, -1, 1);
    lepton.relIso030EAstudyv2  = muRelIsoCustomCone(idx, 0.30, true, 0.5, false, false, -1, 1);
    lepton.relIso035EAstudyv2  = muRelIsoCustomCone(idx, 0.35, true, 0.5, false, false, -1, 1);
    lepton.relIso040EAstudyv2  = muRelIsoCustomCone(idx, 0.40, true, 0.5, false, false, -1, 1);
    lepton.relIso045EAstudyv2  = muRelIsoCustomCone(idx, 0.45, true, 0.5, false, false, -1, 1);
    lepton.relIso050EAstudyv2  = muRelIsoCustomCone(idx, 0.50, true, 0.5, false, false, -1, 1);
    lepton.miniRelIsoCMS3_EA   = muMiniRelIsoCMS3_EA(idx, /*eaversion=*/1);
    lepton.miniRelIsoCMS3_EAv2 = muMiniRelIsoCMS3_EA(idx, /*eaversion=*/2);
    lepton.miniRelIsoCMS3_DB   = muMiniRelIsoCMS3_DB(idx);
    return lepton;
}

ObjUtil::Leptons getMuons()
{
    ObjUtil::Leptons leptons;
    for(unsigned int iMu = 0; iMu < cms3.mus_p4().size(); iMu++)
    {
        ObjUtil::Lepton lepton = getMuon(iMu);
        leptons.push_back(lepton);
    }
    return leptons;
}

ObjUtil::Leptons getLeptons()
{
    ObjUtil::Leptons leptons;
    ObjUtil::Leptons electrons = getElectrons();
    ObjUtil::Leptons muons = getMuons();
    leptons.insert(leptons.end(), electrons.begin(), electrons.end());
    leptons.insert(leptons.end(), muons.begin(), muons.end());
    std::sort(leptons.begin(), leptons.end(), comparator_pt<ObjUtil::Lepton>);
    return leptons;
}

ObjUtil::Jets getJets()
{
    ObjUtil::Jets jets;
    for (unsigned int iJet = 0; iJet < cms3.pfjets_p4().size(); iJet++)
    {

        ObjUtil::Jet jet;
        jet.p4.SetPxPyPzE(cms3.pfjets_p4().at(iJet).px(),cms3.pfjets_p4().at(iJet).py(),cms3.pfjets_p4().at(iJet).pz(),cms3.pfjets_p4().at(iJet).energy());

        // get L1FastL2L3Residual total correction
        jet_corrector->setRho   ( cms3.evt_fixgridfastjet_all_rho() );
        jet_corrector->setJetA  ( cms3.pfjets_area().at(iJet)       );
        jet_corrector->setJetPt ( jet.p4.Pt()               );
        jet_corrector->setJetEta( jet.p4.Eta()              );
        double corr = jet_corrector->getCorrection();

        // check for negative correction
        if (corr < 0. && fabs(jet.p4.Eta()) < 4.7) {
            std::cout << "ScanChain::Looper: WARNING: negative jet correction: " << corr
                << ", raw jet pt: " << jet.p4.Pt() << ", eta: " << jet.p4.Eta() << std::endl;
        }

        // include protections here on jet kinematics to prevent rare warnings/crashes
        double varUP = 1.;
        double varDN = 1.;
        if (!LoopUtil::isdata && jet.p4.Pt()*corr > 0. && fabs(jet.p4.Eta()) < 5.4) {
            jetcorr_uncertainty->setJetEta(jet.p4.Eta());
            jetcorr_uncertainty->setJetPt(jet.p4.Pt() * corr); // must use CORRECTED pt
            double unc = jetcorr_uncertainty->getUncertainty(true);
            varUP = (1. + unc);
            varDN = (1. - unc);
        }

        // apply new JEC to p4
        jet.jecCorr   = corr;
        jet.jecCorrUp = corr * varUP;
        jet.jecCorrDn = corr * varDN;
        jet.undoJEC = cms3.pfjets_undoJEC().at(iJet);

        // Charge hadron fraction
        jet.chf = cms3.pfjets_chargedHadronE().at(iJet) / (cms3.pfjets_undoJEC().at(iJet)*cms3.pfjets_p4().at(iJet).energy());
        jet.nhf = cms3.pfjets_neutralHadronE().at(iJet) / (cms3.pfjets_undoJEC().at(iJet)*cms3.pfjets_p4().at(iJet).energy());
        jet.cef = cms3.pfjets_chargedEmE().at(iJet) / (cms3.pfjets_undoJEC().at(iJet)*cms3.pfjets_p4().at(iJet).energy());
        jet.nef = cms3.pfjets_neutralEmE().at(iJet) / (cms3.pfjets_undoJEC().at(iJet)*cms3.pfjets_p4().at(iJet).energy());
        jet.muf = cms3.pfjets_muonE().at(iJet) / (cms3.pfjets_undoJEC().at(iJet)*cms3.pfjets_p4().at(iJet).energy());
        jet.cm = cms3.pfjets_chargedMultiplicity().at(iJet);
        jet.nm = cms3.pfjets_neutralMultiplicity().at(iJet);
        jet.puValue = cms3.pfjets_pileupJetId().at(iJet);
        jet.npfcand = cms3.pfjets_npfcands().at(iJet);
        if (!cms3.evt_isRealData())
        {
            if (LoopUtil::isfastsim)
                jet.mcdr = cms3.pfjets_mcdr().at(iJet);
            else
                jet.mcdr = -999;
        }

        // btag CSV
        jet.btagCSV = cms3.pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(iJet);

        if (!LoopUtil::isdata) {
            jet.mcPt          = -1;
            jet.mcFlavour     =  0;
            jet.hadronFlavour =  0;
            if (cms3.pfjets_mc_p4()        .size() > 0) jet.mcPt          = cms3.pfjets_mc_p4().at(iJet).pt();
            if (cms3.pfjets_partonFlavour().size() > 0) jet.mcFlavour     = cms3.pfjets_partonFlavour().at(iJet);
            if (cms3.pfjets_hadronFlavour().size() > 0) jet.hadronFlavour = cms3.pfjets_hadronFlavour().at(iJet);
        } else {
            jet.mcPt = -999.;
            jet.mcFlavour = -999;
            jet.hadronFlavour = -999;
        }

        //jet_qgl
        jet.area = cms3.pfjets_area().at(iJet);

        jet.puId      = loosePileupJetId(iJet) ? 1 : 0;
        //jet.puIdpuppi = loosePileupJetId_v2(iJet) ? 1 : 0;
        jet.puIdpuppi = -999;

        jet.id = 0;
        if (isLoosePFJet(iJet))              jet.id = jet.id | (1<<0);
        if (isMediumPFJet(iJet))             jet.id = jet.id | (1<<1);
        if (isTightPFJet(iJet))              jet.id = jet.id | (1<<2);
        if (isLoosePFJetV2(iJet))            jet.id = jet.id | (1<<3);
        if (isTightPFJetV2(iJet))            jet.id = jet.id | (1<<4);
        if (loosePileupJetId(iJet))          jet.id = jet.id | (1<<5);
        //if (loosePileupJetId_v2(iJet, true)) jet.id = jet.id | (1<<6);

        //veto for Fastsim events with crazy unmatched jets
        if (LoopUtil::isfastsim && jet.p4.Pt() > 20 && fabs(jet.p4.Eta()) < 2.5 && jet.chf < 0.1 && jet.mcPt == 0)
            jet.FSveto = 1;
        else
            jet.FSveto = 0;

        jets.push_back(jet);
    }

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

    // Compute PF MET and the two up/down variations
    std::pair <float, float> t1metUP;
    std::pair <float, float> t1metDN;
    std::pair <float, float> t1met;
    if (!LoopUtil::isdata) {
        t1met = getT1CHSMET_fromMINIAOD(jet_corrector, 0, 0);
        t1metUP = getT1CHSMET_fromMINIAOD(jet_corrector, jetcorr_uncertainty, 1);
        t1metDN = getT1CHSMET_fromMINIAOD(jet_corrector, jetcorr_uncertainty, 0);
    }
    else {
        t1met = getT1CHSMET_fromMINIAOD(jet_corrector); // never apply variations to data
        t1metUP = t1met;
        t1metDN = t1met;
    }

    // Set the PF MET
    ObjUtil::MET pfmet;
    pfmet.p4.      SetPtEtaPhiE(t1met.first, 0, t1met.second, t1met.first);
    pfmet.p4CorrUp.SetPtEtaPhiE(t1metUP.first, 0, t1metUP.second, t1metUP.first);
    pfmet.p4CorrDn.SetPtEtaPhiE(t1metDN.first, 0, t1metDN.second, t1metDN.first);

    //// hack for fastsim v1
    //ObjUtil::MET rawmet;
    //if (LoopUtil::isfastsim)
    //  rawmet.p4.SetPtEtaPhiM(cms3.evt_pfmet_raw(), 0, cms3.evt_pfmetPhi_raw(), 0);
    //else
    //  rawmet.p4.SetPtEtaPhiM(cms3.evt_METToolbox_pfmet_raw(), 0, cms3.evt_METToolbox_pfmetPhi_raw(), 0);

    //// Set true MET
    //ObjUtil::MET genmet;
    //if (!LoopUtil::isdata)
    //  genmet.p4.SetPtEtaPhiM(cms3.gen_met(), 0, cms3.gen_metPhi(), 0);

    //// Set calo MET
    //ObjUtil::MET calomet;
    //calomet.p4.SetPtEtaPhiM(cms3.evt_calomet(), 0, cms3.evt_calometPhi(), 0);

    //// Set trk MET
    //ObjUtil::MET trackmet;
    //metStruct trkmet = trackerMET(0.1);
    //trackmet.p4.SetPtEtaPhiM(trkmet.met, 0, trkmet.metphi, 0);

    mets.push_back(pfmet);
    //mets.push_back(rawmet);
    //mets.push_back(genmet);
    //mets.push_back(calomet);
    //mets.push_back(trackmet);

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
        bool is_mother_wz = (abs(motherId) == 24 || abs(motherId) == 23);

        if (!is_mother_wz)
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

ObjUtil::EventInfo getEventInfo()
{
    ObjUtil::EventInfo eventinfo;
    eventinfo.run = cms3.evt_run();
    eventinfo.lumi = cms3.evt_lumiBlock();
    eventinfo.event = cms3.evt_event();
    //  eventinfo.nEvts = cms3.evt_nEvts();
    if (!cms3.evt_isRealData())
        eventinfo.scale1fb = cms3.evt_scale1fb();
    else
        eventinfo.scale1fb = 1;
    eventinfo.pileup = cms3.evt_pileup();
    eventinfo.nvtx = cms3.evt_nvtxs();
    return eventinfo;
}

float gen_ht()
{
    float gen_ht = 0;
    for (unsigned int iGen = 0; iGen < cms3.genps_p4().size(); iGen++)
    {
        if ((abs(cms3.genps_id().at(iGen)) <  6 || abs(cms3.genps_id().at(iGen)) == 21) &&
            (cms3.genps_status().at(iGen) == 22 || cms3.genps_status().at(iGen)  == 23))
            gen_ht += cms3.genps_p4().at(iGen).pt();
    }
    return gen_ht;
}

void initJetCorrection()
{

    // The following code was taken from MT2 baby maker

    TString currentFileName = LoopUtil::getCurrentTFile()->GetName();

    std::vector<std::string> jetcorr_config_filenames;
    std::string jetcorr_uncertainty_filename;

    //// files for RunIISpring15 MC
    //int bx = 25;
    //if (bx == 50) {
    //  if (LoopUtil::isdata) {
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_DATA_L1FastJet_AK4PFchs.txt");
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_DATA_L2Relative_AK4PFchs.txt");
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_DATA_L3Absolute_AK4PFchs.txt");
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_DATA_L2L3Residual_AK4PFchs.txt");
    //  } else {
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_MC_L1FastJet_AK4PFchs.txt");
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_MC_L2Relative_AK4PFchs.txt");
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_MC_L3Absolute_AK4PFchs.txt");
    //    jetcorr_uncertainty_filename       = "CORE/Tools/jetcorr/data/run2_50ns/Summer15_50nsV4_DATA_Uncertainty_AK4PFchs.txt";
    //  }
    //}
    //else if (bx == 25) {
    //  if (LoopUtil::isdata) {
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV7_DATA_L1FastJet_AK4PFchs.txt");
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV7_DATA_L2Relative_AK4PFchs.txt");
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV7_DATA_L3Absolute_AK4PFchs.txt");
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV7_DATA_L2L3Residual_AK4PFchs.txt");
    //  } else if (LoopUtil::isfastsim) {
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/MCRUN2_74_V9_L1FastJet_AK4PFchs.txt");
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/MCRUN2_74_V9_L2Relative_AK4PFchs.txt");
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/MCRUN2_74_V9_L3Absolute_AK4PFchs.txt");
    //    jetcorr_uncertainty_filename =       "CORE/Tools/jetcorr/data/run2_25ns/MCRUN2_74_V9_Uncertainty_AK4PFchs.txt"; // not sure if these are correct..
    //  } else {
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_MC_L1FastJet_AK4PFchs.txt");
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_MC_L2Relative_AK4PFchs.txt");
    //    jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV2_MC_L3Absolute_AK4PFchs.txt");
    //    jetcorr_uncertainty_filename =       "CORE/Tools/jetcorr/data/run2_25ns/Summer15_25nsV7_DATA_Uncertainty_AK4PFchs.txt";
    //  }
    //}


    //if (LoopUtil::isdata)
    //{
    //  // run dependent corrections for 80X data
    //  if (currentFileName.Contains("2016B") || currentFileName.Contains("2016C") || currentFileName.Contains("2016D"))
    //  {
    //    jetcorr_config_filenames.clear();
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016BCDV4_DATA_L1FastJet_AK4PFchs.txt"   );
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016BCDV4_DATA_L2Relative_AK4PFchs.txt"  );
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016BCDV4_DATA_L3Absolute_AK4PFchs.txt"  );
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016BCDV4_DATA_L2L3Residual_AK4PFchs.txt");
    //  }
    //  else if (currentFileName.Contains("2016E") || currentFileName.Contains("2016F"))
    //  {
    //    jetcorr_config_filenames.clear();
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016EFV4_DATA_L1FastJet_AK4PFchs.txt"   );
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016EFV4_DATA_L2Relative_AK4PFchs.txt"  );
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016EFV4_DATA_L3Absolute_AK4PFchs.txt"  );
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016EFV4_DATA_L2L3Residual_AK4PFchs.txt");

    //    jetcorr_config_filenames_postrun278802.clear();
    //    jetcorr_config_filenames_postrun278802.push_back  ("jetCorrections/Summer16_23Sep2016GV4_DATA_L1FastJet_AK4PFchs.txt"   );
    //    jetcorr_config_filenames_postrun278802.push_back  ("jetCorrections/Summer16_23Sep2016GV4_DATA_L2Relative_AK4PFchs.txt"  );
    //    jetcorr_config_filenames_postrun278802.push_back  ("jetCorrections/Summer16_23Sep2016GV4_DATA_L3Absolute_AK4PFchs.txt"  );
    //    jetcorr_config_filenames_postrun278802.push_back  ("jetCorrections/Summer16_23Sep2016GV4_DATA_L2L3Residual_AK4PFchs.txt");
    //  }
    //  else if (currentFileName.Contains("2016G"))
    //  {
    //    jetcorr_config_filenames.clear();
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016GV4_DATA_L1FastJet_AK4PFchs.txt"   );
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016GV4_DATA_L2Relative_AK4PFchs.txt"  );
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016GV4_DATA_L3Absolute_AK4PFchs.txt"  );
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016GV4_DATA_L2L3Residual_AK4PFchs.txt");
    //  }
    //  else if (currentFileName.Contains("2016H"))
    //  {
    //    jetcorr_config_filenames.clear();
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016HV4_DATA_L1FastJet_AK4PFchs.txt"   );
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016HV4_DATA_L2Relative_AK4PFchs.txt"  );
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016HV4_DATA_L3Absolute_AK4PFchs.txt"  );
    //    jetcorr_config_filenames.push_back  ("jetCorrections/Summer16_23Sep2016HV4_DATA_L2L3Residual_AK4PFchs.txt");
    //  }
    //}
    //else if (LoopUtil::isfastsim)
    if (LoopUtil::isfastsim)
    {
        jetcorr_config_filenames.clear();
        jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_L1FastJet_AK4PFchs.txt");
        jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_L2Relative_AK4PFchs.txt");
        jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_L3Absolute_AK4PFchs.txt");
        jetcorr_uncertainty_filename =       "CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_Uncertainty_AK4PFchs.txt";
    }
    else if (currentFileName.Contains("Spring16"))
    { // Spring16 MC (ICHEP)
        jetcorr_config_filenames.clear();
        jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Spring16_25nsV6_MC_L1FastJet_AK4PFchs.txt");
        jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Spring16_25nsV6_MC_L2Relative_AK4PFchs.txt");
        jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Spring16_25nsV6_MC_L3Absolute_AK4PFchs.txt");
        jetcorr_uncertainty_filename =       "CORE/Tools/jetcorr/data/run2_25ns/Spring16_25nsV6_MC_Uncertainty_AK4PFchs.txt";
    }
    else
    { // default: Summer16 corrections (Moriond 2017)
        jetcorr_config_filenames.clear();
        jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer16_23Sep2016V3_MC/Summer16_23Sep2016V3_MC_L1FastJet_AK4PFchs.txt");
        jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer16_23Sep2016V3_MC/Summer16_23Sep2016V3_MC_L2Relative_AK4PFchs.txt");
        jetcorr_config_filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/Summer16_23Sep2016V3_MC/Summer16_23Sep2016V3_MC_L3Absolute_AK4PFchs.txt");
        jetcorr_uncertainty_filename =       "CORE/Tools/jetcorr/data/run2_25ns/Summer16_23Sep2016V3_MC/Summer16_23Sep2016V3_MC_Uncertainty_AK4PFchs.txt";
    }

    cout << "applying JEC from the following files:" << endl;
    for (unsigned int ifile = 0; ifile < jetcorr_config_filenames.size(); ++ifile) {
        cout << "   " << jetcorr_config_filenames.at(ifile) << endl;
    }

    jet_corrector  = makeJetCorrector(jetcorr_config_filenames);

    if (!LoopUtil::isdata) {
        cout << "applying JEC uncertainties from file: " << endl
            << "   " << jetcorr_uncertainty_filename << endl;
        jetcorr_uncertainty = new JetCorrectionUncertainty(jetcorr_uncertainty_filename);
    }

}

void initElectronMVA()
{
    createAndInitMVA("MVAinput", true, false, 80);
}

//eof
