// Author: Ivan Caro Terrazas
// Date: 11/August/2018

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
#include "parameters.h"

#define PI 3.14159
#define euler 2.71828
#define cal_factor 0.0000385

using namespace std;

/////////////////////////////////////////////
int main(int argc, char **argv){

  //----Parameters----//
  double charge_cut = 1.;
  //Clustering Algorithm Parameters
  double alpha = 5; // Sphere radius
  double ext_alpha = 21.5; // Extended cone radius
  double al_print = alpha*100; //
  unsigned ang_points = 8; // Points in window usied for ordering algorithm
  int min_points_trk = ang_points*2; // Minimum number of points usied in ordering algorithm
  double min_cone_ang = 0.97; // Minimum Cone angle; ArcCos(0.97) -> 14 deg -> 0.24 Rad
  double max_phi = acos(min_cone_ang); // cone angle in radians
  double back_ang = -0.4; // Angle of back cone 113 deg -> 1.98 Rad
  back_ang = acos(back_ang); // back angle in radians

  double dist_unclustered = 2.; //Distance at which the unclustered point is considered for the unclustered/clustered ratio
  double cut_ucp_ratio = 0.20; // unclustered/clustered ratio at which to cut cluster as having to many unclustered points
  double cut_bottom_dist = 10.;// CUT: Max distance from the last clustered point and the lowest Y value point

  double eps_ang = 0.314159; // epsilon_phi paramter
  double eps_dist = 1.5; // epsilon_r parameter
  double weight;

  //----Output file----//
  string parent = "Clustered_Sample/";
  DIR* dir = opendir(parent.c_str());
  if (ENOENT == errno){ /* Directory does not exists. */
    const int dir_err = mkdir(parent.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
  TString pre_outfile = (TString) parent + "clst_" ;

  //----Specifying root file to make---//
  TFile *f_output;
  //TString f_name = "test.root";
  TString f_name = Form(pre_outfile + "test_%d.root", 314);
  f_output = TFile::Open(f_name,"RECREATE");
/*
  //----TTree to store results----//
  TTree *tclst = new TTree("Clusters","No flash cuts");
  double tclst_rn, tclst_ev, tclst_cn, tclst_sz, tclst_xcn;
  double tclst_x, tclst_y, tclst_z, tclst_q;
  double tclst_uq, tclst_vq, tclst_wq;
  tclst->Branch("run_num",&tclst_rn); tclst->Branch("ev_num",&tclst_ev);
  tclst->Branch("cluster_id",&tclst_cn); tclst->Branch("size",&tclst_sz);
  tclst->Branch("xcn",&tclst_xcn);
  tclst->Branch("x",&tclst_x); tclst->Branch("y",&tclst_y); tclst->Branch("z",&tclst_z);
  tclst->Branch("q",&tclst_q); tclst->Branch("uq",&tclst_uq);
  tclst->Branch("vq",&tclst_vq); tclst->Branch("wq",&tclst_wq);
*/

  //----Filelist to use----//
  std::ifstream filelist(argv[1]);
  std::string rootfile;

  while(std::getline(filelist, rootfile)){ //<1> Looping over events
    std::cout << rootfile << '\n';
    EventHandler event; event.load_event(rootfile); //Loads Even
    std::vector<int> clusters = event.wc_clusters(); //Get cluster list
    WCClst all_clsts = event.Get_All_Tracks();

    Int_t run_num, ev_num;
    run_num = event.rn(); ev_num = event.ev();
    std::cout << Form("Run: %d & Event: %d",run_num,ev_num) << '\n';
    continue;
    WCClstManager clsts; clsts.load_tracks(all_clsts, clusters);
    WCClstBundle EventClsts = clsts.Get_Clusters(); //Clusters Separated

    for(WCClst &cl : EventClsts){ //<2> Looping over WC clusters
      StitchTool stitched_clst;
      stitched_clst.load_wc_cluster(cl);
      WCClstPoint clst_points, unclst, temp_unclst, low_q1_unclst;

    } //<2> WC clusters loops
    cout << Form("Done with event %d", event.ev()) << endl;
  }//<1> Event Loops

  f_output->Write();
  f_output->Close();
  std::cout << "DONE" << '\n';

}
