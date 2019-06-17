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

  TString filebasedir = "hists/";
  TString fileName1 = "IsooverMu24__Run2016.root";
  TString fileName2 = "IsooverMu24__Run2018.root";

  TGraphAsymmErrors* g1 = GetEffGraph(filebasedir+fileName1, var);
  TGraphAsymmErrors* g2 = GetEffGraph(filebasedir+fileName2, var);

  // -- canvas with ratio
  TString canvasName = "c_vs"+var;
  if( setZoomIn ) canvasName = canvasName + "_zoomIn";
  PlotTool::GraphCanvaswRatio *canvasRatio = new PlotTool::GraphCanvaswRatio(canvasName, 0, 0);

  //==== output info
  TString outdir = "outputs/IsooverMu24";
  gSystem->mkdir(outdir, kTRUE);
  canvasRatio->SetOutputDir(outdir);

  canvasRatio->Register(g1, "2016 Data", kBlack);
  canvasRatio->Register(g2, "2018 Data", kBlue);

  TString titleX = "";
  if( var == "Pt" )  titleX = "p_{T}(#mu) [GeV]";
  if( var == "Eta" ) titleX = "#eta(#mu)";
  if( var == "Phi" ) titleX = "#phi(#mu)";
  if( var == "Vtx" ) titleX = "Number of primary vertices";

  canvasRatio->SetTitle( titleX, "Isolation/L3 efficiency", "2018/2016");
  canvasRatio->SetLegendPosition( 0.60, 0.32, 0.95, 0.45 );
  // canvasRatio->SetLegendColumn(2);

  if( var == "Pt" && setZoomIn ) canvasRatio->SetRangeX(24, 500);
  if( var == "Pt" && !setZoomIn ) canvasRatio->SetRangeX(0, 500);
  if( var == "Vtx" )              canvasRatio->SetRangeX(2.5, 50.5);

  canvasRatio->SetRangeY( 0.8, 1.05 );
  // if( var == "Pt" && !setZoomIn) canvasRatio->SetRangeY( 0, 1.1 );

  canvasRatio->SetRangeRatio( 0.85, 1.15 );

  canvasRatio->Latex_CMSPre();
  canvasRatio->RegisterLatex( 0.57, 0.96, "#font[42]{#scale[0.6]{10 fb^{-1} (13 TeV, 2016 and 2018)}}");


  //canvasRatio->RegisterLatex( 0.60, 0.96, "#font[42]{#scale[0.6]{RunXXX (XX pb^{-1})}}");
  // canvasRatio->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{IsoMu24/Mu24}}");
  if( var != "Pt" )
    canvasRatio->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{P_{T} > 26 GeV}}");

  canvasRatio->Draw();
}

TGraphAsymmErrors* GetEffGraph( TString fileName, TString var )
{
  MuonHLT::TnPEffTool *tool = new MuonHLT::TnPEffTool(fileName);
  
  TGraphAsymmErrors* gEff = tool->CalcTnPEff_CutAndCount( var );
  return gEff;
}
