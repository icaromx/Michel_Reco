// Author: Ivan Caro Terrazas
// Date: 08/August/2018

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

using namespace std;

/////////////////////////////////////////////
int main(int argc, char **argv){

  //----Cut Parameters----//
  double vdrift = 0.1101; //#cm/us CONSTANT
  double pe_threshold = 10;
  double x_anode = 0.0;
  double x_cathode = 256.4;
  double x_anode_th = 3.0;
  double x_cathode_th = 4.0;


  //----Output file----//
  string parent = "Stopping_Mu_Sample/";
  DIR* dir = opendir(parent.c_str());
  if (ENOENT == errno){ /* Directory does not exists. */
    const int dir_err = mkdir(parent.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
  TString pre_outfile = (TString) parent + "mu_sel_" ;
  //----Filelist to use----//
  std::ifstream filelist(argv[1]);
  std::string rootfile;

  while(std::getline(filelist, rootfile)){ //Looping over events
    EventHandler event;
    event.load_event(rootfile);
    if(event.GetEntries() == 0) continue;
    std::vector<int> clusters = event.wc_clusters();
    WCClst all_clsts = event.Get_All_Tracks();

    std::cout << Form("Looking at Run %d, Event %d", event.rn(), event.ev()) << '\n';
    //----Specifying root file to make---//
    TFile *f_output;
    TString f_name = Form(pre_outfile + "Run_%d_Event_%d.root", event.rn(), event.ev());
    f_output = TFile::Open(f_name,"RECREATE");

    //----TTree to store results----//
    TTree *trun = new TTree("Trun","Run Info");
    double runNo = event.rn(); double eventNo = event.ev();
    trun->Branch("runNo",&runNo); trun->Branch("eventNo",&eventNo);

    TTree *tclst = new TTree("T_charge_cluster_q1","No flash cuts");
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

    WCClstManager clsts;
    clsts.load_tracks(all_clsts, clusters);
    WCClstBundle EventClsts;
    EventClsts = clsts.Get_Clusters();

    for(WCClst &cl : EventClsts){
      if(cl.back().x > x_cathode - x_cathode_th/2. && cl.back().x < x_cathode + x_cathode_th/2.) continue;
      if(cl.back().x > x_anode - x_anode_th/2. && cl.back().x < x_anode + x_anode_th/2.) continue;
      if(cl.back().y < -85. || cl.at(0).y < 95.) continue;
      if(cl.back().z <  20. || cl.at(0).z > 1015.) continue;
      if(abs(cl.back().x - cl.at(0).x) > 230.) continue;
      if(mymaths::pythagoras(cl.back().x,cl.at(0).x,cl.back().y,cl.at(0).y,cl.back().z,cl.at(0).z) < 20.) continue;
      if(cl.back().x < -165. or cl.back().x > 340.) continue;
      std::cout << "This cluster passed: "<< cl.at(0).cn << '\n';

      double xcn = (abs((int)cl.at(0).x)*cl.size());
      for(WCClstPoint &p : cl){
        tclst_rn = p.rn; tclst_ev = p.ev; tclst_cn = p.cn; tclst_sz = cl.size();
        tclst_xcn = xcn;
        tclst_x = p.x; tclst_y = p.y; tclst_z = p.z; tclst_q = p.q;
        tclst_uq = p.uq; tclst_vq = p.vq; tclst_wq = p.wq;
        tclst->Fill();
      }
      tsel_clsts_rn = cl.at(0).rn; tsel_clsts_ev = cl.at(0).ev;
      tsel_clsts_cn = cl.at(0).cn; tsel_clsts_sz = cl.size();
      tsel_clsts_xcn = xcn;
      tsel_clsts->Fill();
    }

    f_output->Write();
    f_output->Close();
    cout << Form("Done with Run %d, Event %d", event.rn(), event.ev()) << endl;
  }
  std::cout << "DONE WITH PROGRAM" << '\n';
}
