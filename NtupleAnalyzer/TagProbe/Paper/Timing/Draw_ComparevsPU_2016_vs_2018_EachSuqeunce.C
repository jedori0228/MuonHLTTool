#include "canvas_margin.h"
#include "mylib.h"


TString Precision(double a, int ndigit){

  int RaiseDigit = int(a*pow(10,ndigit+1))%10;
  bool Raise = RaiseDigit%10 >= 5 ? true : false;

  int temp = int( a*pow(10,ndigit) );
  if(Raise) temp += 1;
  double out = 1.*temp/pow(10,ndigit);

  TString str_int = TString::Itoa(int(out),10);

  int int_frac = temp % int(pow(10,ndigit));
  TString str_frac = TString::Itoa(int_frac,10);

  //cout << "[Precision] int_frac = " << int_frac << endl;
  //cout << "[Precision] str_frac = " << str_frac << endl;
  for(int i=1; i<=ndigit-1; i++){
    if(int_frac<pow(10,i)){
      for(int j=0; j<i; j++){
        //cout << "[Precision]  i = " << i << endl;
        str_frac = "0"+str_frac;
      }
      break;
    }
  }
  return str_int+"."+str_frac;


}


void Draw_ComparevsPU_2016_vs_2018_EachSuqeunce(){

  gStyle->SetOptStat();
  setTDRStyle();

  TString OutDir = "output/PUStudy/";
  gSystem->mkdir(OutDir,kTRUE);

  double x[3] = {
    0.5, 1.5, 2.5,
  };

  double y_2016_Cascade[3] = {
0.924856805825,
0.830029152261,
0.334239295615,
  };
  double y_2016_Tracker[3] = {
0,
2.10992835047,
0.294810367335,
  };


  double y_2016_Sum[3] = {
0.924856805825,
1.85464700885,
0.380858358511,
  };

  //double y_2016_Sum[3];
  //for(int i=0; i<3; i++) y_2016_Sum[i] = y_2016_Cascade[i]+y_2016_Tracker[i];

  double y_2018_Iter[3] = {
0.94802518317,
10.3732302036,
1.02863800582,
  };

  double y_2018_IterNoDoublet[3] = {
0.962889545597,
6.99539826348,
1.0304400072,
  };

  TGraph *gr_2016_Cascade = new TGraph(3, x, y_2016_Cascade);
  TGraph *gr_2016_Tracker = new TGraph(3, x, y_2016_Tracker);
  TGraph *gr_2016_Sum = new TGraph(3, x, y_2016_Sum);
  TGraph *gr_2018_Iter = new TGraph(3, x, y_2018_Iter);
  TGraph *gr_2018_IterNoDoublet = new TGraph(3, x, y_2018_IterNoDoublet);

  TCanvas *c = new TCanvas("c", "", 600, 600);
  canvas_margin(c);
  //c->SetLogy();

  TH1D *hist_dummy = new TH1D("hist_dummy", "", 10, 0., 10.);
  hist_axis(hist_dummy);
  hist_dummy->GetYaxis()->SetTitle("Average CPU time [ms]");
  hist_dummy->GetXaxis()->SetTitle("#mu trigger step");
  hist_dummy->GetXaxis()->SetRangeUser(0,3);
  hist_dummy->Draw("histaxis");
  hist_dummy->GetYaxis()->SetRangeUser(0., 20.);
  hist_dummy->GetXaxis()->SetBinLabel(1, "L2");
  hist_dummy->GetXaxis()->SetBinLabel(2, "L3");
  hist_dummy->GetXaxis()->SetBinLabel(3, "Isolation");

  //=== 2016, Cascade
  gr_2016_Cascade->SetMarkerStyle(20);
  gr_2016_Cascade->SetMarkerColor(kGreen);
  gr_2016_Cascade->SetLineColor(kGreen);
  //=== 2016, Tracker
  gr_2016_Tracker->SetMarkerStyle(21);
  gr_2016_Tracker->SetMarkerColor(kRed);
  gr_2016_Tracker->SetLineColor(kRed);

  //=== 2016, Sum
  gr_2016_Sum->SetMarkerStyle(20);
  gr_2016_Sum->SetMarkerColor(kBlue);
  gr_2016_Sum->SetLineColor(kBlue);
  //==== 2018, Iter
  gr_2018_Iter->SetMarkerStyle(22);
  gr_2018_Iter->SetMarkerColor(kBlack);
  gr_2018_Iter->SetLineColor(kBlack);
  //==== 2018, IterNoDoublet
  gr_2018_IterNoDoublet->SetMarkerStyle(25);
  gr_2018_IterNoDoublet->SetMarkerColor(kViolet);
  gr_2018_IterNoDoublet->SetLineColor(kViolet);

  //latex_info.DrawLatex(0.20, 0.89, "L3 sequence");
  TLegend *lg = new TLegend(0.20, 0.67, 0.90, 0.87);
  lg->SetFillStyle(0);
  lg->SetBorderSize(0);

  //lg->AddEntry( gr_2016_Cascade,  "2016, cascade (45 < pileup < 50)", "lp");
  //lg->AddEntry( gr_2016_Tracker,  "2016, tracker muon (45 < pileup < 50)", "lp");
  lg->AddEntry( gr_2016_Sum, "2016, cascade and tracker muon (45 < pileup < 50)", "lp");
  lg->AddEntry( gr_2018_Iter, "2018, iterative (46 < pileup < 48)", "lp");
  lg->AddEntry( gr_2018_IterNoDoublet, "2018, iterative w/o doublet iteration (46 < pileup < 48)", "lp");
  lg->Draw();

  //gr_2016_Cascade->Draw("lpsame");
  //gr_2016_Tracker->Draw("lpsame");
  gr_2016_Sum->Draw("lpsame");
  gr_2018_Iter->Draw("lpsame");
  gr_2018_IterNoDoublet->Draw("lpsame");

  c->SaveAs(OutDir+"/L1L2L3Iso.pdf");
  c->Close();

}
