#include <stdio.h>
#include <stdlib.h>

// /////////////////////////////////////////////////// //
// RUN MACRO as $ root -l -x 'cerInfo.C("../wcsim.root")' //
// /////////////////////////////////////////////////// //

void cerInfo(char *filename=NULL) {

  gROOT->Reset();
  char* wcsimdirenv;
  wcsimdirenv = getenv ("WCSIMDIR");
  if(wcsimdirenv !=  NULL){
    gSystem->Load("${WCSIMDIR}/libWCSimRoot.so");
  }else{
    gSystem->Load("../libWCSimRoot.so");
  }
  gStyle->SetOptStat(1);

  TFile *f;
  if (filename==NULL){
    f = new TFile("${WCSIMDIR}/wcsim.root");
  }else{
    f = new TFile(filename);
  }
  if (!f->IsOpen()){
    cout << "Error, could not open input file: " << filename << endl;
    return -1;
  }

  TTree  *wcsimT = f->Get("wcsimT");
  WCSimRootEvent *wcsimrootsuperevent = new WCSimRootEvent();
  wcsimT->SetBranchAddress("wcsimrootevent",&wcsimrootsuperevent);
  // Force deletion to prevent memory leak when issuing multiple
  // calls to GetEvent()
  wcsimT->GetBranch("wcsimrootevent")->SetAutoDelete(kTRUE);

  TTree  *wcsimGeoT = f->Get("wcsimGeoT");
  WCSimRootGeom *wcsimrootgeom = new WCSimRootGeom();
  wcsimGeoT->SetBranchAddress("wcsimrootgeom",&wcsimrootgeom);


  // ///////////////////////// //
  // DECLARATION OF HISTOGRAMS //
  // ///////////////////////// //

  TH1D *hNumTubesHit = new TH1D("hNumTubesHit",
                                "Number of Tubes with a true hit",
				                        1000,0.,10000.);
  TH1D *hNumTubesDigiHit = new TH1D("hNumTubesDigiHit",
                                    "Number of Tubes with a true digi hit",
				                            1000,0.,10000.);
  TH1D *hSumQ = new TH1D("hSumQ",
                         "Total Charge by Evt",
			                   1000,0.,10000.);

  TH1D *hTotalPE = new TH1D("hTotalPE",
                         "Total PE",
                         1000,0.,10000.);
  hTotalPE->SetLineColor(kRed-4);

  TH1D *hCerHitTimes = new TH1D("hCerHitTimes",
                                "True Times of all hits",
     				                    1000,0,1000);

  // ///////////////////////// //
  // ------------------------- //
  // ///////////////////////// //

  // Number of particles simulated
  int NbOfEntries = wcsimT->GetEntries();

  // Number of evts with subevts
  int NbSubEvts = 0;

  // Loop on all entries
  for(int iEntry=0;iEntry<NbOfEntries;iEntry++){

    // Print
    if(iEntry % 1000 == 0) printf("iEntry #%d\n",iEntry);

    // Point through entry num ientry
    wcsimT->GetEvent(iEntry);

    // Does the evt has subevt ?
    if(wcsimrootsuperevent->HasSubEvents()) NbSubEvts++;

    // Loop on all triger inside an entry
    for(int iTrig=0;iTrig<wcsimrootsuperevent->GetNumberOfEvents();iTrig++){
      // Recover root info
      WCSimRootTrigger *wcsimrootevent = wcsimrootsuperevent->GetTrigger(0);

      // // Recover all digitized hits
      // int nCherenkovDigiHits = wcsimrootevent->GetNcherenkovdigihits();
      // // Loop on all digi hits
      // for(int iHits=0;iHits<nCherenkovDigiHits;iHits++){
      // 	WCSimRootCherenkovDigiHit *hit = (WCSimRootCherenkovDigiHit*)wcsimrootevent->GetCherenkovDigiHits()->At(iHits);
      // }

      hNumTubesHit->Fill(wcsimrootevent->GetNcherenkovhits());
      hNumTubesDigiHit->Fill(wcsimrootevent->GetNcherenkovdigihits());
      hSumQ->Fill(wcsimrootevent->GetSumQ());

      // Time of RAW hits
      TClonesArray *timeArray = wcsimrootevent->GetCherenkovHitTimes();

      int totalPe = 0;

      for (int i=0;i<wcsimrootevent->GetNcherenkovhits();i++){
        TObject *Hit = (wcsimrootevent->GetCherenkovHits())->At(i);
        WCSimRootCherenkovHit *wcsimrootcherenkovhit =
  	     dynamic_cast<WCSimRootCherenkovHit*>(Hit);

        int tubeNumber     = wcsimrootcherenkovhit->GetTubeID();
        int timeArrayIndex = wcsimrootcherenkovhit->GetTotalPe(0);
        int peForTube      = wcsimrootcherenkovhit->GetTotalPe(1);
        WCSimRootPMT pmt   = wcsimrootgeom->GetPMT(tubeNumber-1);
        totalPe += peForTube;

        for (int j = timeArrayIndex; j < timeArrayIndex + peForTube; j++){
      	  WCSimRootCherenkovHitTime HitTime =
      	    dynamic_cast<WCSimRootCherenkovHitTime>(timeArray->At(j));
            hCerHitTimes->Fill(HitTime.GetTruetime());
        }
      }

      hTotalPE->Fill(totalPe);

    } // END for iTrig

  } // END for iEntry

  // PRINTOUTS //
  printf("Nb of entries : %d\n",NbOfEntries);
  printf("Nb of evts with sub evts : %d (%f%%)\n",NbSubEvts,(double)(NbSubEvts)/(double)(NbOfEntries));

  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  c1->Divide(2,2);
  c1->cd(1); hNumTubesHit->Draw();
  c1->cd(2); hNumTubesDigiHit->Draw();
  c1->cd(3); hSumQ->Draw(); hTotalPE->Draw("same");
  c1->cd(4); hCerHitTimes->Draw();

  // f->Close();

  c1 = new TCanvas("c2","c2",800,600);
hSumQ->Draw();

  TFile *output=NULL;

  char outputName[1000];
  sprintf(outputName,"PROCESSED_%s",filename);

  output = new TFile(outputName,"recreate");
  hNumTubesHit->Write();
  hNumTubesDigiHit->Write();
  hSumQ->Write();
  hCerHitTimes->Write();
  hTotalPE->Write();

  // output->Close();

}
