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
  //                            T       A      B      Oc      C    Oj     t     D+    D-
  // std::vector<float> params = {7.5e+02, -0.069, -0.13, -0.072, 0.55, 0.39, -0.44, 0.1, 0.55};
  // std::vector<float> params = {7.6e+02, 0.089, 0.11, -0.49, 0.46, 1, -0.16, 0.42, 0.24};
  // std::vector<float> params = {270, -0.51, -0.21, -0.36, 0.72, 0.81, -0.36, 0.23, 0.31};
  // std::vector<float> params {7e+02, -0.23, -0.047, -0.29, 0.94, 0.5, 0.43, 0.89, 0.29};
  // std::vector<float> params {380, -0.25, -0.14, -0.079, 0.59, 0.66, 0.13, 0.22, 0.4};
//  std::vector<float> params {880, -0.2, -0.2, -0.16, 0.33, 0.44, 0.19, 0.30, 0.30};
//   std::vector<float> params {1.28e+03, -0.61, -0.209, -0.00254, 0.751, 0.478, 0.355, 0.478, 0.168};
//    std::vector<float> params {1.00e+03, -0.5, 0, 0, 0, 0, 0, 0.5, 0.5};
  
  std::vector<float> params {150, -0.138, -0.4, -0.316, 0.727, 0.523, 0.229, 0.125, 0.5};
//  std::vector<float> params {267, -0.22, -0.194, -0.254, 0.414, 0.523, 0.13, 0.201, 0.335};
  for (int i = 0; i < 10; i++) {
    result res = r->runExperiment(params);
    std::cout << res.time << " " << res.dx << " " << res.dy << " " << res.score << std::endl;
  }

  simxStopSimulation(clientID, simx_opmode_oneshot);
  return 0;
}
