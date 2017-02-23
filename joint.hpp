#ifndef JOINT_H
#define JOINT_H

#include "VRepClass.hpp"

class Joint : public VRepClass {
private:
  double _posAmp;
  double _negAmp;
  double _zeroAngle;
  double _t;
  double _initPhase;
  void setJointTargetPosition(double targetAngle);
public:
  Joint(int clientID, simxInt handle, double initPhase);
  Joint(int clientID, const char *jointName, double initPhase);
  void setJointStats(double posAmp, double negAmp, double zeroAngle);
  void update();
  void reset();
};

#endif
