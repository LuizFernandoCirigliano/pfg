#include "joint.hpp"
#include "helpers.hpp"
#include <iostream>
#include <math.h>
#include <iomanip>


Joint::Joint(int clientID, const char *jointName) :
  VRepClass(clientID, jointName),
  _currentAngle(0.0),
  _neutralAngle(0.0)
  {
    simxGetJointPosition(_clientID, _handle, &_initAngle, simx_opmode_blocking);
    std::cout << _initAngle << std::endl;
  }

void Joint::setJointStats(double posAmp, double negAmp, double neutralAngle, double phase, double T_ms) {
  _posAmp = fabs(posAmp);
  _negAmp = fabs(negAmp);
  _initPhase = phase;
  _neutralAngle = neutralAngle;
  _tDelta = 50.0/T_ms;
  #ifdef DEBUG
  std::cout<< _name <<std::endl;
  std::cout<< std::setprecision(2) << _posAmp << ", ";
  std::cout<< std::setprecision(2) <<  _negAmp << ", ";
  std::cout<< std::setprecision(2) << _neutralAngle << ", ";
  std::cout<< std::setprecision(2) << _initPhase << std::endl;
  #endif
}

void Joint::update() {
  if (!_enabled) return;
  double amplitude = 0 < _t && _t < 0.5 ? _posAmp : _negAmp;
  double newAngle = _neutralAngle + amplitude * sin(2 * PI * _t);
  _t += _tDelta;
  this->setJointTargetPosition(newAngle);
}

void Joint::prepareMoveToNeutralAngle(double T_ms) {
  double num_updates = T_ms/dt_ms;
  double amplitude = 0 < _t && _t < 0.5 ? _posAmp : _negAmp;
  double newAngle = _neutralAngle + amplitude * sin(2 * PI * _t);
  _neutralAngleDelta = (newAngle - _currentAngle)/num_updates;
}

void Joint::moveToNeutralAngle() {
  double newAngle = _currentAngle + _neutralAngleDelta;
  this->setJointTargetPosition(newAngle);
}

void Joint::reset() {
  _t = _initPhase;
  this->setJointTargetPosition(_initAngle);
}

void Joint::setJointTargetPosition(double targetAngle) {
  int ret = simxSetJointTargetPosition(_clientID, _handle, targetAngle, simx_opmode_oneshot);
  _currentAngle = targetAngle;
  #ifdef DEBUG
  // std::cout<< "Current angle: " << std::setprecision(2) << _currentAngle <<std::endl;
  if (ret) {
    std::cout << "Error in joint " << _name << " for angle " << targetAngle
      << "." << std::endl;
    log_response(ret);
  }
  #endif
}
