#include "jointPair.hpp"

JointPair::JointPair(int clientID, const char *rightJointName,
  const char *leftJointName, double lb, double ub) {
    _leftJoint = new Joint(clientID, leftJointName, 0.5, lb, ub);
    _rightJoint = new Joint(clientID, rightJointName, 0.0, lb, ub);
    _upperBound = ub;
    _lowerBound = lb;
  }


void JointPair::reset() {
  _leftJoint->reset();
  _rightJoint->reset();
}

void JointPair::update() {
  _leftJoint->update();
  _rightJoint->update();
}

void JointPair::setJointStats(double posAmp, double negAmp, double zeroAngle) {
  _leftJoint->setJointStats(posAmp, negAmp, zeroAngle);
  _rightJoint->setJointStats(posAmp, negAmp, zeroAngle);
}
