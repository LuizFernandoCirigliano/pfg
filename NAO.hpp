#ifndef NAO_H
#define  NAO_H

#include <vector>
#include "joint.hpp"
#include "jointPair.hpp"

class NAO {
public:
  int _clientID;
  JointPair *_knee;
  JointPair *_ankle;
  JointPair *_hip;
  NAO(int clientID);
  void getNAOJoints(std::vector<JointInterface *> &jointVector);
};

#endif
