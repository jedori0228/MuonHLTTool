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


void Draw_ComparevsPU_2018_EachSuqeunce(){

  gStyle->SetOptStat();
  setTDRStyle();

  TString OutDir = "output/PUStudy/";
  gSystem->mkdir(OutDir,kTRUE);

  double x_2018[9];
  for(int i=0; i<9; i++) x_2018[i] = 0.5 + i;
  double y_2018[9] = {
2.53867942007,
2.03672176285,
0.103544321144,
0.242341873556,
1.10964661578,
0.206288911519,
1.09270564251,
1.94363445218,
1.09739578207,
  };

  TGraph *gr_2018_Iter = new TGraph(9, x_2018, y_2018);

  TCanvas *c = new TCanvas("c", "", 600, 600);
  canvas_margin(c);
  c->SetBottomMargin( 0.18 );
  //c->SetLogy();

  TH1D *hist_dummy = new TH1D("hist_dummy", "", 11, 0., 11.);
  hist_axis(hist_dummy);

  hist_dummy->GetXaxis()->SetTitleOffset(1.50);

  hist_dummy->GetYaxis()->SetTitle("Average CPU time [ms]");
  hist_dummy->GetXaxis()->SetTitle("#mu trigger sequence");
  hist_dummy->GetXaxis()->SetRangeUser(0,9);
  hist_dummy->Draw("histaxis");
  hist_dummy->GetYaxis()->SetRangeUser(0., 5.);
  hist_dummy->GetXaxis()->SetBinLabel(1, "Local pixel & strip reco.");
  hist_dummy->GetXaxis()->SetBinLabel(2, "OI");
  hist_dummy->GetXaxis()->SetBinLabel(3, "IO from L2 (quadruplet)");
  hist_dummy->GetXaxis()->SetBinLabel(4, "IO from L2 (triplet)");
  hist_dummy->GetXaxis()->SetBinLabel(5, "IO from L2 (doublet)");
  hist_dummy->GetXaxis()->SetBinLabel(6, "IO from L1 (quadruplet)");
  hist_dummy->GetXaxis()->SetBinLabel(7, "IO from L1 (triplet)");
  hist_dummy->GetXaxis()->SetBinLabel(8, "IO from L1 (doublet)");
  hist_dummy->GetXaxis()->SetBinLabel(9, "Merge");


  //==== 2018, Iter
  gr_2018_Iter->SetMarkerStyle(22);
  gr_2018_Iter->SetMarkerColor(kBlack);
  gr_2018_Iter->SetLineColor(kBlack);

  //latex_info.DrawLatex(0.20, 0.89, "L3 sequence");
  TLegend *lg = new TLegend(0.20, 0.67, 0.80, 0.87);
  lg->SetFillStyle(0);
  lg->SetBorderSize(0);
  lg->AddEntry( gr_2018_Iter, "2018, iterative (46 < pileup < 48)", "lp");
  lg->Draw();

  gr_2018_Iter->Draw("lpsame");

  c->SaveAs(OutDir+"/L1L2L3IsoFineBinning.pdf");
  c->Close();

}
