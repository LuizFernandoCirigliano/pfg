#include "NAO.hpp"

NAO::NAO(int clientID) :
  _clientID(clientID) {
//    _hip = new Joint(_clientID, "RHipYawPitch3");
//    _hip->_enabled = false;
    _legHip = new JointPair(_clientID, "RHipPitch3", "LHipPitch3");
    _knee =  new JointPair(_clientID, "RKneePitch3", "LKneePitch3");
    _ankle = new JointPair(_clientID, "RAnklePitch3", "LAnklePitch3");

    _shoulder = new JointPair(_clientID, "RShoulderPitch3", "LShoulderPitch3");
}

void NAO::getNAOJoints(std::vector<JointInterface *> &jointVector) {
  // HIP
//  jointVector.push_back( _hip );

  // LEG
  jointVector.push_back( _legHip );
  jointVector.push_back( _knee );
  // jointVector.push_back( _ankle );

  // ARM
  jointVector.push_back( _shoulder );
}
