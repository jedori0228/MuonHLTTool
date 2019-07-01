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


void Draw_CompareIso_2018(){

  gStyle->SetOptStat();
  setTDRStyle();

  TString OutDir = "output/IsoTiming/";
  gSystem->mkdir(OutDir,kTRUE);

  vector<int> Runs = {
  319941,
  };
  vector<TString> aliases = {
  "Run319941 (2018)",  //"PU48-49 Menu-HLTv3.5V10Run319941",
  //"PU44-47 Menu-HLTv3.5V10Run321755",
  };

  //==== Paths
  vector<TString> Paths = {
    "HLT_Mu27_v13",
    "HLT_IsoMu27_v16",
    "HLT_Mu50_v13",
  };
  vector<Color_t> colors = {
  kBlack,
  kRed,
  kGreen,
  };

  TLatex latex_CMSPriliminary, latex_Trigger;
  latex_CMSPriliminary.SetNDC();
  latex_Trigger.SetNDC();
  latex_CMSPriliminary.SetTextSize(0.035);
  latex_Trigger.SetTextSize(0.030);

  for(unsigned int i=0; i<Runs.size(); i++){

    TString Run = TString::Itoa( Runs.at(i), 10 );

    cout << "@@@@ " << Run << endl;

    cout << "rootfiles/DQM_V0001_R000"+Run+"__HLT__FastTimerService__All.root" << endl;
    TFile *file = new TFile("rootfiles/DQM_V0001_R000"+Run+"__HLT__FastTimerService__All.root");

    TCanvas *c_Path = new TCanvas("c_Path", "", 1000, 800);
    canvas_margin(c_Path);
    c_Path->cd();
    c_Path->SetLogy();
    TH1D *hist_dummy_Path = new TH1D("hist_dummy_Path", "", 1000, 0., 1000.);
    hist_dummy_Path->GetYaxis()->SetRangeUser(0.000008,2);
    hist_dummy_Path->GetXaxis()->SetRangeUser(0,500);
    hist_dummy_Path->GetYaxis()->SetTitle("events / 10.0 ms");
    hist_dummy_Path->GetXaxis()->SetTitle("processing time [ms]");
    hist_dummy_Path->Draw("axis");
    hist_axis(hist_dummy_Path);

    TLegend *lg2 = new TLegend(0.34, 0.80, 0.99, 0.95);
    lg2->SetBorderSize(0);
    lg2->SetFillStyle(0);

    for(unsigned int it_Path=0; it_Path<Paths.size(); it_Path++){

      TString Path = Paths.at(it_Path);

      cout << "@@@@ Path = " << Path << endl;

      cout << "DQMData/Run "+Run+"/HLT/Run summary/TimerService/process TIMING paths/path "+Path+"/path time_real" << endl;
      TH1F *hist = (TH1F *)file->Get("DQMData/Run "+Run+"/HLT/Run summary/TimerService/process TIMING paths/path "+Path+"/path time_real");
      hist->Rebin(2);

      hist->Scale(1./hist->Integral());

      hist->SetLineColor(colors.at(it_Path));
      hist->SetLineWidth(2);
      double MeanTiming = hist->GetMean();
      TString str_MeanTiming = Precision(MeanTiming,2);
      cout << MeanTiming << " -> " << str_MeanTiming << endl;

      lg2->AddEntry(hist, Path+" Mean: "+str_MeanTiming+" ms");
      c_Path->cd();
      hist->Draw("histsame");


    }

    //==== Latex

    c_Path->cd();
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
    //latex_Trigger.DrawLatex(0.03, 0.03, Path);
    lg2->Draw();
    c_Path->SaveAs(OutDir+"/Run_"+Run+"_2018_Iso.pdf");
    c_Path->SaveAs(OutDir+"/Run_"+Run+"_2018_Iso.png");
    c_Path->Close();

  }



}
