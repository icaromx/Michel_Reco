// Author: Ivan Caro Terrazas
// Date: 13/August/2018

////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "TROOT.h"
#include "TFile.h"
#include "TNetFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TStopwatch.h"
#include "TLine.h"
#include "TString.h"
#include "TStyle.h"

#include "mymaths.h"
#include "cluster_tools.h"
#include "pca.h"

#define PI 3.14159
#define euler 2.71828
#define cal_factor 0.0000385

using namespace std;

/////////////////////////////////////////////
int main(int argc, char **argv){

  //----Parameters----//

  //----Output file----//
  string parent = "Stopping_Mu_Sample/";
  DIR* dir = opendir(parent.c_str());
  if (ENOENT == errno){ /* Directory does not exists. */
    const int dir_err = mkdir(parent.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
  TString pre_outfile = (TString) parent + "mu_sel_" ;

  //----Specifying root file to make---//
  TFile *f_output;
  TString f_name = "test.root";
  //TString f_name = Form(pre_outfile + "Run_%d_Event_%d.root", event.rn(), event.ev());
  f_output = TFile::Open(f_name,"RECREATE");

  //----Filelist to use----//
  std::ifstream filelist(argv[1]);
  std::string rootfile;

  //----TTree to store results----//
  TTree *tclst = new TTree("T_charge_cluster","No flash cuts");
  double tclst_rn, tclst_ev, tclst_cn, tclst_sz, tclst_xcn;
  double tclst_x, tclst_y, tclst_z, tclst_q;
  double tclst_uq, tclst_vq, tclst_wq;
  tclst->Branch("run_num",&tclst_rn); tclst->Branch("ev_num",&tclst_ev);
  tclst->Branch("cluster_id",&tclst_cn); tclst->Branch("size",&tclst_sz);
  tclst->Branch("xcn",&tclst_xcn);
  tclst->Branch("x",&tclst_x); tclst->Branch("y",&tclst_y); tclst->Branch("z",&tclst_z);
  tclst->Branch("q",&tclst_q); tclst->Branch("uq",&tclst_uq);
  tclst->Branch("vq",&tclst_vq); tclst->Branch("wq",&tclst_wq);

  TTree *tsel_clsts = new TTree("T_Clusters","Selected clusters for sample");
  double tsel_clsts_rn, tsel_clsts_ev, tsel_clsts_cn, tsel_clsts_sz, tsel_clsts_xcn;
  tsel_clsts->Branch("run_num",&tsel_clsts_rn); tsel_clsts->Branch("ev_num",&tsel_clsts_ev);
  tsel_clsts->Branch("cluster_id",&tsel_clsts_cn); tsel_clsts->Branch("size",&tsel_clsts_sz);
  tsel_clsts->Branch("xcn",&tsel_clsts_xcn);

  while(std::getline(filelist, rootfile)){ //Looping over events
    EventHandler event;
    event.load_event(rootfile);
    std::vector<int> clusters = event.wc_clusters();
    WCClst all_clsts = event.Get_All_Tracks();

    WCClstManager clsts;
    clsts.load_tracks(all_clsts, clusters);
    WCClstBundle EventClsts;
    EventClsts = clsts.Get_Clusters();

    for(WCClst &cl : EventClsts){

    }

    f_output->Write();
    f_output->Close();
    cout << Form("Done with event %d", event.ev()) << endl;
  }
  std::cout << "DONE" << '\n';

}