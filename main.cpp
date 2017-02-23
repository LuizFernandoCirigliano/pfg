#include <iostream>

extern "C" {
  #include "extApi.h"
}

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

  double genome[45] = {0};
  while(true) {
    for(int i = 0; i < 100; i++) {
      r.update();
    }
    for (int i = 0; i < 45; i++) {
      genome[i] = (((double)(rand() % 100)) / 100) * 3;
    }
    r.setGenome(genome);
    r.reset();
  }

  return 0;
}
