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
  
  std::vector<float> params {297, -0.830, -0.259, -0.203, 2.88, 0.508, 0.1430, 0.3750, 0.677, -0.0491};
  for (int i = 0; i < 10; i++) {
    result res = r->runExperiment(params);
    std::cout << res.time << " " << res.dx << " " << res.dy << " " << res.score << std::endl;
  }

  simxStopSimulation(clientID, simx_opmode_oneshot);
  return 0;
}
