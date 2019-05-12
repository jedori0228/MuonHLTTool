#include <TagProbe/TnPTool.h>

void DrawHistForEachVariable(TString var, int vtxmin, int vtxmax, Bool_t setZoomIn = kFALSE);
TGraphAsymmErrors* GetEffGraph( TString fileName, TString var );

void DrawHist_Comparison_IsooverMu24_NVtxBin(int vtxmin, int vtxmax) {
  DrawHistForEachVariable( "Pt", vtxmin, vtxmax );
  DrawHistForEachVariable( "Pt", vtxmin, vtxmax, kTRUE );
  DrawHistForEachVariable( "Eta", vtxmin, vtxmax );
  DrawHistForEachVariable( "Phi", vtxmin, vtxmax );
  DrawHistForEachVariable( "Vtx", vtxmin, vtxmax );
}

void DrawHistForEachVariable(TString var, int vtxmin, int vtxmax, Bool_t setZoomIn = kFALSE)
{

  TString filebasedir = "hists/";
  TString fileName1 = "IsooverMu24NvtxBin"+TString::Itoa(vtxmin,10)+"_"+TString::Itoa(vtxmax,10)+"__Run2016.root";
  TString fileName2 = "IsooverMu24NvtxBin"+TString::Itoa(vtxmin,10)+"_"+TString::Itoa(vtxmax,10)+"__Run2018.root";

  TGraphAsymmErrors* g1 = GetEffGraph(filebasedir+fileName1, var);
  TGraphAsymmErrors* g2 = GetEffGraph(filebasedir+fileName2, var);

  // -- canvas with ratio
  TString canvasName = "c_vs"+var;
  if( setZoomIn ) canvasName = canvasName + "_zoomIn";
  PlotTool::GraphCanvaswRatio *canvasRatio = new PlotTool::GraphCanvaswRatio(canvasName, 0, 0);

  //==== output info
  TString outdir = "outputs/IsooverMu24_NVtxBin"+TString::Itoa(vtxmin,10)+"_"+TString::Itoa(vtxmax,10);
  gSystem->mkdir(outdir, kTRUE);
  canvasRatio->SetOutputDir(outdir);

  canvasRatio->Register(g1, "Run2016H", kBlack);
  canvasRatio->Register(g2, "Run2018D", kBlue);

  TString titleX = "";
  if( var == "Pt" )  titleX = "P_{T}(#mu) [GeV]";
  if( var == "Eta" ) titleX = "#eta(#mu)";
  if( var == "Phi" ) titleX = "#phi(#mu)";
  if( var == "Vtx" ) titleX = "# vtx";

  canvasRatio->SetTitle( titleX, "Efficiency", "Run2018D/Run2016H");
  canvasRatio->SetLegendPosition( 0.60, 0.32, 0.95, 0.45 );
  // canvasRatio->SetLegendColumn(2);

  if( var == "Pt" && setZoomIn ) canvasRatio->SetRangeX(24, 120);

  canvasRatio->SetRangeY( 0.8, 1.05 );
  if( var == "Pt" && !setZoomIn) canvasRatio->SetRangeY( 0, 1.1 );

  canvasRatio->SetRangeRatio( 0.85, 1.15 );

  canvasRatio->Latex_CMSPre();


  //canvasRatio->RegisterLatex( 0.60, 0.96, "#font[42]{#scale[0.6]{RunXXX (XX pb^{-1})}}");
  canvasRatio->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{IsoMu24/Mu24}}");
  if( var != "Pt" )
    canvasRatio->RegisterLatex( 0.16, 0.87, "#font[42]{#scale[0.6]{P_{T} > 26 GeV}}");
    canvasRatio->RegisterLatex( 0.16, 0.83, "#font[42]{#scale[0.6]{"+TString::Itoa(vtxmin,10)+" #leq Nvtx < "+TString::Itoa(vtxmax,10)+"}}");

  canvasRatio->Draw();
}

TGraphAsymmErrors* GetEffGraph( TString fileName, TString var )
{
  MuonHLT::TnPEffTool *tool = new MuonHLT::TnPEffTool(fileName);
  
  TGraphAsymmErrors* gEff = tool->CalcTnPEff_CutAndCount( var );
  return gEff;
}
