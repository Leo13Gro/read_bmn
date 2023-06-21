#define model_cxx
#include "model.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void model::Loop()
{


//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   auto Psi_prof = new TProfile("Psi_prof", "< cos 2(Psi_poz - Psi_neg) > vs b", 17, 0, 17, -100, 100);
   auto Psi_poz_prof = new TProfile("Psi_poz_prof", "< cos 2(Psi_poz - psiRP) > vs b", 17, 0, 17, -100, 100);
   auto Psi_neg_prof = new TProfile("Psi_neg_prof", "< cos 2(Psi_neg - psiRP) > vs b", 17, 0, 17, -100, 100);

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      vector<float> Q_poz = { 0, 0 };
      vector<float> Q_neg = { 0, 0 };
      for (int i_tr = 0; i_tr < nh; i_tr++)
      {
          Float_t mom = sqrt(momx[i_tr] * momx[i_tr] + momy[i_tr] * momy[i_tr] + momz[i_tr] * momz[i_tr]);
          Float_t Eta = 0.5 * log((mom + momz[i_tr]) / (mom - momz[i_tr]));
          Float_t Phi = atan2(momy[i_tr], momx[i_tr]);

          if (Eta - 1.17 > 0.05 && Eta - 1.17 < 1)
          {
              Q_poz[0] += cos(2 * Phi); Q_poz[1] += sin(2 * Phi);
          }

          if (Eta - 1.17 < -0.05 && Eta - 1.17 > -1)
          {
              Q_neg[0] += cos(2 * Phi); Q_neg[1] += sin(2 * Phi);
          }
      }

      double Psi_poz = atan2(Q_poz[1], Q_poz[0]) / 2;
      double Psi_neg = atan2(Q_neg[1], Q_neg[0]) / 2;
      double Psi_diff = cos(2 * (Psi_poz - Psi_neg));

      double Psi_poz_diff = cos(Psi_poz - phi2);
      double Psi_neg_diff = cos(Psi_neg - phi2);

      Psi_poz_prof->Fill(bimp, Psi_poz_diff);
      Psi_neg_prof->Fill(bimp, Psi_neg_diff);
      Psi_prof->Fill(bimp, Psi_diff);

   }
   auto c = new TCanvas("c", "c");

   Psi_prof->Draw();
   c->SaveAs("bmn_old_hist/Psi_prof.png");

   Psi_poz_prof->Draw();
   c->SaveAs("bmn_old_hist/Psi_poz_prof.png");
   Psi_neg_prof->Draw();
   c->SaveAs("bmn_old_hist/Psi_neg_prof.png");
}
