#include <vector>
#include <iostream>

#include <chrono>
#include <thread>

extern "C" {
  #include "extApi.h"
}

#include "joint.hpp"
#include "robot.hpp"

const int portNumber = 25000;

void reset_scene();

int main() {
  int clientID = simxStart("127.0.0.1", portNumber, true, true, 2000, 5);

  if (clientID == -1) {
    std::cout << "Failed to Connect" << std::endl;
    return -1;
  }

  Robot r(clientID, "NAO");

  while(true) {
    for(int i = 0; i < 50; i++) {
      r.update();
    }

    r.reset();
  }

  return 0;
}
