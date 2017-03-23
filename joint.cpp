#include "joint.hpp"
#include "helpers.hpp"
#include <iostream>
#include <math.h>
#include <iomanip>


Joint::Joint(int clientID, const char *jointName, double initPhase, double lb, double ub) :
  VRepClass(clientID, jointName),
  _t(initPhase),
  _initPhase(initPhase) {
    _upperBound = ub;
    _lowerBound = lb;
  }

void Joint::setJointStats(double posAmp, double negAmp, double zeroAngle) {
  _posAmp = posAmp;
  _negAmp = negAmp;
  _zeroAngle = zeroAngle;
  #ifdef DEBUG
  std::cout<< _name <<std::endl;
  std::cout<< "Positive Amplitude: " << std::setprecision(2) << posAmp <<std::endl;
  std::cout<< "Negative Amplitude: " << std::setprecision(2) <<  negAmp <<std::endl;
  std::cout<< "Zero angle: " << std::setprecision(2) << zeroAngle <<std::endl;
  #endif
}

void Joint::update() {
  double amplitude = _t < 0.5 ? _posAmp : _negAmp;
  double newAngle = _zeroAngle + amplitude * sin(2 * PI * _t);
  _t += 0.01;
  if (_t >= 1) _t = 0;
  this->setJointTargetPosition(newAngle);
}

void Joint::reset() {
  _t = _initPhase;
  this->setJointTargetPosition(_zeroAngle);
}

void Joint::setJointTargetPosition(double targetAngle) {
  int ret = simxSetJointTargetPosition(_clientID, _handle, targetAngle, simx_opmode_oneshot);
  _currentAngle = targetAngle;
  #ifdef DEBUG
  if (ret) {
    std::cout << "Error in joint " << _handle << " for angle " << targetAngle
      << "." << std::endl;
    log_response(ret);
  }
  #endif
}
