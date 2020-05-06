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
    //0.5, 1.5, 2.5,
    1., 2., 3.,
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
0.924856805825+1.85464700885,
0.924856805825+1.85464700885+0.380858358511,
  };
//==== change it so cumulative


  //double y_2016_Sum[3];
  //for(int i=0; i<3; i++) y_2016_Sum[i] = y_2016_Cascade[i]+y_2016_Tracker[i];

  double y_2018_Iter[3] = {
0.94802518317,
0.94802518317+10.3732302036,
0.94802518317+10.3732302036+1.02863800582,
  };

  double y_2018_IterNoDoublet[3] = {
0.962889545597,
0.962889545597+6.99539826348,
0.962889545597+6.99539826348+1.0304400072,
  };

  TGraph *gr_2016_Sum = new TGraph(3, x, y_2016_Sum);
  TGraph *gr_2018_Iter = new TGraph(3, x, y_2018_Iter);
  TGraph *gr_2018_IterNoDoublet = new TGraph(3, x, y_2018_IterNoDoublet);

  TCanvas *c = new TCanvas("c", "", 600, 600);
  canvas_margin(c);
  //c->SetLogy();

  TH1D *hist_dummy = new TH1D("hist_dummy", "", 10, 0., 10.);
  hist_axis(hist_dummy);
  hist_dummy->GetYaxis()->SetTitle("Cumulative CPU time [ms]");
  hist_dummy->GetXaxis()->SetTitle("#mu trigger step");
  hist_dummy->GetXaxis()->SetRangeUser(0,3);
  hist_dummy->Draw("histaxis");
  hist_dummy->GetYaxis()->SetRangeUser(0., 20.);
  hist_dummy->GetXaxis()->SetBinLabel(1, "L2");
  hist_dummy->GetXaxis()->SetBinLabel(2, "L3");
  hist_dummy->GetXaxis()->SetBinLabel(3, "Isolation");

  TLatex latex_year;
  latex_year.SetNDC();
  latex_year.SetTextSize(0.035);
  latex_year.SetTextFont(42);
  latex_year.DrawLatex(0.61, 0.96, "2016 and 2018, 13 TeV");

  TLatex latex_CMSPriliminary;
  latex_CMSPriliminary.DrawLatexNDC(0.16, 0.96, "#font[62]{CMS}#font[42]{#it{#scale[0.8]{ Preliminary}}}");

  TH1D *hist_2016_Sum = new TH1D("hist_2016_Sum", "", 3, 0., 3.);
  TH1D *hist_2018_Iter = new TH1D("hist_2018_Iter", "", 3, 0., 3.);
  TH1D *hist_2018_IterNoDoublet = new TH1D("hist_2018_IterNoDoublet", "", 3, 0., 3.);
  for(int i=1; i<=3; i++){
    hist_2016_Sum->SetBinContent(i, y_2016_Sum[i-1]);
    hist_2018_Iter->SetBinContent(i, y_2018_Iter[i-1]);
    hist_2018_IterNoDoublet->SetBinContent(i, y_2018_IterNoDoublet[i-1]);
  }

  int lineWidth=2;

  //==== 2016, Sum
  hist_2016_Sum->SetMarkerStyle(20);
  hist_2016_Sum->SetMarkerColor(kBlue);
  hist_2016_Sum->SetLineColor(kBlue);
  hist_2016_Sum->SetLineWidth(lineWidth);
  //==== 2018, Iter
  hist_2018_Iter->SetMarkerStyle(22);
  hist_2018_Iter->SetMarkerColor(kBlack);
  hist_2018_Iter->SetLineColor(kBlack);
  hist_2018_Iter->SetLineWidth(lineWidth);
  //==== 2018, IterNoDoublet
  hist_2018_IterNoDoublet->SetMarkerStyle(25);
  hist_2018_IterNoDoublet->SetMarkerColor(kViolet);
  hist_2018_IterNoDoublet->SetLineColor(kViolet);
  hist_2018_IterNoDoublet->SetLineWidth(lineWidth);

  TLegend *lg = new TLegend(0.20, 0.69, 0.95, 0.90);
  lg->SetMargin(0.15);
  lg->SetFillStyle(0);
  lg->SetBorderSize(0);

  lg->AddEntry( hist_2016_Sum, "2016, cascade and tracker muon (45 < pileup < 50)", "lp");
  lg->AddEntry( hist_2018_Iter, "2018, iterative (46 < pileup < 48)", "lp");
  lg->AddEntry( hist_2018_IterNoDoublet, "2018, iterative w/o doublet iteration (46 < pileup < 48)", "lp");
  lg->Draw();

  hist_2016_Sum->Draw("histsame");
  hist_2018_Iter->Draw("histsame");
  hist_2018_IterNoDoublet->Draw("histsame");

  c->SaveAs(OutDir+"/L1L2L3Iso.pdf");
  c->Close();

}
