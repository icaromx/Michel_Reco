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
#include "parameters.h"

void EventHandler::load_event(std::string line){
  ffilename.Form("%s",line.c_str());
  TFile *infile = new TFile(ffilename);
  TTree *Trun = (TTree*)infile->Get("Trun");
  Trun->SetBranchAddress("runNo",&frun_num);
  Trun->SetBranchAddress("subRunNo",&fsub_run);
  Trun->SetBranchAddress("eventNo",&fev_num);
  Trun->GetEntry(0);

  Int_t cluster_id, xcn;
  Double_t x, y, z, q, uq, vq, wq;
  WCClstPoint point;
  TTree *T_charge_cluster = (TTree*)infile->Get("T_charge_cluster_q1");
  fentries = T_charge_cluster->GetEntries();
  T_charge_cluster->SetBranchAddress("cluster_id", &cluster_id);
  T_charge_cluster->SetBranchAddress("qx", &x);
  T_charge_cluster->SetBranchAddress("qy", &y);
  T_charge_cluster->SetBranchAddress("qz", &z);
  T_charge_cluster->SetBranchAddress("qc", &q);
  T_charge_cluster->SetBranchAddress("uqc", &uq);
  T_charge_cluster->SetBranchAddress("vqc", &vq);
  T_charge_cluster->SetBranchAddress("wqc", &wq);
  if (T_charge_cluster->GetListOfBranches()->FindObject("xcn")){
    T_charge_cluster->SetBranchAddress("xcn", &xcn);
  }
  T_charge_cluster->GetEntry(0);
  fcids.push_back(cluster_id);
  for (int i = 0; i < T_charge_cluster->GetEntries(); i++){
    T_charge_cluster->GetEntry(i);
    if (T_charge_cluster->GetListOfBranches()->FindObject("xcn")) {
        point.xcn = xcn;
    }else{
        point.xcn = -1;
    }
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
  infile->Close();
}

void WCClstManager::load_tracks(WCClst event_clsts, std::vector<int> cluster_ids){
  Parameters par;
  for(int &cn : cluster_ids){
    WCClst cluster;
    for (WCClstPoint &point : event_clsts){
      if(point.cn != cn and !cluster.empty()) break;
      if(point.cn != cn) continue;
      cluster.push_back(point);
    }
    if (cluster.size() < par.stitch_win_size) continue;
    std::sort(cluster.begin(), cluster.end(), by_y());
    event_clusters.push_back(cluster);
  }
}

void StitchTool::load_wc_cluster(WCClst wcclst){
  Parameters par;
  WCClst temp_unstitch, low_q_pts, wcclst_test;
  wcclst_test = wcclst;
  for (size_t i = 0; i < par.stch_seed; i++) { //Parameter used
    stitch_pts.push_back(wcclst_test.at(0));
    wcclst_test.erase(wcclst_test.begin());
  }
  for(int i = 0; i < wcclst_test.size(); i++){ //<3> Looping over points
    WCClstPoint temp_pt;
    temp_pt.rn = wcclst.at(i).rn; temp_pt.ev = wcclst.at(i).ev;
    temp_pt.cn = wcclst.at(i).cn; temp_pt.xcn = wcclst.at(i).xcn;
    temp_pt.x = wcclst.at(i).x; temp_pt.y = wcclst.at(i).y; temp_pt.z = wcclst.at(i).z;
    temp_pt.q = wcclst.at(i).q;
    temp_pt.uq = wcclst.at(i).uq; temp_pt.vq = wcclst.at(i).vq; temp_pt.wq = wcclst.at(i).wq;
    if (wcclst.at(i).q > 8000) {
      temp_unstitch.push_back(temp_pt);
    }else{
      low_q_pts.push_back(temp_pt);
    }
  }

  while (temp_unstitch.size() != 0){
    WCClst WC_Chunk;
    for (int p = wcclst_test.size() - par.stch_seed; p < wcclst_test.size(); p++){
      WCClstPoint temp_p;
      temp_p.rn = wcclst_test.at(p).rn; temp_p.ev = wcclst_test.at(p).ev;
      temp_p.cn = wcclst_test.at(p).cn; temp_p.xcn = wcclst_test.at(p).xcn;
      temp_p.x = wcclst_test.at(p).x; temp_p.y = wcclst_test.at(p).y; temp_p.z = wcclst_test.at(p).z;
      temp_p.q = wcclst_test.at(p).q;
      temp_p.uq = wcclst_test.at(p).uq; temp_p.vq = wcclst_test.at(p).vq; temp_p.wq = wcclst_test.at(p).wq;
      WC_Chunk.push_back(temp_p);
    }
    
  }


  stitch_pts = temp_unstitch;
}
