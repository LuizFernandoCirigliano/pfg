#include "robot.hpp"
#include <iostream>

#include <chrono>
#include <thread>

#include "helpers.hpp"

const int step_ms = 50;

const char* rightLegJoints[] = {
  "RHipYawPitch3",
  "RHipRoll3",
  "RHipPitch3",
  "RKneePitch3",
  "RAnklePitch3",
  "RAnkleRoll3",
  "NAO_RFsrRR",
  "NAO_RFsrFR",
  "NAO_RFsrRL",
  "NAO_RFsrFL",
};

const char *leftLegJoints[] = {
  "LHipYawPitch3",
  "LHipRoll3",
  "LHipPitch3",
  "LKneePitch3",
  "LAnklePitch3",
  "LAnkleRoll3",
  "NAO_LFsrRR",
  "NAO_LFsrFR",
  "NAO_LFsrRL",
  "NAO_LFsrFL",
};

const char *rightArmJoints[] = {
  "RShoulderPitch3",
  "RShoulderRoll3",
  "RElbowYaw3",
  "RElbowRoll3",
  "RWristYaw3"
};

const char *leftArmJoints[] = {
  "LShoulderPitch3",
  "LShoulderRoll3",
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

  for (int i = 0; i < 10; i++) {
    _joints.push_back( Joint( clientID, rightLegJoints[i], 0.0 ) );
    _joints.push_back( Joint( clientID, leftLegJoints[i], 0.5 ) );
  }
  for (int i = 0; i < 5; i++) {
    _joints.push_back( Joint( clientID, rightArmJoints[i], 0.0 ) );
    _joints.push_back( Joint( clientID, leftArmJoints[i], 0.5 ) );
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

  std::this_thread::sleep_for(std::chrono::milliseconds(step_ms));
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

void Robot::setGenome(double genome[45]) {
  for (int i = 0; i < 15; i++) {
    _joints[2*i].setJointStats(genome[3*i], genome[3*i + 1], genome[3*i + 2]);
    _joints[2*i + 1].setJointStats(genome[3*i], genome[3*i + 1], genome[3*i + 2]);
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
