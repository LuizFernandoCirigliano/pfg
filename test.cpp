#include <iostream>
#include <iomanip>

extern "C" {
  #include "extApi.h"
}

#include "robot.hpp"

const int portNumber = 25000;
Robot* r = 0;

int main(int argc, char const *argv[]) {
  int clientID = simxStart("127.0.0.1", portNumber, true, true, 2000, 5);

  if (clientID == -1) {
    std::cout << "Failed to Connect" << std::endl;
    return -1;
  }

  r = new Robot(clientID, "NAO");
  std::vector<double> params {0.51, 0.21, -0.36, 0.76, 0, 0.81, 0.23, 0.31, 1.57, 0.36};
  // std::vector<double> params {0.0, 0.0, 0, 0.0, 0, 0, 0.23, 0.31, 1.57, 0.36};
  std::cout << r->runExperiment(params) << std::endl;
  return 0;
}
