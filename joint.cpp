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

void Joint::setJointStats(float posAmp, float negAmp, float neutralAngle, float phase, float T_ms) {
  _posAmp = posAmp;
  _negAmp = negAmp;
  _initPhase = phase;
  _neutralAngle = neutralAngle;
  _tDelta = step_ms/T_ms;
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
  float amplitude = (0 < _t && _t < 0.5) || (-1 < _t && _t < -0.5) ? _posAmp : _negAmp;
  float newAngle = _neutralAngle + _amplFactor * amplitude * sin(2 * PI * _t);
  _t += _tDelta;

  if (_t >= 1.0) {
    _t = 0;
    if (_amplFactor < 1.0) _amplFactor += 0.25;
  }

  this->setJointTargetPosition(newAngle);
}

void Joint::reset() {
  _t = _initPhase;
  _amplFactor = 0.0;
  this->setJointTargetPosition(_initAngle);
}

void Joint::setJointTargetPosition(float targetAngle) {
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
