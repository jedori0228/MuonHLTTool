#include <TagProbe/TnPTool.h>

void DrawHistForEachVariable(TString var, Bool_t setZoomIn = kFALSE);
TGraphAsymmErrors* GetEffGraph( TString fileName, TString var );

int i_isotype = 0;
int i_etaregion = 0;

void DrawHist_Comparison_EachIsooverMu24(int a=0, int b=0) {

  i_isotype = a;
  i_etaregion = b;

  DrawHistForEachVariable( "Pt" );
  DrawHistForEachVariable( "Pt", kTRUE );
  DrawHistForEachVariable( "Eta" );
  DrawHistForEachVariable( "Phi" );
  DrawHistForEachVariable( "Vtx" );
}

void DrawHistForEachVariable(TString var, Bool_t setZoomIn = kFALSE)
{

  TString filebasedir = "hists/Pt24Updtaes/";

  TString IsoType = "ECALIsoOverMu24";
  TString ytitle = "ECAL iso / Mu24";
  if(i_isotype==1){
    IsoType = "HCALIsoOverMu24";
    ytitle = "HCAL iso / Mu24";
  }
  if(i_isotype==2){
    IsoType = "TrackerIsoOverMu24";
    ytitle = "Tracker iso / Mu24";
  }
  if(i_isotype==3){
    IsoType = "HCALoverECAL";
    ytitle = "HCAL / ECAL";
  }
  if(i_isotype==4){
    IsoType = "TrackeroverHCAL";
    ytitle = "Tracker / HCAL";
  }

  TString EtaRegion = "EtaInclusive";
  if(i_etaregion==1) EtaRegion = "Barrel";
  if(i_etaregion==2) EtaRegion = "EndCap";

  TString fileName1 = IsoType+"_"+EtaRegion+"__Run2016.root";
  TString fileName2 = IsoType+"_"+EtaRegion+"__Run2017.root";
  TString fileName2_D = IsoType+"_"+EtaRegion+"__Run2017_D.root";
  TString fileName3 = IsoType+"_"+EtaRegion+"__Run2018.root";

  TGraphAsymmErrors* g1 = GetEffGraph(filebasedir+fileName1, var);
  TGraphAsymmErrors* g2 = GetEffGraph(filebasedir+fileName2, var);
  TGraphAsymmErrors* g2_D = GetEffGraph(filebasedir+fileName2_D, var);
  TGraphAsymmErrors* g3 = GetEffGraph(filebasedir+fileName3, var);

  // -- canvas with ratio
  TString canvasName = "c_vs"+var;
  if( setZoomIn ) canvasName = canvasName + "_zoomIn";
  PlotTool::GraphCanvas *canvasRatio = new PlotTool::GraphCanvas(canvasName, 0, 0);

  //==== output info
  TString outdir = "outputs/Pt24Updtaes/"+IsoType+"_"+EtaRegion;
  gSystem->mkdir(outdir, kTRUE);
  canvasRatio->SetOutputDir(outdir);

  canvasRatio->Register(g1, "2016 Data", kBlue, 20);
  canvasRatio->Register(g2_D, "2017 Data (D)", kRed+2, 21);
  canvasRatio->Register(g2, "2017 Data (F)", kRed, 21);
  canvasRatio->Register(g3, "2018 Data", kBlack, 22);

  TString titleX = "";
  if( var == "Pt" )  titleX = "p_{T}(#mu) [GeV]";
  if( var == "Eta" ) titleX = "#eta(#mu)";
  if( var == "Phi" ) titleX = "#phi(#mu)";
  if( var == "Vtx" ) titleX = "Number of reconstructed primary vertices";

  canvasRatio->SetTitle( titleX, ytitle+" efficiency", "2018/2016");
  canvasRatio->SetLegendPosition( 0.65, 0.20, 0.95, 0.33 );
  // canvasRatio->SetLegendColumn(2);

  if( var == "Pt" && setZoomIn ) canvasRatio->SetRangeX(24, 500);
  if( var == "Pt" && !setZoomIn ) canvasRatio->SetRangeX(24, 500);
  if( var == "Vtx" )              canvasRatio->SetRangeX(2.5, 60.5);

  canvasRatio->SetRangeY( 0.90, 1.03 );
  // if( var == "Pt" && !setZoomIn) canvasRatio->SetRangeY( 0, 1.1 );

  canvasRatio->Latex_CMSPre();
  //canvasRatio->RegisterLatex( 0.57, 0.96, "#font[42]{#scale[0.6]{10 fb^{-1} (13 TeV, 2016 and 2018)}}");
  canvasRatio->RegisterLatex( 0.47, 0.96, "#font[42]{#scale[0.51]{5 fb^{-1} (2016), 5 fb^{-1} (2017), 5 fb^{-1} (2018) (13 TeV)}}");


  //canvasRatio->RegisterLatex( 0.60, 0.96, "#font[42]{#scale[0.6]{RunXXX (XX pb^{-1})}}");
  // canvasRatio->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.6]{IsoMu24/Mu24}}");
  if( var != "Pt" )
    canvasRatio->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.7]{P_{T} > 24 GeV, "+EtaRegion+"}}");
  else
    canvasRatio->RegisterLatex( 0.16, 0.91, "#font[42]{#scale[0.7]{"+EtaRegion+"}}");

  canvasRatio->Draw();
}

TGraphAsymmErrors* GetEffGraph( TString fileName, TString var )
{
  MuonHLT::TnPEffTool *tool = new MuonHLT::TnPEffTool(fileName);
  
  TGraphAsymmErrors* gEff = tool->CalcTnPEff_CutAndCount( var );
  return gEff;
}
