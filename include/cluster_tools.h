#pragma once

using namespace std;

struct WCClstPoint{
  Int_t rn, ev, cn, xcn;
  Double_t x, y, z, q, uq, vq, wq;
};

typedef vector<WCClstPoint> WCClst;
typedef vector<WCClst> WCClstBundle;

struct by_cn {
	bool operator()(WCClstPoint const &a, WCClstPoint const &b) {
		if(a.cn == b.cn) return a.x > b.x;
		else return a.cn > b.cn;
	}
};

struct by_y {
	bool operator()(WCClstPoint const &a, WCClstPoint const &b) {
		if(a.y == b.y) return a.x > b.x;
		else return a.y > b.y;
	}
};

class EventHandler {
    TString ffilename;
    Int_t frun_num, fsub_run, fev_num, fentries;
    std::vector<int> fcids;
    WCClst fall_clsts;
  public:
    void load_event(std::string);
    TString name() {return ffilename;}
    int rn() {return frun_num;}
    int ev() {return fev_num;}
    int sr() {return fsub_run;}
    std::vector<int> wc_clusters() {return fcids;}
    WCClst Get_All_Tracks() {return fall_clsts;}
    int GetEntries(){return fentries;}
};

class WCClstManager {
    Int_t num_clusters;
    WCClstBundle event_clusters;
  public:
    void load_tracks(WCClst,std::vector<int>);
    WCClstBundle Get_Clusters() {return event_clusters;}
};

class StitchTool {
    WCClst stitch_pts, unstitch_pts;//, temp_unstitch, low_q_pts;
  public:
    void load_wc_cluster(WCClst);
    WCClst Get_Stitched_Pts() {return stitch_pts;}
    WCClst Get_UnStitched_Pts() {return unstitch_pts;}
};
