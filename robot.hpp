#ifndef ROBOT_H
#define  ROBOT_H

#include "VRepClass.hpp"
#include "joint.hpp"
#include "shape.hpp"
#include "nao.hpp"
#include <vector>

class Robot : public VRepClass {
  simxFloat _initialPosition[3];
  simxFloat _initialOrientation[3];

  void initJoints();
public:
  int _numJoints;
  std::vector<JointInterface *> _joints;
  std::vector<Shape> _shapes;
  NAO *_nao;
  Robot(int clientID, const char* name);
  void reset();
  void update();
  void setGenome( double genome[] );
  void printPosition();
  double getXDistance();
};

#endif
