#ifndef ROBOT_H
#define  ROBOT_H

#include "VRepClass.hpp"
#include "joint.hpp"
#include "shape.hpp"
#include <vector>

class Robot : public VRepClass {
  simxFloat _initialPosition[3];
  simxFloat _initialOrientation[3];
public:
  std::vector<Joint> _joints;
  std::vector<Shape> _shapes;
  Robot(int clientID, const char* name);
  void reset();
  void update();
};

#endif
