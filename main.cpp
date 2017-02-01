#include <vector>
#include <iostream>

#include <chrono>
#include <thread>

extern "C" {
  #include "extApi.h"
}

#include "nao.h"

int portNumber = 25000;

int main() {
  int clientID = simxStart("127.0.0.1", portNumber, true, true, 2000, 5);

  if (clientID == -1) {
    std::cout << "Failed to Connect" << std::endl;
    return -1;
  }

  simxInt rightArmShapesHandles[legShapeCount];
  simxInt rightArmJointsHandles[legJointCount];

  for(int i = 0; i < armJointCount; i++) {
    simxGetObjectHandle(clientID, rightArmJoints[i],
      &rightArmJointsHandles[i], simx_opmode_oneshot_wait);
  }

  for(int i = 0; i < armShapeCount; i++) {
    simxGetObjectHandle(clientID, rightArmShapes[i],
      &rightArmShapesHandles[i], simx_opmode_oneshot_wait);
  }

  float angle = 0;
  bool ascending = true;

  while (true) {
    for (simxInt &joint : rightArmJointsHandles) {
      simxSetJointTargetPosition(clientID, joint, angle, simx_opmode_oneshot);
    }

    if ( (angle <= -3 && !ascending) || (angle >= 3.1 && ascending) ) {
      ascending = !ascending;
    }

    if (ascending) {
      angle += 0.1;
    } else {
      angle -= 0.1;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  return 0;
}
