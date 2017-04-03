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

float Objective(GAGenome &);

int main() {
  int clientID = simxStart("127.0.0.1", portNumber, true, true, 2000, 5);

  if (clientID == -1) {
    std::cout << "Failed to Connect" << std::endl;
    return -1;
  }

  r = new Robot(clientID, "NAO");
  simxSynchronous(clientID, true);

  GARealAlleleSetArray alleles;
  for (auto &pair : r->getAleles()) {
    alleles.add(pair.first, pair.second);
  }

  GARealGenome genome(alleles, Objective);

  GAParameterList params;
  GASteadyStateGA::registerDefaultParameters(params);
  params.set(gaNnGenerations, 50);
  params.set(gaNpopulationSize, 100);
  params.set(gaNscoreFrequency, 5);
  params.set(gaNflushFrequency, 50);
  params.set(gaNselectScores, (int)GAStatistics::AllScores);
  // params.parse(argc, argv, gaFalse);

  GASteadyStateGA ga1(genome);
  ga1.parameters(params);
  ga1.set(gaNscoreFilename, "bog1.dat");
  std::cout << "\nrunning ga number 1 (alternate allele(0) and allele(3))..."<< std::endl;
  ga1.evolve();

  std::cout << "************************" << std::endl;
  std::cout << ga1.statistics() << std::endl;
  std::cout << "the ga generated:\n" << ga1.statistics().bestIndividual() << std::endl;

  delete r;
  return 0;
}


float Objective(GAGenome& g) {
  GARealGenome& genome = (GARealGenome&)g;
  std::vector<double> params;
  for(int i=0; i<genome.length(); i++){
    params.push_back(genome.gene(i));
  }
  std::cout << std::endl;
  double score = r->runExperiment(params);
  std::cout << "SCORE: " << score << std::endl;
  return score;
}
