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


void Draw_ComparevsPU_2016_vs_2018(){

  gStyle->SetOptStat();
  setTDRStyle();

  TString OutDir = "output/PUStudy/";
  gSystem->mkdir(OutDir,kTRUE);

  vector<TString> PUs_2016 = {
"23-27",
"33-37",
"45-50",
  };

  vector<int> Runs_2016 = {
283270,
283453,
283353,
  };
  double x_PUs_2016[3] = { 25., 35., 47.5 };
  double x_PUs_err_2016[3] = { 2., 2., 2.5 };

  //==== Paths
  vector<TString> Paths_2016 = {
    "HLT_IsoMu24_v4",
    "HLT_IsoTkMu24_v4",

    "HLT_Mu50_v5",
    "HLT_TkMu50_v3",

    "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v7",
    "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3",
  };
  vector<TGraphAsymmErrors *> grs_2016;

  for(unsigned int i_path=0; i_path<Paths_2016.size(); i_path++){

    TString Path = Paths_2016.at(i_path);

    double MeanValues[3];
    double MeanValues_Error[3];
    for(unsigned int i_PU=0; i_PU<PUs_2016.size(); i_PU++){

      TString PU = PUs_2016.at(i_PU);
      TString Run = TString::Itoa( Runs_2016.at(i_PU), 10 );

      TFile *file = new TFile("rootfiles/PUStudy/2016/job_PU"+PU+"/DQM_V0001_R000"+Run+"__HLT__FastTimerService__All.root");
      TH1F *hist = (TH1F *)file->Get("DQMData/Run "+Run+"/HLT/Run summary/TimerService/Running 1 processes/process TIMING/Paths/"+Path+"_total");
      hist->Rebin(2);

      hist->Scale(1./hist->Integral());

      double MeanTiming = hist->GetMean();
      double MeanTiming_Error = hist->GetMeanError();

      MeanValues[i_PU] = MeanTiming ;
      MeanValues_Error[i_PU] = MeanTiming_Error;

    }

    TGraphAsymmErrors *gr = new TGraphAsymmErrors(3, x_PUs_2016, MeanValues, x_PUs_err_2016, x_PUs_err_2016, MeanValues_Error, MeanValues_Error);
    gr->SetName(Path+"_2016");
    grs_2016.push_back( gr );
  }

  //==== 2018

  vector<TString> PUs_2018 = {
"36-38",
"38-40",
"40-42",
"42-44",
"44-46",
"46-48",
"48-50",
"50-52",
  };

  vector<int> Runs_2018 = {
324878,
325022,
324878,
325022,
325022,
325022,
325022,
325022,
  };
  double x_PUs_2018[8] = {37, 39, 41, 43, 45, 47, 49, 51};
  double x_PUs_err_2018[8] = {1, 1, 1, 1, 1, 1, 1, 1};

  //==== Paths
  vector<TString> Paths_2018 = {
    "HLT_IsoMu24_v13",

    "HLT_Mu50_v13",

    "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v5",
  };
  vector<TGraphAsymmErrors *> grs_2018;

  for(unsigned int i_path=0; i_path<Paths_2018.size(); i_path++){

    TString Path = Paths_2018.at(i_path);

    double MeanValues[8];
    double MeanValues_Error[8];
    for(unsigned int i_PU=0; i_PU<PUs_2018.size(); i_PU++){

      TString PU = PUs_2018.at(i_PU);
      TString Run = TString::Itoa( Runs_2018.at(i_PU), 10 );

      TFile *file = new TFile("rootfiles/PUStudy/2018/job_PU"+PU+"/DQM_V0001_R000"+Run+"__HLT__FastTimerService__All.root");
      TH1F *hist = (TH1F *)file->Get("DQMData/Run "+Run+"/HLT/Run summary/TimerService/process TIMING paths/path "+Path+"/path time_real");
      hist->Rebin(2);

      hist->Scale(1./hist->Integral());

      double MeanTiming = hist->GetMean();
      double MeanTiming_Error = hist->GetMeanError();

      MeanValues[i_PU] = MeanTiming ;
      MeanValues_Error[i_PU] = MeanTiming_Error;

    }

    TGraphAsymmErrors *gr = new TGraphAsymmErrors(8, x_PUs_2018, MeanValues, x_PUs_err_2018, x_PUs_err_2018, MeanValues_Error, MeanValues_Error);
    gr->SetName(Path+"_2018");
    grs_2018.push_back( gr );
  }

  TH1D *hist_dummy = new TH1D("hist_dummy", "", 200, 0., 100.);
  hist_axis(hist_dummy);
  hist_dummy->GetYaxis()->SetTitle("Average CPU time [ms]");
  hist_dummy->GetXaxis()->SetTitle("Mean number of interactions per crossing");
  hist_dummy->GetXaxis()->SetRangeUser(20, 55);

  //==== IsoMu24

  cout << "@@@@ Drawing IsoMu24" << endl;

  TCanvas *c_IsoMu24 = new TCanvas("c_IsoMu24", "", 600, 600);
  canvas_margin(c_IsoMu24);
  hist_dummy->Draw("histaxis");
  hist_dummy->GetYaxis()->SetRangeUser(0., 20.);
  //=== 2016, HLT_IsoMu24_v4
  cout << grs_2016.at(0)->GetName() << endl;
  grs_2016.at(0)->SetMarkerStyle(20);
  grs_2016.at(0)->SetMarkerColor(kGreen);
  grs_2016.at(0)->SetLineColor(kGreen);
  //=== 2016, HLT_IsoTkMu24_v4
  cout << grs_2016.at(1)->GetName() << endl;
  grs_2016.at(1)->SetMarkerStyle(21);
  grs_2016.at(1)->SetMarkerColor(kRed);
  grs_2016.at(1)->SetLineColor(kRed);
  //==== 2016, Cas+Trk
  double y_IsoMu24_2016[3] = {
3.90398642106,
5.3604611091,
6.06314451156,
  };
  TGraphAsymmErrors *gr_IsoMu24_2016 = new TGraphAsymmErrors(3, x_PUs_2016, y_IsoMu24_2016, x_PUs_err_2016, x_PUs_err_2016, 0, 0);
  gr_IsoMu24_2016->SetMarkerStyle(20);
  gr_IsoMu24_2016->SetMarkerColor(kBlue);
  gr_IsoMu24_2016->SetLineColor(kBlue);

  //==== 2018, HLT_IsoMu24_v13
  cout << grs_2018.at(0)->GetName() << endl;
  grs_2018.at(0)->SetMarkerStyle(22);
  grs_2018.at(0)->SetMarkerColor(kBlack);
  grs_2018.at(0)->SetLineColor(kBlack);

  TLatex latex_info;
  latex_info.SetNDC();
  latex_info.SetTextSize(0.030);
  latex_info.DrawLatex(0.20, 0.89, "Isolated single #mu trigger with p_{T} > 24 GeV");

  TLegend *lg_IsoMu24 = new TLegend(0.18, 0.72, 0.80, 0.87);
  lg_IsoMu24->SetFillStyle(0);
  lg_IsoMu24->SetBorderSize(0);
  //lg_IsoMu24->AddEntry( grs_2016.at(0), "2016, cascade", "lp");
  //lg_IsoMu24->AddEntry( grs_2016.at(1), "2016, tracker muon", "lp");
  lg_IsoMu24->AddEntry( gr_IsoMu24_2016, "2016, cascade and tracker muon", "lp");
  lg_IsoMu24->AddEntry( grs_2018.at(0), "2018, iterative", "lp");
  lg_IsoMu24->Draw();

  //grs_2016.at(0)->Draw("psame");
  //grs_2016.at(1)->Draw("psame");
  gr_IsoMu24_2016->Draw("psame");
  grs_2018.at(0)->Draw("psame");

  gr_IsoMu24_2016->Draw("psame");
  
  TLatex latex_year;
  latex_year.SetNDC();
  latex_year.SetTextSize(0.035);
  latex_year.SetTextFont(42);
  latex_year.DrawLatex(0.61, 0.96, "2016 and 2018, 13 TeV");
  TLatex latex_CMSPriliminary;
  latex_CMSPriliminary.DrawLatexNDC(0.16, 0.96, "#font[62]{CMS}#font[42]{#it{#scale[0.8]{ Preliminary}}}");

  c_IsoMu24->SaveAs(OutDir+"/IsoMu24.pdf");
  c_IsoMu24->Close();


  //==== Mu50

  cout << "@@@@ Drawing Mu50" << endl;

  TCanvas *c_Mu50 = new TCanvas("c_Mu50", "", 600, 600);
  canvas_margin(c_Mu50);
  hist_dummy->Draw("histaxis");
  hist_dummy->GetYaxis()->SetRangeUser(0., 20.);
  //=== 2016, HLT_Mu50_v5
  cout << grs_2016.at(2)->GetName() << endl;
  grs_2016.at(2)->SetMarkerStyle(20);
  grs_2016.at(2)->SetMarkerColor(kGreen);
  grs_2016.at(2)->SetLineColor(kGreen);
  //=== 2016, HLT_TkMu50_v3
  cout << grs_2016.at(3)->GetName() << endl;
  grs_2016.at(3)->SetMarkerStyle(21);
  grs_2016.at(3)->SetMarkerColor(kRed);
  grs_2016.at(3)->SetLineColor(kRed);
  //==== 2016, Cas+Trk
  double y_Mu50_2016[3] = {
3.75832255851,
4.95029051021,
5.71665733464,
  };
  TGraphAsymmErrors *gr_Mu50_2016 = new TGraphAsymmErrors(3, x_PUs_2016, y_Mu50_2016, x_PUs_err_2016, x_PUs_err_2016, 0, 0);
  gr_Mu50_2016->SetMarkerStyle(20);
  gr_Mu50_2016->SetMarkerColor(kBlue);
  gr_Mu50_2016->SetLineColor(kBlue);
  //==== 2018, HLT_Mu50_v13
  cout << grs_2018.at(1)->GetName() << endl;
  grs_2018.at(1)->SetMarkerStyle(22);
  grs_2018.at(1)->SetMarkerColor(kBlack);
  grs_2018.at(1)->SetLineColor(kBlack);

  latex_info.DrawLatex(0.20, 0.89, "Non-isolated single #mu trigger with p_{T} > 50 GeV");
  TLegend *lg_Mu50 = new TLegend(0.18, 0.72, 0.80, 0.87);
  lg_Mu50->SetFillStyle(0);
  lg_Mu50->SetBorderSize(0);
  //lg_Mu50->AddEntry( grs_2016.at(2), "2016, cascade", "lp");
  //lg_Mu50->AddEntry( grs_2016.at(3), "2016, tracker muon", "lp");
  lg_Mu50->AddEntry( gr_Mu50_2016, "2016, cascade and tracker muon", "lp");
  lg_Mu50->AddEntry( grs_2018.at(1), "2018, iterative", "lp");

  lg_Mu50->Draw();

  //grs_2016.at(2)->Draw("psame");
  //grs_2016.at(3)->Draw("psame");
  gr_Mu50_2016->Draw("psame");
  grs_2018.at(1)->Draw("psame");

  latex_year.SetNDC();
  latex_year.SetTextSize(0.035);
  latex_year.SetTextFont(42);
  latex_year.DrawLatex(0.61, 0.96, "2016 and 2018, 13 TeV");
  latex_CMSPriliminary.DrawLatexNDC(0.16, 0.96, "#font[62]{CMS}#font[42]{#it{#scale[0.8]{ Preliminary}}}");

  c_Mu50->SaveAs(OutDir+"/Mu50.pdf");
  c_Mu50->Close();

  //==== Special lines for L3

  double L3_MeanValues_2016_Tk[3] = {
    7.93E-01,
    1.70E+00,
    2.11E+00,
  };
  double L3_MeanValues_Error_2016_Tk[3] = {
    0,
    0,
    0,
  };
  TGraphAsymmErrors *gr_L3_2016_Tk = new TGraphAsymmErrors(3, x_PUs_2016, L3_MeanValues_2016_Tk, x_PUs_err_2016, x_PUs_err_2016, L3_MeanValues_Error_2016_Tk, L3_MeanValues_Error_2016_Tk);

  double L3_MeanValues_2016_Cas[3] = {
    3.37E-01,
    6.66E-01,
    8.30E-01,
  };
  double L3_MeanValues_Error_2016_Cas[3] = {
    0,
    0,
    0,
  };
  TGraphAsymmErrors *gr_L3_2016_Cas = new TGraphAsymmErrors(3, x_PUs_2016, L3_MeanValues_2016_Cas, x_PUs_err_2016, x_PUs_err_2016, L3_MeanValues_Error_2016_Cas, L3_MeanValues_Error_2016_Cas);

  double L3_MeanValues_2016[3] = {
0.92101039827,
1.94405054348,
2.45366107079,
  };
  TGraphAsymmErrors *gr_L3_2016 = new TGraphAsymmErrors(3, x_PUs_2016, L3_MeanValues_2016, x_PUs_err_2016, x_PUs_err_2016, 0, 0);

  double L3_MeanValues_2018_Iter[8] = {
    8.02E+00,
    8.31E+00,
    9.42E+00,
    9.54E+00,
    1.00E+01,
    1.04E+01,
    1.08E+01,
    1.15E+01,
  };
  double L3_MeanValues_Error_2018_Iter[8] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
  };
  TGraphAsymmErrors *gr_L3_2018_Iter = new TGraphAsymmErrors(8, x_PUs_2018, L3_MeanValues_2018_Iter, x_PUs_err_2018, x_PUs_err_2018, L3_MeanValues_Error_2018_Iter, L3_MeanValues_Error_2018_Iter);

  cout << "@@@@ Drawing L3MuonReco" << endl;

  TCanvas *c_L3MuonReco = new TCanvas("c_L3MuonReco", "", 600, 600);
  canvas_margin(c_L3MuonReco);
  hist_dummy->Draw("histaxis");
  hist_dummy->GetYaxis()->SetRangeUser(0., 20.);
  //=== 2016, Cas
  gr_L3_2016_Cas->SetMarkerStyle(20);
  gr_L3_2016_Cas->SetMarkerColor(kGreen);
  gr_L3_2016_Cas->SetLineColor(kGreen);
  //=== 2016, Tk
  gr_L3_2016_Tk->SetMarkerStyle(21);
  gr_L3_2016_Tk->SetMarkerColor(kRed);
  gr_L3_2016_Tk->SetLineColor(kRed);
  //=== 2016 Sum
  gr_L3_2016->SetMarkerStyle(21);
  gr_L3_2016->SetMarkerColor(kBlue);
  gr_L3_2016->SetLineColor(kBlue);
  //==== 2018, Iter
  gr_L3_2018_Iter->SetMarkerStyle(22);
  gr_L3_2018_Iter->SetMarkerColor(kBlack);
  gr_L3_2018_Iter->SetLineColor(kBlack);

  latex_info.DrawLatex(0.20, 0.89, "L3 sequence");
  TLegend *lg_L3MuonReco = new TLegend(0.18, 0.67, 0.68, 0.87);
  lg_L3MuonReco->SetFillStyle(0);
  lg_L3MuonReco->SetBorderSize(0);
  //lg_L3MuonReco->AddEntry( gr_L3_2016_Cas, "2016, cascade", "lp");
  //lg_L3MuonReco->AddEntry( gr_L3_2016_Tk, "2016, tracker muon", "lp");
  lg_L3MuonReco->AddEntry( gr_L3_2016, "2016, cascade and tracker muon", "lp");
  lg_L3MuonReco->AddEntry( gr_L3_2018_Iter, "2018, iterative", "lp");
  lg_L3MuonReco->Draw();

  //gr_L3_2016_Tk->Draw("psame");
  //gr_L3_2016_Cas->Draw("psame");
  gr_L3_2016->Draw("psame");
  gr_L3_2018_Iter->Draw("psame");

  latex_year.SetNDC();
  latex_year.SetTextSize(0.035);
  latex_year.SetTextFont(42);
  latex_year.DrawLatex(0.61, 0.96, "2016 and 2018, 13 TeV");
  latex_CMSPriliminary.DrawLatexNDC(0.16, 0.96, "#font[62]{CMS}#font[42]{#it{#scale[0.8]{ Preliminary}}}");

  c_L3MuonReco->SaveAs(OutDir+"/L3MuonReco.pdf");
  c_L3MuonReco->Close();

}
