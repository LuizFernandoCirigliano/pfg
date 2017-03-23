#ifndef JOINTPAIR
#define JOINTPAIR

#include "joint.hpp"

class JointPair : public JointInterface {
public:
  Joint* _leftJoint;
  Joint* _rightJoint;
  JointPair(int clientID, const char *rightJointName,
    const char *leftJointName, double lb, double ub);
  void setJointStats(double posAmp, double negAmp, double zeroAngle);
  void update();
  void reset();
};

#endif
