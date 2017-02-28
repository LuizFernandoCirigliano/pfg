#include "joint.hpp"
#include "helpers.hpp"
#include <iostream>
#include <math.h>

Joint::Joint(int clientID, simxInt handle, double initPhase) :
  VRepClass(clientID, handle),
  _posAmp(3.14),
  _negAmp(3.14),
  _zeroAngle(0),
  _t(initPhase),
  _initPhase(initPhase) {}

Joint::Joint(int clientID, const char *jointName, double initPhase) :
  VRepClass(clientID, jointName),
  _posAmp(3.14),
  _negAmp(3.14),
  _zeroAngle(0),
  _t(initPhase),
  _initPhase(initPhase) {}

void Joint::setJointStats(double posAmp, double negAmp, double zeroAngle) {
  _posAmp = posAmp;
  _negAmp = negAmp;
  _zeroAngle = zeroAngle;
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

  if (DISABLE_LOG) return;
  if (ret) {
    std::cout << "Error in joint " << _handle << " for angle " << targetAngle
      << "." << std::endl;
    log_response(ret);
  }
}
