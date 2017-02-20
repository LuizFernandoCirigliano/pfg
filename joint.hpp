#ifndef JOINT_H
#define JOINT_H

#include "VRepClass.hpp"

class Joint : public VRepClass {
private:
  bool _movingForward;
  double _currentAngle;
  double _minAngle;
  double _maxAngle;
  simxFloat _initialAngle;
  double _speed;
  void setJointTargetPosition(double targetAngle);
public:
  Joint(int clientID, const char* name, double minAngle, double maxAngle,
     double initialAngle, int steps, bool initialDirection);
  Joint(int clientID, simxInt handle);
  void setJointStats(double minAngle, double maxAngle, double currentAngle,
    double steps);
  void update();
  void reset();
};

#endif
