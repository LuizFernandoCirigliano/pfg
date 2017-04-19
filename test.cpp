#include <iostream>
#include <iomanip>

extern "C" {
  #include "extApi.h"
}

#include "robot.hpp"

const int portNumber = 25000;
Robot* r = 0;

int main(int argc, const char** argv) {
  const char *address = argc == 2 ? argv[1] : "127.0.0.1";
  int clientID = simxStart(address, portNumber, true, true, 2000, 5);

  if (clientID == -1) {
    std::cout << "Failed to Connect" << std::endl;
    return -1;
  }

  r = new Robot(clientID, "NAO");
  
//  std::vector<float> params {267, -0.22, -0.194, -0.254, 0.414, 0.523, 0.13, 0.201, 0.335};
//  std::vector<float> params {480, -0.378, -0.202, -0.315, 0.676, 0.58, 0.228, 0.33,  0};
  for (int i = 0; i < 10; i++) {
    result res = r->runExperiment(params);
    std::cout << res.time << " " << res.dx << " " << res.dy << " " << res.score << std::endl;
  }

  simxStopSimulation(clientID, simx_opmode_oneshot);
  return 0;
}
