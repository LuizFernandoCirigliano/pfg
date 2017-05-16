#include "robot.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <cmath>
#include <string>

#include "helpers.hpp"
#include "NAO.hpp"


Robot::Robot(int clientID, const char* name) :
  VRepClass(clientID, name) {
  int count = 0;
  simxInt *shapeHandles;
  simxGetObjects(clientID, sim_object_shape_type, &count, &shapeHandles, simx_opmode_blocking);
  // simxGetObjects(clientID, sim_object_shape_type, &count, &shapeHandles, simx_opmode_blocking)

  for (int i = 0; i < count; i++) {
    _shapes.push_back( Shape( clientID, shapeHandles[i]) );
  }

  _nao = new NAO(clientID);
  _nao->getNAOJoints(_joints);
  _numJoints = _joints.size();

  simxGetObjectPosition(_clientID, _handle, -1, _initialPosition, simx_opmode_blocking);
  simxGetObjectOrientation(_clientID, _handle, -1, _initialOrientation, simx_opmode_blocking);

  simxSynchronous(clientID, true);
}

void Robot::update() {
  simxPauseCommunication(_clientID, 1);
  for (auto &joint : _joints) {
    joint->update();
  }
  float leftAnkleAngle = -1*(_nao->_legHip->_leftJoint->_currentAngle +
    _nao->_knee->_leftJoint->_currentAngle);
  float rightAnkleAngle = -1*(_nao->_legHip->_rightJoint->_currentAngle +
    _nao->_knee->_rightJoint->_currentAngle);

  _nao->_ankle->_leftJoint->setJointTargetPosition(leftAnkleAngle);
  _nao->_ankle->_rightJoint->setJointTargetPosition(rightAnkleAngle);

  float leftAnkleRollAngle = -1*_nao->_legHipRoll->_leftJoint->_currentAngle;
  float rightAnkleRollAngle = -1*_nao->_legHipRoll->_rightJoint->_currentAngle;

  _nao->_ankleRoll->_leftJoint->setJointTargetPosition(leftAnkleRollAngle);
  _nao->_ankleRoll->_rightJoint->setJointTargetPosition(rightAnkleRollAngle);

  simxPauseCommunication(_clientID, 0);

  simxSynchronousTrigger(_clientID);
}

void Robot::reset() {
  simxPauseCommunication(_clientID, 1);
  for ( auto &j : _joints ) {
    j->reset();
  }
  _nao->_ankle->reset();
  _nao->_ankleRoll->reset();

  simxPauseCommunication(_clientID, 0);


  for (int i = 0; i < 30; i++) {
    simxSynchronousTrigger(_clientID);
  }

  simxPauseCommunication(_clientID, 1);
  int posRet = simxSetObjectPosition(_clientID, _handle, -1,
                                     _initialPosition, simx_opmode_oneshot);
  int oriRet = simxSetObjectOrientation(_clientID, _handle, -1,
                                        _initialOrientation, simx_opmode_oneshot );

  for ( auto &s : _shapes ) {
    s.reset();
  }

  simxPauseCommunication(_clientID, 0);

  for (int i = 0; i < 30; i++) {
    simxSynchronousTrigger(_clientID);
  }
}

result Robot::runExperiment( const std::vector<float> &genome, const float time_s, const std::string label) {
  std::ofstream routefile;
  routefile.open("rotas/" + label + ".csv");
  this->setGenome(genome);
  this->reset();
  simxFloat position[3] = {0.0, 0.0, 0.0};
  simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
  simxFloat maxZ = 0.0;
  int i = 0;
  float num_ticks = time_s*1000/step_ms;
  for(; i < num_ticks; i++) {
    this->update();
    simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
    routefile << std::setprecision(3) << position[0] << ", " << position[1] << ", " << position[2] << std::endl;
    if (position[2] > maxZ) {
      maxZ = position[2];
    } else if (position[2] < maxZ - 0.15) {
      std::cout<< "Robot Fell" << std::endl;
      break;
    }
  }
  routefile.close();

  for (int j = 0; j < 5; j++) {
    simxSynchronousTrigger(_clientID);
  }
  float dx = position[0] - _initialPosition[0];
  float dy = position[1] - _initialPosition[1];
  float dist = sqrt(pow(dx, 2) + pow(dy, 2));
  float score = 1.0 + 15.0*i/600 + 10.0*dist + 20.0*fmax(dx, 0.0f);
  struct result r = {score, dx, dy, i*step_ms/1000.0f};
  
  return r;
}

std::vector< std::pair<float, float> > Robot::getAleles() {
  std::vector< std::pair<float, float> > alleles;

  alleles.push_back( std::make_pair( 200.0, 600.0) ); // T(ms)
  if (_nao->_legHip->_enabled) {
    alleles.push_back( std::make_pair( -1.0, -0.2 ) ); //A: Pos Amplitude
    alleles.push_back( std::make_pair( -0.4, 0.0 ) ); //B: Neg Amplitude
    alleles.push_back( std::make_pair( -0.5, 0.0 ) ); //Oc : Neutral Angle
  }

  if (_nao->_knee->_enabled) {
    alleles.push_back( std::make_pair( 0.2, 3.1) ); //C: pos amp
    alleles.push_back( std::make_pair( 0.0, 2) ); //Oj: neutral angle
    alleles.push_back( std::make_pair( 0.0, 0.5 ) ); //t: phase
  }

  if (_nao->_shoulder->_enabled) {
    alleles.push_back( std::make_pair( 0.0, 1.0) ); //D+: pos amp
    alleles.push_back( std::make_pair( 0.0, 1.0) ); //D-: neg amp
  }

  if (_nao->_legHipRoll->_enabled) {
    alleles.push_back( std::make_pair( 0.0, 0.5) ); // E: Pos amp
  }

  if (_nao->_elbow->_enabled) {
    alleles.push_back( std::make_pair( 0.0, 1) );
    alleles.push_back( std::make_pair( 0.0, 1.55) );
  }

  return alleles;
}

void Robot::setGenome(const std::vector<float> &genome) {
  int i = 0;
  float T = genome[i++];
  // std::cout << "T(ms): " << std::setprecision(2) << T << std::endl;

  if (_nao->_legHip->_enabled) {
    _nao->_legHip->setJointStats( genome[i], genome[i+1], genome[i+2], 0.0, T);
    i+=3;
  }

  if (_nao->_knee->_enabled) {
    _nao->_knee->setJointStats( genome[i], 0.0, genome[i+1], genome[i+2], T);
    i+=3;
  }

  if (_nao->_shoulder->_enabled) {
    _nao->_shoulder->setJointStats( genome[i], genome[i+1], 1.57, 0.0, T);
    i+=2;
  }

  if (_nao->_legHipRoll->_enabled) {
    _nao ->_legHipRoll->_leftJoint->setJointStats( genome[i], 0.0, 0.0, 0.0, T);
    _nao ->_legHipRoll->_rightJoint->setJointStats( -1*genome[i], 0.0, 0.0, 0.5, T);
    i++;
  }

  if (_nao->_elbow->_enabled) {
    _nao->_elbow->_leftJoint->setJointStats( -1*genome[i], -1*genome[i+1], 0.0, 0.0, T);
    _nao->_elbow->_rightJoint->setJointStats( genome[i], genome[i+1], 0.0, 0.5, T);
    i+=2;
  }
}

void Robot::printPosition(simxFloat* position) {
  simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
  std::cout << "[" << position[0] << ", " << position[1] << ", " << position[2] << "]" << std::endl;
}
