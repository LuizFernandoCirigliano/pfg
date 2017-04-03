#ifndef JOINTPAIR
#define JOINTPAIR

#include "joint.hpp"

class JointPair : public JointInterface {
public:
  Joint* _leftJoint;
  Joint* _rightJoint;
  JointPair(int clientID,
    const char *rightJointName,
    const char *leftJointName);
  void setJointStats(double posAmp, double negAmp, double zeroAngle, double phase, double T);
  void update();
  void reset();
  void prepareMoveToNeutralAngle(double T_ms);
  void moveToNeutralAngle();
};

#endif
