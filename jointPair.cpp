#include "jointPair.hpp"

JointPair::JointPair(int clientID,
  const char *rightJointName,
  const char *leftJointName) {
    _rightJoint = new Joint(clientID, rightJointName);
    _leftJoint = new Joint(clientID, leftJointName);
  }


void JointPair::reset() {
  _leftJoint->reset();
  _rightJoint->reset();
}

void JointPair::update() {
  _leftJoint->update();
  _rightJoint->update();
}

void JointPair::setJointStats(double posAmp, double negAmp, double neutralAngle, double phase, double T) {
  _leftJoint->setJointStats(posAmp, negAmp, neutralAngle, phase, T);
  _rightJoint->setJointStats(posAmp, negAmp, neutralAngle, phase + 0.5, T);
}

void JointPair::prepareMoveToNeutralAngle(double T_ms) {
  _leftJoint->prepareMoveToNeutralAngle(T_ms);
  _rightJoint->prepareMoveToNeutralAngle(T_ms);
}

void JointPair::moveToNeutralAngle() {
  _leftJoint->moveToNeutralAngle();
  _rightJoint->moveToNeutralAngle();
}
