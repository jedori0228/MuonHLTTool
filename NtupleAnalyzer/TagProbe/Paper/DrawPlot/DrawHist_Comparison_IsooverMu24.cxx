#include <TagProbe/TnPTool.h>

void DrawHistForEachVariable(TString var, Bool_t setZoomIn = kFALSE);
TGraphAsymmErrors* GetEffGraph( TString fileName, TString var );

void DrawHist_Comparison_IsooverMu24() {
  DrawHistForEachVariable( "Pt" );
  DrawHistForEachVariable( "Pt", kTRUE );
  DrawHistForEachVariable( "Eta" );
  DrawHistForEachVariable( "Phi" );
  DrawHistForEachVariable( "Vtx" );
}

void DrawHistForEachVariable(TString var, Bool_t setZoomIn = kFALSE)
{

  TString filebasedir = "hists/Pt24Updtaes/";
  TString fileName1 = "IsooverMu24__Run2016.root";
  TString fileName2 = "IsooverMu24__Run2017.root";
  TString fileName2_2 = "IsooverMu24__Run2017_D.root";
  TString fileName3 = "IsooverMu24__Run2018.root";

  TGraphAsymmErrors* g1 = GetEffGraph(filebasedir+fileName1, var);
  TGraphAsymmErrors* g2 = GetEffGraph(filebasedir+fileName2, var);
  TGraphAsymmErrors* g2_2 = GetEffGraph(filebasedir+fileName2_2, var);
  TGraphAsymmErrors* g3 = GetEffGraph(filebasedir+fileName3, var);

  // -- canvas with ratio
  TString canvasName = "c_vs"+var;
  if( setZoomIn ) canvasName = canvasName + "_zoomIn";
  bool isLogX = false;
  if(var=="Pt") isLogX = true;
  isLogX = false;
  PlotTool::GraphCanvas *canvasRatio = new PlotTool::GraphCanvas(canvasName, isLogX, 0);

  //==== output info
  TString outdir = "outputs/IsooverMu24";
  gSystem->mkdir(outdir, kTRUE);
  canvasRatio->SetOutputDir(outdir);

  canvasRatio->Register(g1, "2016 Data", kBlue, 20);
  //canvasRatio->Register(g2, "2017 Data", kRed, 21);
  canvasRatio->Register(g2_2, "2017 Data", kRed, 21);
  canvasRatio->Register(g3, "2018 Data", kBlack, 22);

  TString titleX = "";
  if( var == "Pt" )  titleX = "p_{T}(#mu) [GeV]";
  if( var == "Eta" ) titleX = "#eta(#mu)";
  if( var == "Phi" ) titleX = "#phi(#mu)";
  if( var == "Vtx" ) titleX = "Number of reconstructed primary vertices";

  canvasRatio->SetTitle( titleX, "Isolation / L3 efficiency", "2018/2016");
  canvasRatio->SetLegendPosition( 0.50, 0.75, 0.95, 0.93 );
  // canvasRatio->SetLegendColumn(2);

  if( var == "Pt" && setZoomIn )  canvasRatio->SetRangeX(24, 120);
  if( var == "Pt" && !setZoomIn ) canvasRatio->SetRangeX(24, 500);
  if( var == "Vtx" )              canvasRatio->SetRangeX(2.5, 60.5);
  if( var == "Eta" )              canvasRatio->SetRangeX(-2.4, 2.4);
  Double_t degree15 = 3.141592 / 12.0;
  if( var == "Phi" )              canvasRatio->SetRangeX((-1)*degree15*12, degree15*12);

  canvasRatio->SetRangeY( 0.88  , 1.04 );
  //if( var == "Pt" && !setZoomIn) canvasRatio->SetRangeY( 0.93, 1.01 );

  canvasRatio->Latex_CMSPre();
  //canvasRatio->RegisterLatex( 0.57, 0.96, "#font[42]{#scale[0.6]{10 fb^{-1} (13 TeV, 2016 and 2018)}}");
  canvasRatio->RegisterLatex( 0.47, 0.96, "#font[42]{#scale[0.51]{5 fb^{-1} (2016), 4 fb^{-1} (2017), 5 fb^{-1} (2018) (13 TeV)}}");


  //canvasRatio->RegisterLatex( 0.60, 0.96, "#font[42]{#scale[0.6]{RunXXX (XX pb^{-1})}}");
  // canvasRatio->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{IsoMu24/Mu24}}");
  if( var != "Pt" )
    canvasRatio->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.7]{P_{T} > 24 GeV}}");

  canvasRatio->Draw();
}

TGraphAsymmErrors* GetEffGraph( TString fileName, TString var )
{
  MuonHLT::TnPEffTool *tool = new MuonHLT::TnPEffTool(fileName);
  
  TGraphAsymmErrors* gEff = tool->CalcTnPEff_CutAndCount( var );
  return gEff;
}
