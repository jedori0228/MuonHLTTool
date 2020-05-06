#ifndef mylib_h
#define mylib_h

TGraphAsymmErrors* hist_to_graph(TH1D* hist, bool YErrorZero=false){

  TH1::SetDefaultSumw2(true);

  int Nbins = hist->GetXaxis()->GetNbins();
  double x[Nbins], y[Nbins], xlow[Nbins], xup[Nbins], ylow[Nbins], yup[Nbins];
  TAxis *xaxis = hist->GetXaxis();
  for(Int_t i=0; i<Nbins; i++){
    x[i] = xaxis->GetBinCenter(i+1);
    y[i] = hist->GetBinContent(i+1);
    xlow[i] = xaxis->GetBinCenter(i+1)-xaxis->GetBinLowEdge(i+1);
    xup[i] = xaxis->GetBinUpEdge(i+1)-xaxis->GetBinCenter(i+1);
    ylow[i] = hist->GetBinError(i+1);
    yup[i] = hist->GetBinError(i+1);
    if(YErrorZero){
      ylow[i] = 0;
      yup[i] = 0;
    }
    //ylow[i] = 0;
    //yup[i] = 0;
    //cout << "x = " << x[i] << ", y = " << y[i] << ", x_low = " << xlow[i] << ", xup = " << xup[i] << ", ylow = " << ylow[i] << ", yup = " << yup[i] << endl;
  }
  TGraphAsymmErrors *out = new TGraphAsymmErrors(Nbins, x, y, xlow, xup, ylow, yup);
  out->SetTitle("");
  return out;

}

TGraphAsymmErrors* hist_to_graph(TH1D* hist, int n_skip_x_left){

  TH1::SetDefaultSumw2(true);

  int Nbins = hist->GetXaxis()->GetNbins()-n_skip_x_left;
  double x[Nbins], y[Nbins], xlow[Nbins], xup[Nbins], ylow[Nbins], yup[Nbins];
  TAxis *xaxis = hist->GetXaxis();
  for(Int_t i=1; i<=Nbins; i++){
    x[i-1] = xaxis->GetBinCenter(i+n_skip_x_left);
    y[i-1] = hist->GetBinContent(i+n_skip_x_left);
    xlow[i-1] = xaxis->GetBinCenter(i+n_skip_x_left)-xaxis->GetBinLowEdge(i+n_skip_x_left);
    xup[i-1] = xaxis->GetBinUpEdge(i+n_skip_x_left)-xaxis->GetBinCenter(i+n_skip_x_left);
    ylow[i-1] = hist->GetBinError(i+n_skip_x_left);
    yup[i-1] = hist->GetBinError(i+n_skip_x_left);
    //ylow[i-1] = 0;
    //yup[i-1] = 0;
    //cout << "x = " << x[i-1] << ", y = " << y[i-1] << ", x_low = " << xlow[i-1] << ", xup = " << xup[i-1] << ", ylow = " << ylow[i-1] << ", yup = " << yup[i-1] << endl;
  }
  TGraphAsymmErrors *out = new TGraphAsymmErrors(Nbins, x, y, xlow, xup, ylow, yup);
  out->SetTitle("");
  return out;

}

