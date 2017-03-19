#include "robot.hpp"
#include <iostream>

#include <chrono>
#include <thread>
#include <math.h>

#include "helpers.hpp"

const char* rightLegJoints[] = {
  "RHipYawPitch3",
  // "RHipRoll3",
  "RHipPitch3",
  "RKneePitch3",
  "RAnklePitch3",
  // "RAnkleRoll3",
};

const char *leftLegJoints[] = {
  "LHipYawPitch3",
  // "LHipRoll3",
  "LHipPitch3",
  "LKneePitch3",
  "LAnklePitch3",
  // "LAnkleRoll3",
};

const char *rightArmJoints[] = {
  "RShoulderPitch3",
  // "RShoulderRoll3",
  "RElbowYaw3",
  "RElbowRoll3",
  "RWristYaw3"
};

const char *leftArmJoints[] = {
  "LShoulderPitch3",
  // "LShoulderRoll3",
  "LElbowYaw3",
  "LElbowRoll3",
  "LWristYaw3"
};


Robot::Robot(int clientID, const char* name) :
  VRepClass(clientID, name) {
  int count = 0;
  simxInt *shapeHandles;
  log_response( simxGetObjects(clientID, sim_object_shape_type, &count, &shapeHandles, simx_opmode_blocking) );

  for (int i = 0; i < count; i++) {
    _shapes.push_back( Shape( clientID, shapeHandles[i]) );
  }

  for (int i = 0; i < legJointCount; i++) {
    _joints.push_back( Joint( clientID, rightLegJoints[i], false ) );
    _joints.push_back( Joint( clientID, leftLegJoints[i], false) );
  }
  for (int i = 0; i < armJointCount; i++) {
    _joints.push_back( Joint( clientID, rightArmJoints[i], false ) );
    _joints.push_back( Joint( clientID, leftArmJoints[i], false ) );
  }

  simxGetObjectPosition(_clientID, _handle, -1, _initialPosition, simx_opmode_blocking);
  simxGetObjectOrientation(_clientID, _handle, -1, _initialOrientation, simx_opmode_blocking);
}


void Robot::update() {
  simxPauseCommunication(_clientID, 1);
  for (Joint &joint : _joints) {
    joint.update();
  }
  simxPauseCommunication(_clientID, 0);

  // std::this_thread::sleep_for(std::chrono::milliseconds(5));
  simxSynchronousTrigger(_clientID);
}

void Robot::reset() {
  simxPauseCommunication(_clientID, 1);
  for ( Joint &j : _joints ) {
    j.reset();
  }
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

void Robot::setGenome(double genome[3*jointCount + 1]) {
  double T = genome[3*jointCount];
  for (int i = 0; i < jointCount; i++) {
    _joints[2*i].setJointStats(genome[3*i], genome[3*i + 1], genome[3*i + 2], T);
    _joints[2*i + 1].setJointStats(genome[3*i], genome[3*i + 1], genome[3*i + 2] + PI, T);
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
