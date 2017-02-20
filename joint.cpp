#include "joint.hpp"
#include "helpers.hpp"
#include <iostream>

Joint::Joint(int clientID, const char* name, double minAngle,
  double maxAngle, double initialAngle, int steps, bool initialDirection) :
  VRepClass(clientID, name),
  _minAngle(minAngle),
  _maxAngle(maxAngle),
  _currentAngle(initialAngle),
  _movingForward(initialDirection),
  _speed( (maxAngle - minAngle)/steps )
{
  log_response(
    simxSetJointPosition(_clientID, _handle, _initialAngle, simx_opmode_blocking)
  );
}

Joint::Joint(int clientID, simxInt handle) :
  VRepClass(clientID, handle),
  _minAngle(-3.14),
  _maxAngle(3.14),
  _currentAngle(0),
  _movingForward(true),
  _speed(0.1)
{
  simxGetJointPosition(_clientID, _handle, &_initialAngle, simx_opmode_blocking);
  std::cout << "Joint: " << _handle << " Angle: "  << _initialAngle << std::endl;
}

void Joint::setJointStats(double minAngle, double maxAngle, double currentAngle, double steps)
{
  _minAngle = minAngle;
  _maxAngle = maxAngle;
  _currentAngle = currentAngle;
  _speed = (maxAngle - minAngle)/steps;
}
void Joint::update()
{
  // Invert direction of movement if at edge.
  double newAngle = _currentAngle;
  if ( (newAngle >= _maxAngle && _movingForward) ||
    (newAngle <= _minAngle && !_movingForward) ) {
    _movingForward = !_movingForward;
  }
  if ( _movingForward ) {
    newAngle += _speed;
    if (newAngle > _maxAngle) {
      newAngle = _maxAngle;
    }
  } else {
    newAngle -= _speed;
    if (newAngle < _minAngle) {
      newAngle = _minAngle;
    }
  }
  this->setJointTargetPosition(newAngle);
}

void Joint::reset()
{
  this->setJointTargetPosition(_initialAngle);
  _movingForward = true;
}

void Joint::setJointTargetPosition(double targetAngle)
{
  _currentAngle = targetAngle;
  int ret = simxSetJointTargetPosition(_clientID, _handle, targetAngle, simx_opmode_oneshot);

  if (ret) {
    std::cout << "Error in joint " << _handle << " for angle " << targetAngle
      << "." << std::endl;
    log_response(ret);
  }
}
