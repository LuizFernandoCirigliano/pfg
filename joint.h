#ifndef JOINT_H
#define JOINT_H

extern "C" {
  #include "extApi.h"
}

class Joint {
  private:
    int _clientID;
    bool _movingForward;
    double _currentAngle;
    simxInt _jointHandler;
    double _minAngle;
    double _maxAngle;
    double _speed;
    double _initialAngle;
  public:
    Joint(int clientID, const char* jointName, double minAngle, double maxAngle,
       double speed, double initialAngle);
    void update();
    // double getCurrentAngle();
};

#endif
