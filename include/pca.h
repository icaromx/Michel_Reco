#include <vector>
#include "TVector3.h"
#include "cluster_tools.h"

using namespace std;

struct PCAPoint {
	float x;
	float y;
	float z;
};
/*
struct TrkPoint{
	double c;
	double x;
	double y;
	double z;
	double q;
	double uq;
	double vq;
	double wq;
};

typedef vector<TrkPoint> track_def;
*/

struct PCAResults {
	TVector3 centroid;
	pair<TVector3,TVector3> endPoints;
	float length;
	TVector3 eVals;
	vector<TVector3> eVecs;
};

typedef vector<PCAPoint> PCAPointCloud;

namespace PCA{
  void LoadPointCloud(PCAPointCloud &points, const WCClst &ord_trk);
  PCAResults DoPCA(const PCAPointCloud &points);
}
