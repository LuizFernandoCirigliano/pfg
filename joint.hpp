#ifndef JOINT_H
#define JOINT_H

#include "VRepClass.hpp"

class JointInterface
{
public:
  JointInterface(){}
  virtual ~JointInterface(){}
  virtual void setJointStats(double posAmp, double negAmp, double zeroAngle) = 0;
  virtual void update() = 0;
  virtual void reset() = 0;
  double _upperBound;
  double _lowerBound;
};

class Joint : public VRepClass, public JointInterface {
private:
  double _posAmp;
  double _negAmp;
  double _zeroAngle;
  double _t;
  double _initPhase;
public:
  void setJointTargetPosition(double targetAngle);
  Joint(int clientID, const char *jointName, double initPhase, double lb, double up);
  void setJointStats(double posAmp, double negAmp, double zeroAngle);
  void update();
  void reset();
  double _currentAngle;
};

#endif
