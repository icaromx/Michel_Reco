//#pragma once
#include <vector>
using namespace std;
namespace mymaths{
  
  double trunc_mean(std::vector<double> data, double per);
  double mean(std::vector<double> data);
  double pythagoras(double x1, double x2, double y1, double y2, double z1, double z2);
  vector<double> Unit_Vec(double x1,double y1,double z1);
  vector<double> Unit_Vec_NO(double x1,double x2,double y1,double y2,double z1,double z2);
  double dotProdFunc(double x1,double x2,double y1,double y2,double z1,double z2);
  vector<double> CrossProd(double x1,double x2,double y1,double y2,double z1,double z2);

}
