#include "robot.hpp"
#include <iostream>

#include <chrono>
#include <thread>
#include <iomanip>

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


  for (int i = 0; i < 30; i++) {
    simxSynchronousTrigger(_clientID);
  }
  
  simxPauseCommunication(_clientID, 1);
  int posRet = simxSetObjectPosition(_clientID, _handle, -1,
                                     _initialPosition, simx_opmode_oneshot);
  int oriRet = simxSetObjectOrientation(_clientID, _handle, -1,
                                        _initialOrientation, simx_opmode_oneshot);
  
  for ( auto &s : _shapes ) {
    s.reset();
  }

  simxPauseCommunication(_clientID, 0);
  
  for (int i = 0; i < 30; i++) {
    simxSynchronousTrigger(_clientID);
  }
}

result Robot::runExperiment( const std::vector<float> &genome ) {
  this->setGenome(genome);
  this->reset();
  simxFloat position[3] = {0.0, 0.0, 0.0};
  simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
  
  int i = 0;
  for(; i < 400; i++) {
    this->update();
    simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
    if (position[2] < 0.15) {
      std::cout<< "Robot Fell" << std::endl;
      break;
    }
  }
  for (int i = 0; i < 5; i++) {
    simxSynchronousTrigger(_clientID);
  }
  float dx = position[0] - _initialPosition[0];
  float dy = position[1] - _initialPosition[1];
  float score = 1.0 + 20.0*i/400.0 + 30.0*dx;
  struct result r = {score, dx, dy, i*step_ms/1000.0f};
  return r;
}

std::vector< std::pair<float, float> > Robot::getAleles() {
  std::vector< std::pair<float, float> > alleles;

  alleles.push_back( std::make_pair( 200.0, 2000.0) ); // T(ms)
  if (_nao->_legHip->_enabled) {
    alleles.push_back( std::make_pair( -0.78, 0.0 ) ); //A: Pos Amplitude
    alleles.push_back( std::make_pair( -0.40, 0.0 ) ); //B: Neg Amplitude
    alleles.push_back( std::make_pair( -0.40, 0.0 ) ); //Oc : Neutral Angle
  }

  if (_nao->_knee->_enabled) {
    alleles.push_back( std::make_pair( 0.0, 0.78) ); //C: pos amp
    alleles.push_back( std::make_pair( 0.0, 0.7) ); //Oj: neutral angle
    alleles.push_back( std::make_pair( 0.0, 0.5 ) ); //t: phase
  }

  if (_nao->_shoulder->_enabled) {
    alleles.push_back( std::make_pair( 0.0, 0.5) ); //D+: pos amp
    alleles.push_back( std::make_pair( 0.0, 0.5) ); //D-: neg amp
  }
  return alleles;
}

void Robot::setGenome(const std::vector<float> &genome) {
  int i = 0;
  float T = genome[i++];
  std::cout << "T(ms): " << std::setprecision(2) << T << std::endl;

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

}

void Robot::printPosition(simxFloat* position) {
  simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
  std::cout << "[" << position[0] << ", " << position[1] << ", " << position[2] << "]" << std::endl;
}
