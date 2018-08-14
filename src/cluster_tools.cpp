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

//#include "Riostream.h"
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

void EventHandler::load_event(std::string line){
  ffilename.Form("%s",line.c_str());
  TFile *infile = new TFile(ffilename);
  TTree *Trun = (TTree*)infile->Get("Trun");
  Trun->SetBranchAddress("runNo",&frun_num);
  Trun->SetBranchAddress("eventNo",&fev_num);
  Trun->GetEntry(0);

  Int_t cluster_id;
  Double_t x, y, z, q, uq, vq, wq;
  WCClstPoint point;
  TTree *T_charge_cluster = (TTree*)infile->Get("T_charge_cluster");
  T_charge_cluster->SetBranchAddress("cluster_id", &cluster_id);
  T_charge_cluster->SetBranchAddress("qx", &x);
  T_charge_cluster->SetBranchAddress("qy", &y);
  T_charge_cluster->SetBranchAddress("qz", &z);
  T_charge_cluster->SetBranchAddress("qc", &q);
  T_charge_cluster->SetBranchAddress("uqc", &uq);
  T_charge_cluster->SetBranchAddress("vqc", &vq);
  T_charge_cluster->SetBranchAddress("wqc", &wq);
  T_charge_cluster->GetEntry(0);
  fcids.push_back(cluster_id);
  for (int i = 0; i < T_charge_cluster->GetEntries(); i++){
    T_charge_cluster->GetEntry(i);
    point.rn = frun_num; point.ev = fev_num; point.cn = cluster_id;
    point.x = x; point.y = y; point.z = z; point.q = q;
    point.uq = uq; point.vq = vq; point.wq = wq;
    fall_clsts.push_back(point);
    if(fcids.back() == cluster_id) continue;
    fcids.push_back(cluster_id);
  }
  std::sort(fcids.begin(),fcids.end());
  std::reverse(fcids.begin(),fcids.end());
  std::sort(fall_clsts.begin(), fall_clsts.end(), by_cn());

}

void WCClstManager::load_tracks(WCClst event_clsts, std::vector<int> cluster_ids){
  for( int &cn : cluster_ids){
    WCClst cluster;
    for (WCClstPoint &point : event_clsts){
      if(point.cn != cn and !cluster.empty()) break;
      if(point.cn != cn) continue;
      cluster.push_back(point);
    }
    std::sort(cluster.begin(), cluster.end(), by_y());
    event_clusters.push_back(cluster);
  }
}
