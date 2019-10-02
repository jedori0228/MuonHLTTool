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
        cout << "[Precision]  i = " << i << endl;
        str_frac = "0"+str_frac;
      }
      break;
    }
  }
  return str_int+"."+str_frac;


}


void Draw_CompareMu50(){

  gStyle->SetOptStat();
  setTDRStyle();

  TString OutDir = "output/IsoTiming/";
  gSystem->mkdir(OutDir,kTRUE);

  vector<int> Runs = {
  283407,
  283407,
  319941,
  };
  vector<TString> aliases = {
  "Cascade (2016 data)",
  "Tracker muon (2016 data)",
  "Iterative (2018 data)",
  };
  vector<TString> Paths = {
    "HLT_Mu50_v5",
    "HLT_TkMu50_v3",
    "HLT_Mu50_v13",
  };
  vector<Color_t> colors = {
  kBlack,
  kBlack,
  kRed,
  };
  vector<Style_t> styles = {
  1,
  3,
  1,
  };

  TLatex latex_CMSPriliminary, latex_Trigger;
  latex_CMSPriliminary.SetNDC();
  latex_Trigger.SetNDC();
  latex_CMSPriliminary.SetTextSize(0.035);
  latex_Trigger.SetTextSize(0.035);

  TCanvas *c_Path = new TCanvas("c_Path", "", 1000, 800);
  canvas_margin(c_Path);
  c_Path->cd();
  c_Path->SetLogy();
  TH1D *hist_dummy_Path = new TH1D("hist_dummy_Path", "", 2000, 0., 2000.);
  hist_dummy_Path->GetYaxis()->SetRangeUser(0.000008,2);
  hist_dummy_Path->GetXaxis()->SetRangeUser(0, 500);
  hist_dummy_Path->GetYaxis()->SetTitle("events normalized to 1");
  hist_dummy_Path->GetXaxis()->SetTitle("processing time [ms]");
  hist_dummy_Path->Draw("axis");
  hist_axis(hist_dummy_Path);

  TLegend *lg = new TLegend(0.34, 0.70, 0.95, 0.85);
  lg->SetBorderSize(0);
  lg->SetFillStyle(0);

  for(unsigned int i=0; i<Runs.size(); i++){

    TString Run = TString::Itoa( Runs.at(i), 10 );
    TString Path = Paths.at(i);
    Color_t color = colors.at(i);
    Style_t style = styles.at(i);

    cout << "rootfiles/DQM_V0001_R000"+Run+"__HLT__FastTimerService__All.root" << endl;
    TFile *file = new TFile("rootfiles/DQM_V0001_R000"+Run+"__HLT__FastTimerService__All.root");

    TString histname = "DQMData/Run "+Run+"/HLT/Run summary/TimerService/Running 1 processes/process TIMING/Paths/"+Path+"_total";
    if(i==2) histname = "DQMData/Run "+Run+"/HLT/Run summary/TimerService/process TIMING paths/path "+Path+"/path time_real";

    cout << histname << endl;

    TH1F *hist = (TH1F *)file->Get(histname);
    hist->Rebin(2);
    hist->Scale(1./hist->Integral());

    hist->SetLineColor(colors.at(i));
    hist->SetLineStyle(styles.at(i));
    hist->SetLineWidth(2);
    hist->SetMarkerSize(0);
    hist->SetMarkerColor(colors.at(i));
    double MeanTiming = hist->GetMean();
    TString str_MeanTiming = Precision(MeanTiming,2);
    cout << MeanTiming << " -> " << str_MeanTiming << endl;

    lg->AddEntry(hist, aliases.at(i)+" Mean: "+str_MeanTiming+" ms");
    c_Path->cd();
    hist->Draw("histsame");

  }

  //==== Latex

  c_Path->cd();
  latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
  latex_Trigger.DrawLatex(0.365, 0.86, "Mu50");
  lg->Draw();
  c_Path->SaveAs(OutDir+"/Mu50.pdf");
  c_Path->SaveAs(OutDir+"/Mu50.png");
  c_Path->Close();




}