TGraphAsymmErrors* hist_to_graph(TH1D* hist, int n_skip_x_left, int n_x_shift, int i_x_shift){

  TH1::SetDefaultSumw2(true);

  int Nbins = hist->GetXaxis()->GetNbins()-n_skip_x_left;
  double x[Nbins], y[Nbins], xlow[Nbins], xup[Nbins], ylow[Nbins], yup[Nbins];
  TAxis *xaxis = hist->GetXaxis();
  for(Int_t i=1; i<=Nbins; i++){
    x[i-1] = xaxis->GetBinCenter(i+n_skip_x_left);
    y[i-1] = hist->GetBinContent(i+n_skip_x_left);
    xlow[i-1] = xaxis->GetBinCenter(i+n_skip_x_left)-xaxis->GetBinLowEdge(i+n_skip_x_left);
    xup[i-1] = xaxis->GetBinUpEdge(i+n_skip_x_left)-xaxis->GetBinCenter(i+n_skip_x_left);
    ylow[i-1] = hist->GetBinError(i+n_skip_x_left);
    yup[i-1] = hist->GetBinError(i+n_skip_x_left);

    double dx = (xaxis->GetBinUpEdge(i+n_skip_x_left)-xaxis->GetBinLowEdge(i+n_skip_x_left))/double(n_x_shift+1);
    x[i-1] = xaxis->GetBinLowEdge(i+n_skip_x_left) + double(i_x_shift+1) * dx;
    xlow[i-1] = double(i_x_shift+1) * dx;
    xup[i-1] = xaxis->GetBinUpEdge(i+n_skip_x_left)-x[i-1];

    //ylow[i-1] = 0;
    //yup[i-1] = 0;
    //cout << "x = " << x[i-1] << ", y = " << y[i-1] << ", x_low = " << xlow[i-1] << ", xup = " << xup[i-1] << ", ylow = " << ylow[i-1] << ", yup = " << yup[i-1] << endl;
  }
  TGraphAsymmErrors *out = new TGraphAsymmErrors(Nbins, x, y, xlow, xup, ylow, yup);
  out->SetTitle("");
  return out;

}


TGraphAsymmErrors* GraphSubtract(TGraphAsymmErrors *a, TGraphAsymmErrors *b, bool Rel){

  //==== do a-b

  int NX = a->GetN();
  TGraphAsymmErrors* gr_out = (TGraphAsymmErrors*)a->Clone();

  for(int i=0; i<NX; i++){

    double a_x, a_y, b_x, b_y;

    a->GetPoint(i, a_x, a_y);
    b->GetPoint(i, b_x, b_y);

    if(Rel==true){
      gr_out->SetPoint( i, a_x, (a_y-b_y)/b_y );
    }
    else{
      gr_out->SetPoint( i, a_x, a_y-b_y );
    }

  }

  return gr_out;

}

void RemoveLargeError(TGraphAsymmErrors *a){

  int NX = a->GetN();

  for(int i=0; i<NX; i++){

    double x, y, yerr_low, yerr_high;

    a->GetPoint(i, x, y);
    yerr_low  = a->GetErrorYlow(i);
    yerr_high = a->GetErrorYhigh(i);

    if(y+yerr_high==1.){
      a->SetPointEYhigh( i, yerr_low );
    }

  }

}

void ScaleGraph(TGraphAsymmErrors *a, double c){

  int NX = a->GetN();

  for(int i=0; i<NX; i++){

    double x, y, yerr_low, yerr_high;

    a->GetPoint(i, x, y);
    yerr_low  = a->GetErrorYlow(i);
    yerr_high = a->GetErrorYhigh(i);

    a->SetPoint(i, x, c*y);
    a->SetPointEYlow(i, c*yerr_low);
    a->SetPointEYhigh(i, c*yerr_high);

  }

}

double GetMaximum(TH1F* hist, double ErrorScale=1.){

  TAxis *xaxis = hist->GetXaxis();

  double maxval(-1.);
  for(int i=1; i<=xaxis->GetNbins(); i++){
    if( hist->GetBinContent(i) + ErrorScale*hist->GetBinError(i) > maxval ){
      maxval = hist->GetBinContent(i) + ErrorScale*hist->GetBinError(i);
    }
  }

  return maxval;

}

double GetMaximum(TH1D* hist, double ErrorScale=1.){

  TAxis *xaxis = hist->GetXaxis();

  double maxval(-1.);
  for(int i=1; i<=xaxis->GetNbins(); i++){
    if( hist->GetBinContent(i) + ErrorScale*hist->GetBinError(i) > maxval ){
      maxval = hist->GetBinContent(i) + ErrorScale*hist->GetBinError(i);
    }
  }

  return maxval;

}

double GetMaximum(TGraphAsymmErrors *a, double ErrorScale=1.){

  int NX = a->GetN();

  double maxval(-9999.);
  for(int i=0; i<NX; i++){

    double x, y, yerr_low, yerr_high;

    a->GetPoint(i, x, y);
    yerr_low  = a->GetErrorYlow(i);
    yerr_high = a->GetErrorYhigh(i);

    if( y+ErrorScale*yerr_high > maxval ){
      maxval = y+ErrorScale*yerr_high;
    }

  }

  return maxval;

}

