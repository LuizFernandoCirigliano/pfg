#include "robot.hpp"
#include <iostream>

#include <chrono>
#include <thread>

#include "helpers.hpp"
#include "NAO.hpp"

const int step_ms = 50;

Robot::Robot(int clientID, const char* name) :
  VRepClass(clientID, name) {
  int count = 0;
  simxInt *shapeHandles;
  log_response( simxGetObjects(clientID, sim_object_shape_type, &count, &shapeHandles, simx_opmode_blocking) );

  for (int i = 0; i < count; i++) {
    _shapes.push_back( Shape( clientID, shapeHandles[i]) );
  }

  _nao = new NAO(clientID);
  _nao->getNAOJoints(_joints);
  _numJoints = _joints.size();

  simxGetObjectPosition(_clientID, _handle, -1, _initialPosition, simx_opmode_blocking);
  simxGetObjectOrientation(_clientID, _handle, -1, _initialOrientation, simx_opmode_blocking);

}

void Robot::update() {
  simxPauseCommunication(_clientID, 1);
  for (auto &joint : _joints) {
    joint->update();
  }
  double leftAnkleAngle = -1*(_nao->_hip->_leftJoint->_currentAngle +
    _nao->_knee->_leftJoint->_currentAngle);
  double rightAnkleAngle = -1*(_nao->_hip->_rightJoint->_currentAngle +
    _nao->_knee->_rightJoint->_currentAngle);

  _nao->_ankle->_leftJoint->setJointTargetPosition(leftAnkleAngle);
  _nao->_ankle->_rightJoint->setJointTargetPosition(rightAnkleAngle);

  simxPauseCommunication(_clientID, 0);

  // std::this_thread::sleep_for(std::chrono::milliseconds(step_ms));
  simxSynchronousTrigger(_clientID);
}

void Robot::reset() {
  simxPauseCommunication(_clientID, 1);
  for ( auto &j : _joints ) {
    j->reset();
  }
  _nao->_ankle->reset();
  simxPauseCommunication(_clientID, 0);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  simxPauseCommunication(_clientID, 1);

  int posRet = simxSetObjectPosition(_clientID, _handle, -1,
    _initialPosition, simx_opmode_oneshot);
  int oriRet = simxSetObjectOrientation(_clientID, _handle, -1,
    _initialOrientation, simx_opmode_oneshot);

  for ( Shape &s : _shapes ) {
    s.reset();
  }

  simxPauseCommunication(_clientID, 0);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void Robot::setGenome(double genome[]) {
  for (int i = 0; i < _numJoints; i++) {
    _joints[i]->setJointStats(genome[3*i], genome[3*i + 1], genome[3*i + 2]);
  }
}

double Robot::getXDistance() {
  simxFloat position[3];
  simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
  return position[0] - _initialPosition[0];
}

void Robot::printPosition() {
  simxFloat position[3];
  simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
  std::cout << "[" << position[0] << ", " << position[1] << ", " << position[2] << "]" << std::endl;
}
