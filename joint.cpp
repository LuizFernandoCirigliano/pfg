#include "joint.h"

Joint::Joint(int clientID, const char* jointName, double minAngle,
  double maxAngle, double speed, double initialAngle)
{
  _minAngle = minAngle;
  _maxAngle = maxAngle;
  _speed = speed;
  _clientID = clientID;
  _initialAngle = initialAngle;

  simxGetObjectHandle(clientID, jointName, &_jointHandler,
    simx_opmode_oneshot_wait);
  simxSetJointTargetPosition(_clientID, _jointHandler, initialAngle, simx_opmode_oneshot);
}

void Joint::update()
{
  // Invert direction of movement if at edge.
  if ( (_currentAngle >= _maxAngle && _movingForward) ||
    (_currentAngle <= _minAngle && !_movingForward) ) {
    _movingForward = !_movingForward;
  }
  if ( _movingForward ) {
    _currentAngle += _speed;
    if (_currentAngle > _maxAngle) {
      _currentAngle = _maxAngle;
    }
  } else {
    _currentAngle -= _speed;
    if (_currentAngle < _minAngle) {
      _currentAngle = _minAngle;
    }
  }

  simxSetJointTargetPosition(_clientID, _jointHandler, _currentAngle, simx_opmode_oneshot);
}
