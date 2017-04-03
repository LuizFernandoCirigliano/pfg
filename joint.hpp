#ifndef JOINT_H
#define JOINT_H

#include "VRepClass.hpp"

class JointInterface
{
public:
  JointInterface(){}
  virtual ~JointInterface(){}
  virtual void setJointStats(double posAmp, double negAmp, double neutralAngle, double phase, double T) = 0;
  virtual void update() = 0;
  virtual void reset() = 0;
  virtual void prepareMoveToNeutralAngle(double T_ms) = 0;
  virtual void moveToNeutralAngle() = 0;
  bool _enabled = true;
};

class Joint : public VRepClass, public JointInterface {
private:
  double _posAmp;
  double _negAmp;
  double _neutralAngle;
  double _t;
  double _tDelta;
  double _initPhase;
  simxFloat _initAngle;
public:
  void setJointTargetPosition(double targetAngle);
  Joint(int clientID, const char *jointName);
  void setJointStats(double posAmp, double negAmp, double neutralAngle, double phase, double T_ms);
  void update();
  void reset();
  void prepareMoveToNeutralAngle(double T_ms);
  void moveToNeutralAngle();
  double _currentAngle;
  double _neutralAngleDelta;
};

#endif
