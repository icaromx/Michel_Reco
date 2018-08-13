#include <cmath>

#include "mymaths.h"

namespace mymaths{

  double pythagoras(double x1, double x2, double y1, double y2, double z1, double z2){
    return std::sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
  }

}
