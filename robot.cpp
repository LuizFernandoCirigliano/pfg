#include "robot.hpp"
#include <iostream>

#include <chrono>
#include <thread>
#include <iomanip>

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
  double leftAnkleAngle = -1*(_nao->_legHip->_leftJoint->_currentAngle +
    _nao->_knee->_leftJoint->_currentAngle);
  double rightAnkleAngle = -1*(_nao->_legHip->_rightJoint->_currentAngle +
    _nao->_knee->_rightJoint->_currentAngle);

  _nao->_ankle->_leftJoint->setJointTargetPosition(leftAnkleAngle);
  _nao->_ankle->_rightJoint->setJointTargetPosition(rightAnkleAngle);

  simxPauseCommunication(_clientID, 0);

  // std::this_thread::sleep_for(std::chrono::milliseconds(step_ms));
  simxSynchronousTrigger(_clientID);
}

void Robot::reset() {
  simxPauseCommunication(_clientID, 1);

  int posRet = simxSetObjectPosition(_clientID, _handle, -1,
    _initialPosition, simx_opmode_oneshot);
  int oriRet = simxSetObjectOrientation(_clientID, _handle, -1,
    _initialOrientation, simx_opmode_oneshot);

  for ( auto &s : _shapes ) {
    s.reset();
  }

  simxPauseCommunication(_clientID, 0);


  // std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  for (int i = 0; i < 20; i++) {
    simxSynchronousTrigger(_clientID);
  }

  simxPauseCommunication(_clientID, 1);
  for ( auto &j : _joints ) {
    j->reset();
  }
  _nao->_ankle->reset();
  simxPauseCommunication(_clientID, 0);

  // std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  for (int i = 0; i < 20; i++) {
    simxSynchronousTrigger(_clientID);
  }
}

void Robot::moveToNeutralAngle() {
  double T = 500;
  int num_updates = T/dt_ms;
  for ( auto &j : _joints) {
    j->prepareMoveToNeutralAngle(T);
  }
  for (int i = 0; i < num_updates; i++) {
    for ( auto &j : _joints) {
      j->moveToNeutralAngle();
    }
    simxSynchronousTrigger(_clientID);
  }
}

double Robot::runExperiment( const std::vector<double> &genome ) {
  this->setGenome(genome);
  this->reset();
  simxFloat position[3];
  simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
  // this->moveToNeutralAngle();
  int i = 0;
  for(; i < 400; i++) {
    this->update();
    simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
    if (position[2] < 0.15) {
      std::cout<< "Robot Fell" << std::endl;
      break;
    }
  }
  double score = 10 + 20.0*i/400 + 30*this->getXDistance();
  this->reset();
  return score;
}

std::vector< std::pair<double, double> > Robot::getAleles() {
  std::vector< std::pair<double, double> > alleles;

  // if (_hip->enabled) {
  //   alleles.push_back( make_pair( ) );
  // }
  alleles.push_back( std::make_pair( 100.0, 1500.0) ); // T(ms)
  if (_nao->_legHip->_enabled) {
    alleles.push_back( std::make_pair( 0.01, 1.00 ) ); //A: Pos Amplitude
    alleles.push_back( std::make_pair( 0.01, 1.00 ) ); //B: Neg Amplitude
    alleles.push_back( std::make_pair( -1.00, 0.00 ) ); //Oc : Neutral Angle
  }

  if (_nao->_knee->_enabled) {
    alleles.push_back( std::make_pair( 0.0, 1.5) ); //C: pos amp
    alleles.push_back( std::make_pair( 0.0, 1.5) ); //Oj: neutral angle
    alleles.push_back( std::make_pair( -0.9, -0.1 ) ); //t: phase
  }

  if (_nao->_shoulder->_enabled) {
    alleles.push_back( std::make_pair( 0.0, 1.0) ); //D+: pos amp
    alleles.push_back( std::make_pair( 0.0, 1.0) ); //D-: neg amp
  }
  return alleles;
}

void Robot::setGenome(const std::vector<double> &genome) {
  int i = 0;
  double T = genome[i++];
  std::cout << "T(ms): " << std::setprecision(2) << T << std::endl;
  // if (_hip->enabled) {
  //   _hip->setJointStats( genome[i++]);
  // }
  if (_nao->_legHip->_enabled) {
    _nao->_legHip->setJointStats( genome[i], genome[i+1], genome[i+2], 0.0, T);
    i+=3;
  }
  if (_nao->_knee->_enabled) {
    _nao->_knee->setJointStats( genome[i], 0.0, genome[i+1], genome[i+2], T);
    i+=3;
  }
  if (_nao->_shoulder->_enabled) {
    _nao->_shoulder->setJointStats( genome[i], genome[i+1], 1.57, 0.5, T);
    i+=2;
  }

}

double Robot::getXDistance() {
  simxFloat position[3];
  simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
  return position[0] - _initialPosition[0];
}

void Robot::printPosition(simxFloat* position) {
  simxGetObjectPosition(_clientID, _handle, -1, position, simx_opmode_streaming);
  std::cout << "[" << position[0] << ", " << position[1] << ", " << position[2] << "]" << std::endl;
}