double GetMinimum(TH1D* hist, double ErrorScale=1.){

  TAxis *xaxis = hist->GetXaxis();

  double minval(99999999);
  for(int i=1; i<=xaxis->GetNbins(); i++){
    if( hist->GetBinContent(i) == 0 ) continue;
    if( hist->GetBinContent(i) + ErrorScale*hist->GetBinError(i) < minval ){
      minval = hist->GetBinContent(i) - ErrorScale*hist->GetBinError(i);
    }
  }

  return minval;

}

double GetMinimum(TGraphAsymmErrors *a, double ErrorScale=1.){

  int NX = a->GetN();

  double minval(99999999);
  for(int i=0; i<NX; i++){

    double x, y, yerr_low, yerr_high;

    a->GetPoint(i, x, y);
    yerr_low  = a->GetErrorYlow(i);
    yerr_high = a->GetErrorYhigh(i);

    if( y == 0 ) continue;
    if( y-ErrorScale*yerr_low < minval ){
      minval = y-ErrorScale*yerr_low;
    }

  }

  return minval;

}

double GetYieldSystematics(TH1D *hist){

  int n_syst = hist->GetXaxis()->GetNbins();
  int n_source = (n_syst-1)/2;

  //==== Bin index
  //==== i=1 : central
  //==== i=2 : _MuonEn_up
  //==== i=3 : _MuonEn_down
  //==== -> n_syst = 3
  //==== -> n_source = (n_syst-1)/2 = (3-1)/2 = 1

  double y_central = hist->GetBinContent(1);

  double sum_syst = 0.;
  for(int i=1; i<=n_source; i++){
    double yield_up = hist->GetBinContent(i*2);
    double yield_down = hist->GetBinContent(i*2+1);

    double syst_up = fabs(yield_up-y_central);
    double syst_down = fabs(yield_down-y_central);

    sum_syst += 0.5*(syst_up*syst_up+syst_down*syst_down);

  }

  return sqrt(sum_syst);

}

TDirectory *MakeTemporaryDirectory(){

  gROOT->cd();
  TDirectory* tempDir = 0;
  int counter = 0;
  while (not tempDir) {
    // First, let's find a directory name that doesn't exist yet:
    std::stringstream dirname;
    dirname << "HNCommonLeptonFakes_%i" << counter;
    if (gROOT->GetDirectory((dirname.str()).c_str())) {
      ++counter;
      continue;
    }
    // Let's try to make this directory:
    tempDir = gROOT->mkdir((dirname.str()).c_str());

  }

  return tempDir;

}

void AddPhantomZero(double a, TString align, int digit_int, int digit_frac){

  if(align=="r"){
    int number_maxdigit = 0;
    for(int i=10; i>=0; i--){
      if(a/pow(10,i)>=1.){
        number_maxdigit = i;
        break;
      }
    }
    //cout << "number_maxdigit = " << number_maxdigit << endl;
    for(int i=0; i<digit_int-(number_maxdigit+1); i++) cout << "\\phantom{0}";
    cout << std::fixed<<std::setprecision(digit_frac) << a;
  }

  else if(align=="l"){
    int target_total_digit = digit_int+digit_frac;
    int number_maxdigit = 0;
    for(int i=10; i>=0; i--){
      if(a/pow(10,i)>=1.){
        number_maxdigit = i;
        break;
      }
    }
    //cout << "target_total_digit = " << target_total_digit << endl;
    //cout << "number_maxdigit = " << number_maxdigit << endl;
    //cout << "--> # of \\phantom{0} = " << target_total_digit-digit_int-(number_maxdigit+1) << endl;

    cout << std::fixed<<std::setprecision(digit_frac) << a;
    for(int i=0; i<target_total_digit-(number_maxdigit+1)-digit_frac; i++) cout << "\\phantom{0}";
  }

}

bool IsSignalCATSamaple(TString samplename){

  if(samplename.Contains("HNpair")) return true;

  return false;

}

