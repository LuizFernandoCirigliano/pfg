#include "joint.hpp"
#include "helpers.hpp"
#include <iostream>
#include <math.h>

Joint::Joint(int clientID, const char *jointName, bool inverted) :
  VRepClass(clientID, jointName) {
    _inverted = inverted;
  }

void Joint::setJointStats(double maxAng, double minAng, double initPhase, double T) {
  maxAng = 0;
  minAng = -1;
  if (_inverted) {
    maxAng = -1*maxAng;
    minAng = -1*minAng;
  }
  if (maxAng < minAng) {
    double tmp = maxAng;
    maxAng = minAng;
    minAng = tmp;
  }
  _maxAng = maxAng;
  _minAng = minAng;
  // _T = 2;

  _T = 4;
  _velocity = (_maxAng - _minAng)*2/_T;
  double phase = initPhase > 2*PI ? (initPhase - 2*PI) : initPhase;
  if (phase > PI) {
    _velocity = -1*_velocity;
    phase = 2*PI - phase;
  }
  // if (_inverted) {
  //   _velocity = -1*_velocity;
  // }
  _initialAngle = _minAng + (_maxAng - _minAng)*(phase/PI);
  if (_initialAngle > _maxAng || _initialAngle < _minAng) {
    std::cout << "Something is wrong yo" << std::endl;
  }
  std::cout << "--------" << std::endl;
  std::cout << "phase: " << phase <<std::endl;
  std::cout << "initialAngle: " << _initialAngle << std::endl;
  std::cout << "maxAngle: " << _maxAng << std::endl;
  std::cout << "minxAngle: " << _minAng << std::endl;
  std::cout << "velocity: " << _velocity << std::endl;
  std::cout << "--------" << std::endl;
}

void Joint::update() {
  simxInt operationMode = _firstUpdate ? simx_opmode_streaming : simx_opmode_buffer;
  simxFloat position;
  int retValue;
  simxGetJointPosition(_clientID, _handle, &position, operationMode);
  if (_firstUpdate) {
    // std::cout << "First update" << std::endl;
    retValue = simxSetJointTargetVelocity(_clientID, _handle, _velocity, simx_opmode_oneshot);
    _firstUpdate = false;
  } else {
    // std::cout << position << std::endl;
    if (position < _minAng && _velocity < 0) {
      _velocity = -1*_velocity;
      retValue = simxSetJointTargetVelocity(_clientID, _handle, _velocity, simx_opmode_oneshot);
    } else if (position > _maxAng && _velocity > 0) {
      _velocity = -1*_velocity;
      retValue = simxSetJointTargetVelocity(_clientID, _handle, _velocity, simx_opmode_oneshot);
    }
  }
  if (retValue == -1) std::cout << "Error setting joint velocity" << std::endl;
}

void Joint::reset() {
  this->setJointTargetPosition(_initialAngle);
  simxSetJointTargetVelocity(_clientID, _handle, 0, simx_opmode_oneshot);
  _firstUpdate = true;
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
