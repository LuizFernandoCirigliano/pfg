#ifndef ROBOT_H
#define  ROBOT_H

#include "VRepClass.hpp"
#include "joint.hpp"
#include "shape.hpp"
#include "NAO.hpp"

#include <vector>

struct result{
  float score, distance, time;
};

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
  void setGenome( const std::vector<float> &genome );
  void printPosition(simxFloat* position);
  float getXDistance();
  void moveToNeutralAngle();
  result runExperiment( const std::vector<float> &genome );
  std::vector< std::pair<float, float> > getAleles();
};

#endif
