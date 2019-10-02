#include <TagProbe/TnPTool.h>

void DrawHistForEachVariable(TString var, Bool_t setZoomIn = kFALSE);
TGraphAsymmErrors* GetEffGraph( TString fileName, TString var );

void DrawHist_Comparison_L2overL1() {
  DrawHistForEachVariable( "Pt" );
  DrawHistForEachVariable( "Pt", kTRUE );
  DrawHistForEachVariable( "Eta" );
  DrawHistForEachVariable( "Phi" );
  DrawHistForEachVariable( "Vtx" );
}

void DrawHistForEachVariable(TString var, Bool_t setZoomIn = kFALSE)
{

  TString filebasedir = "hists/";

  TString fileName1 = "L2overL1_NoL2PtCut__Run2016.root";
  TString fileName2 = "L2overL1_NoL2PtCut__Run2018.root";

  TGraphAsymmErrors* g1 = GetEffGraph(filebasedir+fileName1, var);
  TGraphAsymmErrors* g2 = GetEffGraph(filebasedir+fileName2, var);

  // -- canvas with ratio
  TString canvasName = "c_vs"+var;
  if( setZoomIn ) canvasName = canvasName + "_zoomIn";
  PlotTool::GraphCanvas *canvasRatio = new PlotTool::GraphCanvas(canvasName, 0, 0);

  //==== output info
  TString outdir = "outputs/L2overL1";
  gSystem->mkdir(outdir, kTRUE);
  canvasRatio->SetOutputDir(outdir);

  canvasRatio->Register(g1, "2016 Data", kBlack);
  canvasRatio->Register(g2, "2018 Data", kBlue);

  TString titleX = "";
  if( var == "Pt" )  titleX = "p_{T}(#mu) [GeV]";
  if( var == "Eta" ) titleX = "#eta(#mu)";
  if( var == "Phi" ) titleX = "#phi(#mu)";
  if( var == "Vtx" ) titleX = "Number of reconstructed primary vertices";

  canvasRatio->SetTitle( titleX, "L2 / L1 efficiency", "2018/2016");
  canvasRatio->SetLegendPosition( 0.60, 0.32, 0.95, 0.45 );
  // canvasRatio->SetLegendColumn(2);

  if( var == "Pt" && setZoomIn )  canvasRatio->SetRangeX(24, 120);
  if( var == "Pt" && !setZoomIn ) canvasRatio->SetRangeX(0, 500);
  if( var == "Vtx" )              canvasRatio->SetRangeX(2.5, 60.5);

  canvasRatio->SetRangeY( 0.98, 1.01 );
  if( var == "Pt" && !setZoomIn) canvasRatio->SetRangeY( 0.93, 1.01 );

  canvasRatio->Latex_CMSPre();
  //canvasRatio->RegisterLatex( 0.57, 0.96, "#font[42]{#scale[0.6]{10 fb^{-1} (13 TeV, 2016 and 2018)}}");
  //canvasRatio->RegisterLatex( 0.45, 0.96, "#font[42]{#scale[0.6]{5 fb^{-1} (13 TeV, 2016), 5 fb^{-1} (13 TeV, 2018)}}");
  canvasRatio->RegisterLatex( 0.58, 0.96, "#font[42]{#scale[0.55]{5 fb^{-1} (2016), 5 fb^{-1} (2018) (13 TeV)}}");


  //canvasRatio->RegisterLatex( 0.60, 0.96, "#font[42]{#scale[0.6]{RunXXX (XX pb^{-1})}}");
  // canvasRatio->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{L2/L1}}");
  if( var != "Pt" )
    canvasRatio->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.7]{P_{T} > 26 GeV}}");

  canvasRatio->Ndivisions = 1003;

  canvasRatio->Draw();
}

TGraphAsymmErrors* GetEffGraph( TString fileName, TString var )
{
  MuonHLT::TnPEffTool *tool = new MuonHLT::TnPEffTool(fileName);
  
  TGraphAsymmErrors* gEff = tool->CalcTnPEff_CutAndCount( var );
  return gEff;
}
