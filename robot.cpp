#include "robot.hpp"
#include <iostream>
#include <fstream>

#include <chrono>
#include <thread>

#include "helpers.hpp"

const int step_ms = 50;

Robot::Robot(int clientID, const char* name) :
  VRepClass(clientID, name)
{
  int count = 0;
  simxInt *shapeHandles;
  log_response( simxGetObjects(clientID, sim_object_shape_type, &count, &shapeHandles, simx_opmode_blocking) );

  for (int i = 0; i < count; i++) {
    _shapes.push_back( Shape( clientID, shapeHandles[i]) );
  }

  simxInt *jointHandles;
  log_response( simxGetObjects(clientID, sim_object_joint_type, &count, &jointHandles, simx_opmode_blocking) );

  for (int i = 0; i < count; i++) {
    _joints.push_back( Joint( clientID, jointHandles[i]) );
  }

  simxGetObjectPosition(_clientID, _handle, -1, _initialPosition, simx_opmode_blocking);
  simxGetObjectOrientation(_clientID, _handle, -1, _initialOrientation, simx_opmode_blocking);
}

void Robot::update()
{
  simxPauseCommunication(_clientID, 1);
  for (Joint &joint : _joints) {
    joint.update();
  }
  simxPauseCommunication(_clientID, 0);

  std::this_thread::sleep_for(std::chrono::milliseconds(step_ms));
}

void Robot::reset()
{
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
