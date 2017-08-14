#include <stdio.h>
#include <stdlib.h>

// TH1D *readHist(char *filename){
//   TFile *f = new TFile(filename);
//
//   TH1D *histReturn;
//   histReturn[0] = (TH1D*)f->Get(hNumTubesHit);
//   histReturn[1] = (TH1D*)f->Get(hNumTubesDigiHit);
//   histReturn[2] = (TH1D*)f->Get(hSumQ);
//   histReturn[3] = (TH1D*)f->Get(hCerHitTimes);
//
//   return histReturn;
// }

void compareProcessed(){

  // TH1D *hNumTubesHit = new TH1D("hNumTubesHit",
  //                               "Number of Tubes with a true hit",
	// 			                        1000,0,10000);
  // TH1D *hNumTubesDigiHit = new TH1D("hNumTubesDigiHit",
  //                                   "Number of Tubes with a true digi hit",
	// 			                            1000,0,10000);
  // TH1D *hSumQ = new TH1D("hSumQ",
  //                        "Total Charge by Evt",
	// 		                   1000,0.,10000.);
  //
  // TH1D *hCerHitTimes = new TH1D("hCerHitTimes",
  //                               "True Times of all hits",
  //    				                    1000,0,1000);

  char legName[1000] = "9.4.4-10.1.3";

  TFile *f1 = new TFile("piminusallplotsbinary.root");

  TH1D *f1h1 = (TH1D*)f1->Get("hNumTubesHit");
  TH1D *f1h2 = (TH1D*)f1->Get("hNumTubesDigiHit");
  TH1D *f1h3 = (TH1D*)f1->Get("hSumQ");
  TH1D *f1h4 = (TH1D*)f1->Get("hCerHitTimes");
  f1h1->SetLineColor(kBlue-4);
  f1h2->SetLineColor(kBlue-4);
  f1h3->SetLineColor(kBlue-4);
  f1h4->SetLineColor(kBlue-4);

  // TH1D *f1h5 = (TH1D*)f1->Get("hTotalPE");
  // f1h5->SetLineColor(kBlue-4);

  TFile *f2 = new TFile("piminusallplotsbertini.root");

  TH1D *f2h1 = (TH1D*)f2->Get("hNumTubesHit");
  TH1D *f2h2 = (TH1D*)f2->Get("hNumTubesDigiHit");
  TH1D *f2h3 = (TH1D*)f2->Get("hSumQ");
  TH1D *f2h4 = (TH1D*)f2->Get("hCerHitTimes");
  f2h1->SetLineColor(kRed-4);
  f2h2->SetLineColor(kRed-4);
  f2h3->SetLineColor(kRed-4);
  f2h4->SetLineColor(kRed-4);

  // TH1D *f2h5 = (TH1D*)f2->Get("hTotalPE");
  // f2h5->SetLineColor(kRed-4);

  f1h1->GetXaxis()->SetRangeUser(0,3000);
  f1h2->GetXaxis()->SetRangeUser(0,2000);
  f1h3->GetXaxis()->SetRangeUser(0,4000);
  // f1h4->GetXaxis()->SetRangeUser(1000,5000);
  f2h1->GetXaxis()->SetRangeUser(0,3000);
  f2h2->GetXaxis()->SetRangeUser(0,2000);
  f2h3->GetXaxis()->SetRangeUser(0,4000);
  // f2h4->GetXaxis()->SetRangeUser(1000,5000);

  f1h1->GetYaxis()->SetRangeUser(0,f2h1->GetMaximum());
  f1h2->GetYaxis()->SetRangeUser(0,f2h2->GetMaximum());
  f1h3->GetYaxis()->SetRangeUser(0,f2h3->GetMaximum());


  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  c1->Divide(2,2);
  c1->cd(1); f1h1->Draw(""); f2h1->Draw("same");
  c1->cd(2); f1h2->Draw(""); f2h2->Draw("same");
  c1->cd(3); f1h3->Draw(""); f2h3->Draw("same");
  c1->cd(4); f1h4->Draw(""); f2h4->Draw("same");

  // Define the diff plot
  f1h1->Sumw2();
  f2h1->Sumw2();
  f1h2->Sumw2();
  f2h2->Sumw2();
  f1h3->Sumw2();
  f2h3->Sumw2();
  f1h4->Sumw2();
  f2h4->Sumw2();

  TH1D *h1 = (TH1D*)f1h1->Clone("h1");
  h1->Add(f2h1,-1);
  h1->Divide(f1h1);
  TH1D *h2 = (TH1D*)f1h2->Clone("h2");
  h2->Add(f2h2,-1);
  h2->Divide(f1h2);
  TH1D *h3 = (TH1D*)f1h3->Clone("h3");
  h3->Add(f2h3,-1);
  h3->Divide(f1h3);
  TH1D *h4 = (TH1D*)f1h4->Clone("h4");
  h4->Add(f2h4,-1);
  h4->Divide(f1h4);

  TCanvas *c1c1 = new TCanvas("c2","c2",900,720);

  c1c1->Divide(2,4);

  ///////////////////////////////////////
  //  TOP PART : Nb HITS
  ///////////////////////////////////////

  c1c1->cd(1);
  f1h1->Draw("HIST");
  f2h1->Draw("HISTsame");
  f1h1->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  f1h1->GetYaxis()->SetLabelSize(10);
  f1h1->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  f1h1->GetXaxis()->SetLabelSize(10);
  c1c1->cd(1)->SetPad(0.01, 0.65, 0.50, 0.99);
  c1c1->cd(3);
  c1c1->cd(3)->SetPad(0.01, 0.50, 0.50, 0.67);
  h1->Draw("ep");
  h1->SetTitle();
  h1->SetStats(0);      // No statistics on lower plot
  h1->GetYaxis()->SetTitle(legName);
  h1->GetYaxis()->SetTitleSize(15);
  h1->GetYaxis()->SetTitleFont(43);
  h1->GetYaxis()->SetTitleOffset(1.60);
  h1->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  h1->GetYaxis()->SetLabelSize(10);

  c1c1->cd(2);
  f1h2->Draw("HIST");
  f2h2->Draw("HISTsame");
  f1h2->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  f1h2->GetYaxis()->SetLabelSize(10);
  f1h2->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  f1h2->GetXaxis()->SetLabelSize(10);
  c1c1->cd(2)->SetPad(0.50, 0.65, 0.99, 0.99);
  c1c1->cd(4);
  c1c1->cd(4)->SetPad(0.50, 0.50, 0.99, 0.67);
  h2->Draw();
  h2->SetTitle();
  h2->SetStats(0);      // No statistics on lower plot
  h2->GetYaxis()->SetTitle(legName);
  h2->GetYaxis()->SetTitleSize(15);
  h2->GetYaxis()->SetTitleFont(43);
  h2->GetYaxis()->SetTitleOffset(1.60);
  h2->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  h2->GetYaxis()->SetLabelSize(10);

  ///////////////////////////////////////
  //  BOTTOM PART : Q and TIME
  ///////////////////////////////////////

  c1c1->cd(5);
  f1h3->Draw("HIST");
  f2h3->Draw("HISTsame");
  f1h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  f1h3->GetYaxis()->SetLabelSize(10);
  f1h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  f1h3->GetXaxis()->SetLabelSize(10);
  c1c1->cd(5)->SetPad(0.01, 0.13, 0.50, 0.50);
  c1c1->cd(7);
  c1c1->cd(7)->SetPad(0.01, 0.01, 0.50, 0.15);
  h3->Draw();
  h3->SetTitle();
  h3->SetStats(0);      // No statistics on lower plot
  h3->GetYaxis()->SetTitle(legName);
  h3->GetYaxis()->SetTitleSize(15);
  h3->GetYaxis()->SetTitleFont(43);
  h3->GetYaxis()->SetTitleOffset(1.50);
  h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  h3->GetYaxis()->SetLabelSize(10);

  c1c1->cd(6);
  f1h4->Draw("HIST");
  f2h4->Draw("HISTsame");
  f1h4->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  f1h4->GetYaxis()->SetLabelSize(10);
  f1h4->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  f1h4->GetXaxis()->SetLabelSize(10);
  c1c1->cd(6)->SetPad(0.50, 0.13, 0.99, 0.50);
  c1c1->cd(8);
  c1c1->cd(8)->SetPad(0.50, 0.01, 0.99, 0.15);
  h4->Draw();
  h4->SetTitle();
  h4->SetStats(0);      // No statistics on lower plot
  h4->GetYaxis()->SetTitle(legName);
  h4->GetYaxis()->SetTitleSize(15);
  h4->GetYaxis()->SetTitleFont(43);
  h4->GetYaxis()->SetTitleOffset(1.70);
  h4->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  h4->GetYaxis()->SetLabelSize(10);

}