TH1D* DivideHistograms(TH1D *hist_Den, TH1D *hist_Num){

  TH1D *histout = (TH1D *)hist_Num->Clone();

  for(int ix=1; ix<=hist_Num->GetXaxis()->GetNbins(); ix++){

    double this_Den    = hist_Den->GetBinContent(ix);
    double this_DenErr = hist_Den->GetBinError(ix);
    double this_DenRelErr = this_DenErr/this_Den;

    double this_Num    = hist_Num->GetBinContent(ix);
    double this_NumErr = hist_Num->GetBinError(ix);
    double this_NumRelErr = this_NumErr/this_Num;

    double this_out = 0.;
    double this_outerr = 0.;
    if(this_Den!=0&&this_Num!=0){
      this_out = this_Num/this_Den;
      this_outerr = sqrt(this_DenRelErr*this_DenRelErr+this_NumRelErr*this_NumRelErr)*this_out;
    }

    histout->SetBinContent(ix, this_out);
    histout->SetBinError(ix, this_outerr);

  }

  return histout;

}


TH1D *GetStatUpDown(TH1D *hist, int sys){

  TH1D *out = (TH1D *)hist->Clone();
  for(int i=1; i<=hist->GetXaxis()->GetNbins(); i++){

    double y = hist->GetBinContent(i);
    double e = hist->GetBinError(i);

    out->SetBinContent(i, y + (double)sys * e );

  }
  return out;

}

TH1D *GetScaleUpDown(TH1D *hist, double sys){

  TH1D *out = (TH1D *)hist->Clone();
  for(int i=1; i<=hist->GetXaxis()->GetNbins(); i++){

    double y = hist->GetBinContent(i);

    out->SetBinContent(i, y + sys*y );

  }
  return out;

}

double GetDYNormSF(int DataYear, TString channel, bool geterror=false){

  double DYNorm = 1.;
  double DYNorm_err = 0.;

  int int_channel(-1); // 0 : ee, 1 : mm
  if(channel.Contains("Electron")) int_channel = 0;
  else if(channel.Contains("Muon")) int_channel = 1;
  else{
    cout << "(mylib.h) [GetDYNormSF()] : Wrong channel name; " << channel << endl;
    exit(EXIT_FAILURE);
  }

  if(DataYear==2016){
    if(int_channel==0){
      DYNorm = 0.975387;
      DYNorm_err = 0.0213523;
    }
    else if(int_channel==1){
      DYNorm = 0.942031;
      DYNorm_err = 0.0203093;
    }
    else{
      cout << "Wrong DY Norm" << endl;
      exit(EXIT_FAILURE);
    }
  }
  else if(DataYear==2017){
    if(int_channel==0){
      DYNorm = 0.969739;
      DYNorm_err = 0.0207846;
    }
    else if(int_channel==1){
      DYNorm = 0.980255;
      DYNorm_err = 0.0208141;
    }
    else{
      cout << "Wrong DY Norm" << endl;
      exit(EXIT_FAILURE);
    }
  }
  else if(DataYear==2018){
    if(int_channel==0){
      DYNorm = 1.04037;
      DYNorm_err = 0.0219748;
    }
    else if(int_channel==1){
      DYNorm = 1.05012;
      DYNorm_err = 0.0220453;
    }
    else{
      cout << "Wrong DY Norm" << endl;
      exit(EXIT_FAILURE);
    }
  }

  if(geterror) return DYNorm_err;
  else return DYNorm;

}

void SetErrorZero(TH1D *hist){

  for(int i=0; i<=hist->GetXaxis()->GetNbins()+1; i++){
    hist->SetBinError(i,0);
  }

}

void EmptyHistogram(TH1D *hist){

  for(int i=0; i<=hist->GetXaxis()->GetNbins()+1; i++){
    hist->SetBinContent(i,0);
    hist->SetBinError(i,0);
  }

}

