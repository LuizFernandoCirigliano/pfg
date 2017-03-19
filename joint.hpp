#ifndef JOINT_H
#define JOINT_H

#include "VRepClass.hpp"

class Joint : public VRepClass {
private:
  double _maxAng;
  double _minAng;
  double _velocity;
  double _T;
  int _velDirection;
  double _initialAngle;
  bool _firstUpdate = true;
  bool _inverted;

  void setJointTargetPosition(double targetAngle);
public:
  Joint(int clientID, const char *jointName, bool inverted);
  void setJointStats(double maxAng, double minAng, double initPhase, double T);
  void update();
  void reset();
};

#endif
