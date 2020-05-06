#include <TagProbe/TnPTool.h>
#include "canvas_margin.h"

TString filebasedir = "hists/Pt24Updtaes/";
TString fileName1 = "L2overOffline__Run2016.root";
TString fileName2 = "L2overOffline__Run2017.root";
TString fileName3 = "L2overOffline__Run2018.root";

void DrawHistForEachVariable(TString var, Bool_t setZoomIn = kFALSE,int FirstBinIndexFor2D=-1, int FirstBinXIndexFor2D=-1);
TGraphAsymmErrors* GetEffGraph( TString fileName, TString var,int FirstBinIndexFor2D=-1, int FirstBinXIndexFor2D=-1 );

vector<TGraphAsymmErrors *> gr1_for_each_etabin;
vector<TGraphAsymmErrors *> gr2_for_each_etabin;
vector<TGraphAsymmErrors *> gr3_for_each_etabin;
TString EtabinInfo = "";

void DrawHist_Comparison_L2overOffline() {

  DrawHistForEachVariable( "Pt" );
  DrawHistForEachVariable( "Pt", kTRUE );
  DrawHistForEachVariable( "Eta" );
  DrawHistForEachVariable( "Phi" );
  DrawHistForEachVariable( "Vtx" );

  cout << "@@@@ filebasedir = " << filebasedir << endl;
  cout << "@@@@ fileName1 = " << fileName1 << endl;
  cout << "@@@@ fileName2 = " << fileName2 << endl;
  cout << "@@@@ fileName3 = " << fileName3 << endl;
  TFile *f = new TFile(filebasedir+fileName1);
  TH2D *hist_Eta_vs_Phi_template = (TH2D *)f->Get("hEffTemplate2DEta_vs_Phi");
  int ibin = 0;
  for(int ix=0; ix<hist_Eta_vs_Phi_template->GetXaxis()->GetNbins(); ix++){

    double eta_l = hist_Eta_vs_Phi_template->GetXaxis()->GetBinLowEdge(ix+1);
    double eta_r = hist_Eta_vs_Phi_template->GetXaxis()->GetBinUpEdge(ix+1);
    EtabinInfo = TString::Format( "%1.2f<#||{#eta}<%1.2f", eta_l, eta_r);
    DrawHistForEachVariable( "Eta_vs_Phi", kFALSE, ibin, ix );

    for(int iy=0; iy<hist_Eta_vs_Phi_template->GetYaxis()->GetNbins(); iy++){
      TString binInfo = TString::Format("%02dbin_%02d_%02d", ibin, ix, iy);
      cout << binInfo << endl;
      ibin++;
    }

  }

  TH2D *hist_eff_1 = (TH2D *)hist_Eta_vs_Phi_template->Clone();
  TH2D *hist_eff_2 = (TH2D *)hist_Eta_vs_Phi_template->Clone();
  TH2D *hist_eff_3 = (TH2D *)hist_Eta_vs_Phi_template->Clone();

  TH2D *hist_eff_2over1 = (TH2D *)hist_Eta_vs_Phi_template->Clone();
  TH2D *hist_eff_3over1 = (TH2D *)hist_Eta_vs_Phi_template->Clone();
  TH2D *hist_eff_3over2 = (TH2D *)hist_Eta_vs_Phi_template->Clone();

  for(int ix=0; ix<hist_Eta_vs_Phi_template->GetXaxis()->GetNbins(); ix++){

    TGraphAsymmErrors *this_gr1 = gr1_for_each_etabin.at(ix);
    TGraphAsymmErrors *this_gr2 = gr2_for_each_etabin.at(ix);
    TGraphAsymmErrors *this_gr3 = gr3_for_each_etabin.at(ix);
    double eta_l = hist_Eta_vs_Phi_template->GetXaxis()->GetBinLowEdge(ix+1);
    double eta_r = hist_Eta_vs_Phi_template->GetXaxis()->GetBinUpEdge(ix+1);

    cout << "Eta : " << eta_l << ", " << eta_r << endl;

    Double_t* Phis = this_gr1->GetX();
    Double_t* Effs_1 = this_gr1->GetY();
    Double_t* Effs_2 = this_gr2->GetY();
    Double_t* Effs_3 = this_gr3->GetY();
    int nPhi = this_gr1->GetN();
    for(int a=0;a<nPhi;a++){
      cout << "  " <<  a << "\t" << Phis[a] << endl;
      hist_eff_1->SetBinContent(ix+1, a+1, Effs_1[a]);
      hist_eff_2->SetBinContent(ix+1, a+1, Effs_2[a]);
      hist_eff_3->SetBinContent(ix+1, a+1, Effs_3[a]);

      hist_eff_2over1->SetBinContent(ix+1, a+1, Effs_2[a]/Effs_1[a]);
      hist_eff_3over1->SetBinContent(ix+1, a+1, Effs_3[a]/Effs_1[a]);
      hist_eff_3over2->SetBinContent(ix+1, a+1, Effs_3[a]/Effs_2[a]);

    }

  }
  setTDRStyle();
  gStyle->SetPaintTextFormat("1.3f");

  //==== 2016

  TCanvas *c_2D_1 = new TCanvas("c_2D_1", "", 1200, 800);
  canvas_margin(c_2D_1);

  c_2D_1->SetTopMargin( 0.05 );
  c_2D_1->SetBottomMargin( 0.13 );
  c_2D_1->SetRightMargin( 0.15 );
  c_2D_1->SetLeftMargin( 0.10 );

  c_2D_1->cd();
  hist_eff_1->Draw("colztext");
  hist_eff_1->GetXaxis()->SetTitle("#eta(#mu)");
  hist_eff_1->GetYaxis()->SetTitle("#phi(#mu)");

  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.035);
  latex.DrawLatex(0.15, 0.96, "2016 data");

  c_2D_1->SaveAs("outputs/L2overOffline/c_vsEta_vs_Phi_2016.pdf");
  c_2D_1->Close();
 
  //==== 2017

  TCanvas *c_2D_2 = new TCanvas("c_2D_2", "", 1200, 800);
  canvas_margin(c_2D_2);

  c_2D_2->SetTopMargin( 0.05 );
  c_2D_2->SetBottomMargin( 0.13 );
  c_2D_2->SetRightMargin( 0.15 );
  c_2D_2->SetLeftMargin( 0.10 );

  c_2D_2->cd();
  hist_eff_2->Draw("colztext");
  hist_eff_2->GetXaxis()->SetTitle("#eta(#mu)");
  hist_eff_2->GetYaxis()->SetTitle("#phi(#mu)");

  latex.DrawLatex(0.15, 0.96, "2017 data");

  c_2D_2->SaveAs("outputs/L2overOffline/c_vsEta_vs_Phi_2017.pdf");
  c_2D_2->Close();

  //==== 2018

  TCanvas *c_2D_3 = new TCanvas("c_2D_3", "", 1200, 800);
  canvas_margin(c_2D_3);

  c_2D_3->SetTopMargin( 0.05 );
  c_2D_3->SetBottomMargin( 0.13 );
  c_2D_3->SetRightMargin( 0.15 );
  c_2D_3->SetLeftMargin( 0.10 );

  c_2D_3->cd();
  hist_eff_3->Draw("colztext");
  hist_eff_3->GetXaxis()->SetTitle("#eta(#mu)");
  hist_eff_3->GetYaxis()->SetTitle("#phi(#mu)");

  latex.DrawLatex(0.15, 0.96, "2018 data");

  c_2D_3->SaveAs("outputs/L2overOffline/c_vsEta_vs_Phi_2018.pdf");
  c_2D_3->Close();

  //==== 2017/2016

  TCanvas *c_2D_2over1 = new TCanvas("c_2D_2over1", "", 1200, 800);
  canvas_margin(c_2D_2over1);

  c_2D_2over1->SetTopMargin( 0.05 );
  c_2D_2over1->SetBottomMargin( 0.13 );
  c_2D_2over1->SetRightMargin( 0.15 );
  c_2D_2over1->SetLeftMargin( 0.10 );

  c_2D_2over1->cd();
  hist_eff_2over1->Draw("colztext");
  hist_eff_2over1->GetXaxis()->SetTitle("#eta(#mu)");
  hist_eff_2over1->GetYaxis()->SetTitle("#phi(#mu)");

  latex.DrawLatex(0.15, 0.96, "2017/2016");

  c_2D_2over1->SaveAs("outputs/L2overOffline/c_vsEta_vs_Phi_2017over2016.pdf");
  c_2D_2over1->Close();

  //==== 2018/2016

  TCanvas *c_2D_3over1 = new TCanvas("c_2D_3over1", "", 1200, 800);
  canvas_margin(c_2D_3over1);

  c_2D_3over1->SetTopMargin( 0.05 );
  c_2D_3over1->SetBottomMargin( 0.13 );
  c_2D_3over1->SetRightMargin( 0.15 );
  c_2D_3over1->SetLeftMargin( 0.10 );

  c_2D_3over1->cd();
  hist_eff_3over1->Draw("colztext");
  hist_eff_3over1->GetXaxis()->SetTitle("#eta(#mu)");
  hist_eff_3over1->GetYaxis()->SetTitle("#phi(#mu)");

  latex.DrawLatex(0.15, 0.96, "2018/2016");

  c_2D_3over1->SaveAs("outputs/L2overOffline/c_vsEta_vs_Phi_2018over2016.pdf");
  c_2D_3over1->Close();

  //==== 2018/2017

  TCanvas *c_2D_3over2 = new TCanvas("c_2D_3over2", "", 1200, 800);
  canvas_margin(c_2D_3over2);

  c_2D_3over2->SetTopMargin( 0.05 );
  c_2D_3over2->SetBottomMargin( 0.13 );
  c_2D_3over2->SetRightMargin( 0.15 );
  c_2D_3over2->SetLeftMargin( 0.10 );

  c_2D_3over2->cd();
  hist_eff_3over2->Draw("colztext");
  hist_eff_3over2->GetXaxis()->SetTitle("#eta(#mu)");
  hist_eff_3over2->GetYaxis()->SetTitle("#phi(#mu)");

  latex.DrawLatex(0.15, 0.96, "2018/2017");

  c_2D_3over2->SaveAs("outputs/L2overOffline/c_vsEta_vs_Phi_2018over2017.pdf");
  c_2D_3over2->Close();



}

