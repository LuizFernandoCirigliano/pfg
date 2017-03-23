#include "NAO.hpp"

NAO::NAO(int clientID) :
  _clientID(clientID) {
    _hip = new JointPair(_clientID, "RHipPitch3", "LHipPitch3", -1.53, 0.0);
    _knee =  new JointPair(_clientID, "RKneePitch3", "LKneePitch3", -0.09, 1.8);
    _ankle = new JointPair(clientID, "RAnklePitch3", "LAnklePitch3", -1.18, 0.93);
}

void NAO::getNAOJoints(std::vector<JointInterface *> &jointVector) {
  // HIP
  // jointVector.push_back( new Joint(clientID, "RHipYawPitch3", 0, -1.14, 0.74) );

  // LEG
  jointVector.push_back( _hip );
  jointVector.push_back( _knee );


  // ARM
  // jointVector.push_back( new JointPair(clientID, "RShoulderPitch3", "LShoulderPitch3", -2, 2) );
}
