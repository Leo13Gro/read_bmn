#include <iostream>
#include <fstream>
#include <vector>

#include <Rtypes.h>
#include <TString.h>
#include <TChain.h>
#include <TVector3.h>
#include <TMath.h>
#include "TH1F.h"
#include "TFile.h"
#include "TMath.h"
#include <cmath>
#include "Math/IFunction.h"
#include <Math/SpecFuncMathMore.h>

#include "hist_data.C"

using namespace ROOT; 


// Resolution calculation
double GetRes(double _chi, double _harm){
  double con = TMath::Sqrt(TMath::Pi() / 2) / 2;
  double arg = _chi * _chi / 4.;
  double order1 = (_harm - 1) / 2.;
  double order2 = (_harm + 1) / 2.;
  double res = con * _chi * exp(-arg) * (ROOT::Math::cyl_bessel_i(order1, arg) + ROOT::Math::cyl_bessel_i(order2, arg));
  return res;
}


// Chi2 calculation
double GetChi(double _res, double _harm, int accuracy){
  double chi = 2.0;
  double delta = 1.0;
  for (int i = 0; i < accuracy; i++){
    if(GetRes(chi, _harm) < _res) chi = chi + delta;
    else chi = chi - delta;
    delta = delta / 2.;
  }
  return chi;
}

void read_bmn(/* TString FileList, TString OutFileName */){
	
	TChain *inchain = new TChain("t");
	inchain->Add("/mnt/pool/nica/8/parfenovpeter/bmnsim/ogolosov/xecs/dcm_4agev/1.tree.root");
	RDataFrame datafr(*inchain);
	
	
	std::vector<hist_data> Hist_bmn = {
		{"b", 100, {0, 15}, datafr},
		{"trMom.fCoordinates.fEta", 100, {0, 6}, datafr},
		{"trMom.fCoordinates.fPhi", 50, {0, 3.2}, datafr},
		{"trMom.fCoordinates.fPt", 50, {0, 3}, datafr},
		{"simMom.fCoordinates.fEta", 50, {-10, 10}, datafr},
		{"simMom.fCoordinates.fPhi", 50, {0, 3.2}, datafr},
		{"simMom.fCoordinates.fPt", 50, {0, 3}, datafr}
	};
	
	TCanvas* c = new TCanvas("c", "c");
	
	for(auto& it : Hist_bmn)
	{
		it.Draw_png(c);
	}
	
/* 	//				================================Запись в файл======================================
	TFile *f = new TFile(OutFileName.Data(), "recreate");
	
	h->Write(); */
}