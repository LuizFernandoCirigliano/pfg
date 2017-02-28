#include <iostream>
#include <iomanip>

extern "C" {
  #include "extApi.h"
}

#include <ga/ga.h>
#define INSTANTIATE_REAL_GENOME
#include <ga/GARealGenome.h>

#include "robot.hpp"

const int portNumber = 25000;
Robot* r = 0;

void reset_scene();
float Objective(GAGenome &);

int main() {
  int clientID = simxStart("127.0.0.1", portNumber, true, true, 2000, 5);

  if (clientID == -1) {
    std::cout << "Failed to Connect" << std::endl;
    return -1;
  }

  r = new Robot(clientID, "NAO");
  GARealAlleleSet alleles2(-2, 2);
  int length = 45;
  GARealGenome genome(length, alleles2, Objective);

  GAParameterList params;
  GASteadyStateGA::registerDefaultParameters(params);
  params.set(gaNnGenerations, 500);
  params.set(gaNpopulationSize, 5);
  params.set(gaNscoreFrequency, 10);
  params.set(gaNflushFrequency, 50);
  params.set(gaNselectScores, (int)GAStatistics::AllScores);
  // params.parse(argc, argv, gaFalse);

  GASteadyStateGA ga1(genome);
  ga1.parameters(params);
  ga1.set(gaNscoreFilename, "bog1.dat");
  std::cout << "\nrunning ga number 1 (alternate allele(0) and allele(3))..."<< std::endl;
  ga1.evolve();
  std::cout << "the ga generated:\n" << ga1.statistics().bestIndividual() << std::endl;

  delete r;
  return 0;
}


float Objective(GAGenome& g) {
  GARealGenome& genome = (GARealGenome&)g;
  double params[45];
  for(int i=0; i<genome.length(); i++){
    std::cout << std::setprecision(2) << genome.gene(i) << " ";
    params[i] = genome.gene(i);
  }
  std::cout << std::endl;
  r->setGenome(params);
  r->reset();

  for(int i = 0; i < 100; i++) {
    r->update();
  }
  float score = 1 + r->getXDistance();
  std::cout << "Score: " << score << std::endl;
  return score;
}
