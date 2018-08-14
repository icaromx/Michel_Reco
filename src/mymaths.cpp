#include <cmath>
#include <vector>
#include <stdlib.h>
#include "mymaths.h"


using namespace std;

namespace mymaths{

  double mean(std::vector<double> data){
  	double m = 0;
  	for (int i = 0; i < data.size(); ++i){
  		m += data[i];
  	}
  	m = m/((double)data.size());
  	return m;
  }

  double trunc_mean(std::vector<double> data, double per){
  	double m = 0, trunc;
  	sort(data.begin(), data.end());
  	trunc = data.size()*per;
  	trunc = (int)trunc;
  	for (int i = trunc; i < data.size() - trunc; ++i){
  		m += data[i];
  	}
  	m = m/((double)data.size() - 2.* trunc);
  	return m;
  }

  double pythagoras(double x1, double x2, double y1, double y2, double z1, double z2){
    return std::sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
  }

  vector<double> Unit_Vec(double x1,double y1,double z1){
  	std::vector<double> v;
  	double norm;
  	norm = pythagoras(x1,0.0,y1,0.0,z1,0.0);
  	v.push_back(x1/norm);
  	v.push_back(y1/norm);
  	v.push_back(z1/norm);
  	return v;
  }

  vector<double> Unit_Vec_NO(double x1,double x2,double y1,double y2,double z1,double z2){
  	std::vector<double> v;
  	double norm;
  	norm = pythagoras(x1,x2,y1,y2,z1,z2);
  	v.push_back((x2-x1)/norm);
  	v.push_back((y2-y1)/norm);
  	v.push_back((z2-z1)/norm);
  	return v;
  }

  double dotProdFunc(double x1,double x2,double y1,double y2,double z1,double z2){
  	double dotP;
  	dotP = x1*x2 + y1*y2 + z1*z2;
  	return dotP;
  }

  vector<double> CrossProd(double x1,double x2,double y1,double y2,double z1,double z2){
  	vector<double> v;
  	double x,y,z;
  	x = y1*z2 - z1*y2;
  	y = x1*z2 - x2*z1;
  	y = -y;
  	z = x1*y2 - x2*y1;
  	v.push_back(x);
  	v.push_back(y);
  	v.push_back(z);
  	return v;
  }
}