void DrawHistForEachVariable(TString var, Bool_t setZoomIn = kFALSE, int FirstBinIndexFor2D=-1, int FirstBinXIndexFor2D=-1)
{

  TGraphAsymmErrors* g1 = GetEffGraph(filebasedir+fileName1, var, FirstBinIndexFor2D, FirstBinXIndexFor2D);
  TGraphAsymmErrors* g2 = GetEffGraph(filebasedir+fileName2, var, FirstBinIndexFor2D, FirstBinXIndexFor2D);
  TGraphAsymmErrors* g3 = GetEffGraph(filebasedir+fileName3, var, FirstBinIndexFor2D, FirstBinXIndexFor2D);

  if(FirstBinIndexFor2D>=0){
    TGraphAsymmErrors *tmp_1 = (TGraphAsymmErrors *)g1->Clone();
    TGraphAsymmErrors *tmp_2 = (TGraphAsymmErrors *)g2->Clone();
    TGraphAsymmErrors *tmp_3 = (TGraphAsymmErrors *)g3->Clone();
    gr1_for_each_etabin.push_back( tmp_1 );
    gr2_for_each_etabin.push_back( tmp_2 );
    gr3_for_each_etabin.push_back( tmp_3 );

  }

  // -- canvas with ratio
  TString canvasName = "c_vs"+var;
  if( setZoomIn ) canvasName = canvasName + "_zoomIn";
  if(FirstBinIndexFor2D>=0) canvasName += TString::Format("_EtaBin%02d", FirstBinXIndexFor2D);
  PlotTool::GraphCanvas *canvasRatio = new PlotTool::GraphCanvas(canvasName, 0, 0);

  //==== output info
  TString outdir = "outputs/L2overOffline";
  gSystem->mkdir(outdir, kTRUE);
  canvasRatio->SetOutputDir(outdir);

  canvasRatio->Register(g1, "2016 Data", kBlue, 20);
  canvasRatio->Register(g2, "2017 Data", kRed, 21);
  canvasRatio->Register(g3, "2018 Data", kBlack, 22);

  TString titleX = "";
  if( var == "Pt" )  titleX = "p_{T}(#mu) [GeV]";
  if( var == "Eta" ) titleX = "#eta(#mu)";
  if( var == "Phi" ) titleX = "#phi(#mu)";
  if( var == "Vtx" ) titleX = "Number of reconstructed primary vertices";
  if( var == "Eta_vs_Phi" ) titleX = "#phi(#mu)";

  canvasRatio->SetTitle( titleX, "L2 / Offline efficiency", "2018/2016");
  canvasRatio->SetLegendPosition( 0.65, 0.20, 0.95, 0.33 );
  // canvasRatio->SetLegendColumn(2);

  if( var == "Pt" && setZoomIn )  canvasRatio->SetRangeX(24, 120);
  if( var == "Pt" && !setZoomIn ) canvasRatio->SetRangeX(0, 500);
  if( var == "Vtx" )              canvasRatio->SetRangeX(2.5, 60.5);

  canvasRatio->SetRangeY( 0.80  , 1.10 );
  if( var == "Pt" && !setZoomIn) canvasRatio->SetRangeY( 0.80, 1.10 );

  canvasRatio->Latex_CMSPre();
  //canvasRatio->RegisterLatex( 0.57, 0.96, "#font[42]{#scale[0.6]{10 fb^{-1} (13 TeV, 2016 and 2018)}}");
  //canvasRatio->RegisterLatex( 0.45, 0.96, "#font[42]{#scale[0.6]{5 fb^{-1} (13 TeV, 2016), 5 fb^{-1} (13 TeV, 2018)}}");
  //canvasRatio->RegisterLatex( 0.58, 0.96, "#font[42]{#scale[0.55]{5 fb^{-1} (2016), 5 fb^{-1} (2018) (13 TeV)}}");
  canvasRatio->RegisterLatex( 0.47, 0.96, "#font[42]{#scale[0.51]{5 fb^{-1} (2016), 5 fb^{-1} (2017), 5 fb^{-1} (2018) (13 TeV)}}");
  if(FirstBinIndexFor2D>=0) canvasRatio->RegisterLatex( 0.16, 0.86, "#font[42]{#scale[0.7]{"+EtabinInfo+"}}" );


  //canvasRatio->RegisterLatex( 0.60, 0.96, "#font[42]{#scale[0.6]{RunXXX (XX pb^{-1})}}");
  // canvasRatio->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{L2/L1}}");
  if( var != "Pt" )
    canvasRatio->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.7]{P_{T} > 26 GeV}}");

  canvasRatio->Ndivisions = 1003;

  canvasRatio->Draw();
}

TGraphAsymmErrors* GetEffGraph( TString fileName, TString var, int FirstBinIndexFor2D=-1, int FirstBinXIndexFor2D=-1 )
{
  MuonHLT::TnPEffTool *tool = new MuonHLT::TnPEffTool(fileName);
  
  TGraphAsymmErrors* gEff = tool->CalcTnPEff_CutAndCount( var, FirstBinIndexFor2D, FirstBinXIndexFor2D );
  return gEff;
}
