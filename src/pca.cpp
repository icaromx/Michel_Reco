#include "TROOT.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TError.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TH3F.h"
#include "TProfile2D.h"
#include "TChain.h"
#include "TStyle.h"
#include "TString.h"
#include "TVector3.h"
#include "TCanvas.h"
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

#include "mymaths.h"
#include "pca.h"

#ifdef __APPLE__
    #if TARGET_OS_MAC
        #include "/usr/local/Cellar/eigen/3.3.4/include/eigen3/Eigen/Dense" //Needed on MACOS
    #endif
#elif __linux__
    #include "/grid/fermiapp/products/larsoft/eigen/v3_3_3/include/eigen3/Eigen/Dense" //Needed on uboonegpvm
#else
#   error "Unknown compiler"
#endif

using namespace std;

namespace PCA{

  void LoadPointCloud(PCAPointCloud &points, const WCClst &ord_trk) {
  	for (int i = 0; i < ord_trk.size(); ++i){
  		PCAPoint tempPoint;
  		tempPoint.x = ord_trk.at(i).x;
  		tempPoint.y = ord_trk.at(i).y;
  		tempPoint.z = ord_trk.at(i).z;
  		points.push_back(tempPoint);

  	}
  	return;
  }

  PCAResults DoPCA(const PCAPointCloud &points) {
  	TVector3 outputCentroid;
  	pair<TVector3,TVector3> outputEndPoints;
  	float outputLength;
  	TVector3 outputEigenValues;
  	vector<TVector3> outputEigenVecs;
  	float meanPosition[3] = {0., 0., 0.};
  	unsigned int nThreeDHits = 0;
  	for (unsigned int i = 0; i < points.size(); i++) {
  		meanPosition[0] += points[i].x;
  		meanPosition[1] += points[i].y;
  		meanPosition[2] += points[i].z;
  		++nThreeDHits;
  	}
  	if (nThreeDHits == 0) {
  		PCAResults results;
  		return results;
  	}
  	const float nThreeDHitsAsFloat(static_cast<float>(nThreeDHits));
  	meanPosition[0] /= nThreeDHitsAsFloat;
  	meanPosition[1] /= nThreeDHitsAsFloat;
  	meanPosition[2] /= nThreeDHitsAsFloat;
  	outputCentroid = TVector3(meanPosition[0], meanPosition[1], meanPosition[2]);
  	float xi2 = 0.0;
  	float xiyi = 0.0;
  	float xizi = 0.0;
  	float yi2 = 0.0;
  	float yizi = 0.0;
  	float zi2 = 0.0;
  	float weightSum = 0.0;
  	for (unsigned int i = 0; i < points.size(); i++) {
  		const float weight(1.);
  		const float x((points[i].x - meanPosition[0]) * weight);
  		const float y((points[i].y - meanPosition[1]) * weight);
  		const float z((points[i].z - meanPosition[2]) * weight);
  		xi2	+= x * x;
  		xiyi += x * y;
  		xizi += x * z;
  		yi2	+= y * y;
  		yizi += y * z;
  		zi2	+= z * z;
  		weightSum += weight * weight;
  	}

  	Eigen::Matrix3f sig;

  	sig <<  xi2, xiyi, xizi,
  			xiyi, yi2, yizi,
  			xizi, yizi, zi2;

  	sig *= 1.0 / weightSum;

  	Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> eigenMat(sig);

  	typedef std::pair<float,size_t> EigenValColPair;
  	typedef std::vector<EigenValColPair> EigenValColVector;

  	EigenValColVector eigenValColVector;
  	const auto &resultEigenMat(eigenMat.eigenvalues());
  	eigenValColVector.emplace_back(resultEigenMat(0), 0);
  	eigenValColVector.emplace_back(resultEigenMat(1), 1);
  	eigenValColVector.emplace_back(resultEigenMat(2), 2);

  	std::sort(eigenValColVector.begin(), eigenValColVector.end(), [](const EigenValColPair &left, const EigenValColPair &right){return left.first > right.first;} );

  	outputEigenValues = TVector3(eigenValColVector.at(0).first, eigenValColVector.at(1).first, eigenValColVector.at(2).first);

  	const Eigen::Matrix3f &eigenVecs(eigenMat.eigenvectors());

  	for (const EigenValColPair &pair : eigenValColVector) {
  		outputEigenVecs.emplace_back(eigenVecs(0, pair.second), eigenVecs(1, pair.second), eigenVecs(2, pair.second));
  	}

  	PCAResults results;

  	Eigen::ParametrizedLine<float,3> priAxis(Eigen::Vector3f(outputCentroid(0),outputCentroid(1),outputCentroid(2)),Eigen::Vector3f(outputEigenVecs[0](0),outputEigenVecs[0](1),outputEigenVecs[0](2)));

  	Eigen::Vector3f endPoint1(Eigen::Vector3f(outputCentroid(0),outputCentroid(1),outputCentroid(2)));
  	Eigen::Vector3f endPoint2(Eigen::Vector3f(outputCentroid(0),outputCentroid(1),outputCentroid(2)));

  	Eigen::Vector3f testPoint;
  	Eigen::Vector3f projTestPoint;
  	float maxDist1 = -1.0;
  	float maxDist2 = -1.0;
  	float dist;
  	float dotP;
  	for (unsigned int i = 0; i < points.size(); i++) {
  		testPoint = Eigen::Vector3f(points[i].x,points[i].y,points[i].z);
  		projTestPoint = priAxis.projection(testPoint);
  		dist = sqrt(pow(projTestPoint(0)-outputCentroid(0),2.0)+pow(projTestPoint(1)-outputCentroid(1),2.0)+pow(projTestPoint(2)-outputCentroid(2),2.0));
  		dotP = (projTestPoint(0)-outputCentroid(0))*outputEigenVecs[0](0) + (projTestPoint(1)-outputCentroid(1))*outputEigenVecs[0](1) + (projTestPoint(2)-outputCentroid(2))*outputEigenVecs[0](2);


  		if ((dotP < 0.0) && (dist > maxDist1)) {
  			endPoint1 = projTestPoint;
  			maxDist1 = dist;
  		}
  		else if ((dotP > 0.0) && (dist > maxDist2)) {
  			endPoint2 = projTestPoint;
  			maxDist2 = dist;
  		}
  	}
  	outputEndPoints.first = TVector3(endPoint1(0),endPoint1(1),endPoint1(2));
  	outputEndPoints.second = TVector3(endPoint2(0),endPoint2(1),endPoint2(2));
  	outputLength = sqrt(pow(endPoint2(0)-endPoint1(0),2.0)+pow(endPoint2(1)-endPoint1(1),2.0)+pow(endPoint2(2)-endPoint1(2),2.0));
  	results.centroid = outputCentroid;
  	results.endPoints = outputEndPoints;
  	results.length = outputLength;
  	results.eVals = outputEigenValues;
  	results.eVecs = outputEigenVecs;
  	return results;
  }


}
