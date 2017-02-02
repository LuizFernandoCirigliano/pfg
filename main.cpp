#include <vector>
#include <iostream>

#include <chrono>
#include <thread>

extern "C" {
  #include "extApi.h"
}

#include "nao.h"
#include "joint.h"

int portNumber = 25000;

int main() {
  int clientID = simxStart("127.0.0.1", portNumber, true, true, 2000, 5);

  if (clientID == -1) {
    std::cout << "Failed to Connect" << std::endl;
    return -1;
  }

  std::vector<Joint> joints;

  for(int i = 0; i < armJointCount; i++) {
    joints.push_back( Joint(clientID, rightArmJoints[i], -1, 1, 0.01, -1) );
    joints.push_back( Joint(clientID, leftArmJoints[i], -1, 1, 0.01, -1) );
  }

  while (true) {
    for (Joint &joint : joints) {
      joint.update();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  return 0;
}
