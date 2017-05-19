#include <iostream>
#include <iomanip>
#include <string>

extern "C" {
  #include "extApi.h"
}

#include "robot.hpp"

const int portNumber = 25000;
Robot* r = 0;

int main(int argc, const char** argv) {
  const char *address = argc >= 2 ? argv[1] : "127.0.0.1";
  int clientID = simxStart(address, portNumber, true, true, 2000, 5);

  if (clientID == -1) {
    std::cout << "Failed to Connect" << std::endl;
    return -1;
  }

  r = new Robot(clientID, "NAO");
  
  //std::vector<float> params {236, -0.745, -0.237, -0.2, 1.62, 0.649, 0.12, 0.528, 0.336, -0.0941}; // RAMPA
  // std::vector<float> params {297, -0.830, -0.259, -0.203, 2.88, 0.508, 0.1430, 0.3750, 0.677, -0.0491}; // PLANO
  std::vector<float> params {295, -0.872,-0.117,-0.211,3.29,0.622,0.171,0.357,0.307,-0.0128};
  for (int i = 0; i < 1; i++) {
    std::string label = "teste_" + std::to_string(i);
    result res = r->runExperiment(params, 60*15.0, label);
    std::cout << "Tempo: " << res.time << ", dx: " << res.dx << ", dy: " << res.dy << std::endl;
  }

  simxStopSimulation(clientID, simx_opmode_oneshot);
  return 0;
}