vector<TH1D *> ConvertSystematic(TH1D *hist_Central, TH1D *hist_Up, TH1D *hist_Down){

  TH1D *hist_UpMax = (TH1D *)hist_Central->Clone();
  TH1D *hist_DownMax = (TH1D *)hist_Central->Clone();

  for(int i=0; i<=hist_Central->GetXaxis()->GetNbins()+1; i++){

    double y_Central = hist_Central->GetBinContent(i);
    double y_Up = hist_Up->GetBinContent(i);
    double y_Down = hist_Down->GetBinContent(i);

    double y_Max = max( y_Central, max(y_Up, y_Down) );
    double y_Min = min( y_Central, min(y_Up, y_Down) );

/*
    cout << "##############################################" << endl;
    cout << "[ConvertSystematic] y_Central = " << y_Central << endl;
    cout << "[ConvertSystematic] y_Up = " << y_Up << endl;
    cout << "[ConvertSystematic] y_Down = " << y_Down << endl;
    cout << "[ConvertSystematic] y_Max = " << y_Max << endl;
    cout << "[ConvertSystematic] y_Min = " << y_Min << endl;
*/

    double err_UpMax   = fabs( y_Central - y_Max );
    double err_DownMax = fabs( y_Central - y_Min );

    hist_UpMax->SetBinError(i, err_UpMax);
    hist_DownMax->SetBinError(i, err_DownMax);

  }

  vector<TH1D *> out = {hist_UpMax, hist_DownMax};
  return out;

}

void AddSystematic(TH1D *hist_Central, TH1D *hist_Syst){

  for(int i=0; i<=hist_Central->GetXaxis()->GetNbins()+1; i++){

    double y_Central = hist_Central->GetBinContent(i);
    double y_Syst = hist_Syst->GetBinContent(i);

    double err_Central = hist_Central->GetBinError(i);
    double err_Syst = hist_Syst->GetBinError(i);

    if(y_Central!=y_Syst){
      cout << "[AddSystematic] Adding syst, but central is different WTF" << endl;
      cout << "y_Central= " << y_Central << ", err_Central = " << err_Central << endl;
      cout << "y_Syst = " << y_Syst << ", err_Syst = " << err_Syst << endl;
      exit(EXIT_FAILURE);
    }

    double sumerr = sqrt( err_Central*err_Central + err_Syst*err_Syst );

    hist_Central->SetBinError(i, sumerr);

  }

}

TGraphAsymmErrors* GetAsymmError(TH1D *MC_stacked_allerr_Up, TH1D *MC_stacked_allerr_Down){

  int NBin = MC_stacked_allerr_Up->GetXaxis()->GetNbins();
  double x[NBin], x_lerr[NBin], x_rerr[NBin];
  double y[NBin], y_lerr[NBin], y_rerr[NBin];

  for(int i=0; i<NBin; i++){

    x[i] = MC_stacked_allerr_Up->GetXaxis()->GetBinCenter(i+1);
    x_lerr[i] = x[i] - MC_stacked_allerr_Up->GetXaxis()->GetBinLowEdge(i+1);
    x_rerr[i] = MC_stacked_allerr_Up->GetXaxis()->GetBinUpEdge(i+1) - x[i];

    y[i] = MC_stacked_allerr_Up->GetBinContent(i+1);
    y_lerr[i] = MC_stacked_allerr_Down->GetBinError(i+1);
    y_rerr[i] = MC_stacked_allerr_Up->GetBinError(i+1);

  }

  TGraphAsymmErrors *out = new TGraphAsymmErrors(NBin+1, x, y, x_lerr, x_rerr, y_lerr, y_rerr);
  return out;

}

//==== FIXME TEMP FUNCTION

double Get2018DataSurvFrac(TString channel){

  int int_channel(-1); // 0 : ee, 1 : mm
  if(channel.Contains("Electron")) int_channel = 0;
  else if(channel.Contains("Muon")) int_channel = 1;
  else{
    cout << "(mylib.h) [Get2018DataSurvFrac()] : Wrong channel name; " << channel << endl;
    exit(EXIT_FAILURE);
  }

	double dataSurv = 1.;
	if(int_channel==0){
		dataSurv = 1.-0.0211;
	}
	else if(int_channel==1){
		dataSurv = 1.-0.0331;
	}
  else{
    cout << "(mylib.h) [Get2018DataSurvFrac()] : Wrong channel ; " << channel << endl;
    exit(EXIT_FAILURE);
  }

  return dataSurv;

}

#endif
