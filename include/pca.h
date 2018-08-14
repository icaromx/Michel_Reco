#include <vector>
#include "TVector3.h"

using namespace std;

struct Point {
	float x;
	float y;
	float z;
  float q;
};

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

struct PCAResults {
	TVector3 centroid;
	pair<TVector3,TVector3> endPoints;
	float length;
	TVector3 eVals;
	vector<TVector3> eVecs;
};

typedef vector<Point> PointCloud;

namespace PCA{
  void LoadPointCloud(PointCloud &points, const track_def &ord_trk);
  PCAResults DoPCA(const PointCloud &points);
}
