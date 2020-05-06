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


void Draw_ComparevsPU_2018(){

  gStyle->SetOptStat();
  setTDRStyle();

  TString OutDir = "output/PUStudy/2018/";
  gSystem->mkdir(OutDir,kTRUE);

  vector<TString> PUs = {
"36-38",
"38-40",
"40-42",
"42-44",
"44-46",
"46-48",
"48-50",
"50-52",
  };

  vector<int> Runs = {
324878,
325022,
324878,
325022,
325022,
325022,
325022,
325022,
  };

  vector<Color_t> colors = {
    kRed,
    kOrange,
    kYellow+1,
    kGreen,
    kBlue,
    kViolet,
    kGray,
    kBlack,
  };
 
  //==== Paths
  vector<TString> Paths = {
    "HLT_Mu27_v13",
    "HLT_IsoMu27_v16",
    "HLT_Mu50_v13",
    "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v5",
  };

  TLatex latex_CMSPriliminary, latex_Trigger;
  latex_CMSPriliminary.SetNDC();
  latex_Trigger.SetNDC();
  latex_CMSPriliminary.SetTextSize(0.035);
  latex_Trigger.SetTextSize(0.030);

  for(unsigned int i_path=0; i_path<Paths.size(); i_path++){

    TString Path = Paths.at(i_path);

    cout << "@@@@ Path = " << Path << endl;

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

    TLegend *lg2 = new TLegend(0.34, 0.65, 0.95, 0.93);
    lg2->SetBorderSize(0);
    lg2->SetFillStyle(0);

    for(unsigned int i_PU=0; i_PU<PUs.size(); i_PU++){

      TString PU = PUs.at(i_PU);
      TString Run = TString::Itoa( Runs.at(i_PU), 10 );

      TFile *file = new TFile("rootfiles/PUStudy/2018/job_PU"+PU+"/DQM_V0001_R000"+Run+"__HLT__FastTimerService__All.root");

      TH1F *hist = (TH1F *)file->Get("DQMData/Run "+Run+"/HLT/Run summary/TimerService/process TIMING paths/path "+Path+"/path time_real");
      hist->Rebin(2);

      hist->Scale(1./hist->Integral());

      hist->SetMarkerColor(0);
      hist->SetMarkerSize(0);
      hist->SetLineColor(colors.at(i_PU));
      hist->SetLineWidth(2);
      double MeanTiming = hist->GetMean();
      TString str_MeanTiming = Precision(MeanTiming,2);
      cout << MeanTiming << " -> " << str_MeanTiming << endl;

      lg2->AddEntry(hist, "Run: "+Run+", Pile-up range : "+PU+" Mean: "+str_MeanTiming+" ms", "l");
      c_Path->cd();
      hist->Draw("histsame");
    }

    c_Path->SaveAs();

    TLatex latex_Lumi;
    latex_Lumi.SetNDC();
    latex_Lumi.SetTextSize(0.035);
    latex_Lumi.SetTextFont(42);
    latex_Lumi.DrawLatex(0.72, 0.96, "Run2018");

    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}");
    lg2->Draw();
    c_Path->SaveAs(OutDir+"/"+Path+".pdf");
    c_Path->Close();

  }

}
