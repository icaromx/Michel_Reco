#pragma once

using namespace std;

struct ClstPoint{
  Int_t rn, ev, cn;
  Double_t x, y, z, q, uq, vq, wq;
};
typedef vector<ClstPoint> Clst;
typedef vector<Clst> ClstBundle;

struct by_cn {
	bool operator()(ClstPoint const &a, ClstPoint const &b) {
		if(a.cn == b.cn) return a.x > b.x;
		else return a.cn > b.cn;
	}
};
struct by_y {
	bool operator()(ClstPoint const &a, ClstPoint const &b) {
		if(a.y == b.y) return a.x > b.x;
		else return a.y > b.y;
	}
};

class EventHandler {
    TString ffilename;
    Int_t frun_num, fev_num;
    std::vector<int> fcids;
    Clst fall_clsts;
  public:
    void load_event(std::string);
    TString name() {return ffilename;}
    int rn() {return frun_num;}
    int ev() {return fev_num;}
    std::vector<int> wc_clusters() {return fcids;}
    Clst Get_All_Tracks() {return fall_clsts;}
};

class ClstManager {
    Int_t num_clusters;
    ClstBundle event_clusters;
  public:
    void load_tracks(Clst,std::vector<int>);
    ClstBundle Get_Clusters() {return event_clusters;}
};
