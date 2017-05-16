#include <iostream>
#include <fstream>
#include <iomanip>
#include <string> 

extern "C" {
  #include "extApi.h"
}

#include <ga/ga.h>
#define INSTANTIATE_REAL_GENOME
#include <ga/GARealGenome.h>

#include "robot.hpp"

const int portNumber = 25000;
unsigned seed = 100;

std::ofstream csv;

Robot* r = 0;

float Objective(GAGenome &);

int main(int argc, const char** argv) {
  const char *address = argc == 2 ? argv[1] : "127.0.0.1";
  int clientID = simxStart(address, portNumber, true, true, 2000, 5);
//  simxStartSimulation(clientID, simx_opmode_blocking);

  if (clientID == -1) {
    std::cout << "Failed to Connect" << std::endl;
    return -1;
  }

  csv.open("ga.csv");
  csv << "T, A, B, Oc, C, Oj, tj, Dp, Dn, E, t1, dx1, dy1, s1, t2, dx2, dy2, s2, t3, dx3, dy3, s3, savg" << std::endl;
  r = new Robot(clientID, "NAO");

  // GARandomSeed(seed);
  GARealAlleleSetArray alleles;
  for (auto &pair : r->getAleles()) {
    alleles.add(pair.first, pair.second);
  }

  GARealGenome genome(alleles, Objective);
  genome.crossover(GARealBlendCrossover);

  // GAParameterList params;
  // GASteadyStateGA::registerDefaultParameters(params);
  // params.set(gaNnGenerations, 20);
  // params.set(gaNpopulationSize, 150);
  // params.set(gaNscoreFrequency, 5);
  // params.set(gaNflushFrequency, 1);
  // params.set(gaNpMutation, 0.05);
  // params.set(gaNselectScores, (int)GAStatistics::AllScores);

  GASteadyStateGA ga1(genome);
  ga1.populationSize(150);
  ga1.nGenerations(20);
  // ga1.scoreFilename("bog.dat");
  // ga1.scoreFrequency(1);	
  // ga1.flushFrequency(1);
  // ga1.selectScores((int)GAStatistics::AllScores);
  ga1.pReplacement(0.8);
  ga1.pMutation(0.05);
  ga1.evolve(seed);

  std::cout << "************************" << std::endl;
  std::cout << ga1.statistics() << std::endl;
  std::cout << "the ga generated:\n" << ga1.statistics().bestIndividual() << std::endl;

  csv.close();
  delete r;
  return 0;
}

int genome_number = 0;

float Objective(GAGenome& g) {
  genome_number++;
  GARealGenome& genome = (GARealGenome&)g;
  std::vector<float> params;
  for(int i=0; i<genome.length(); i++){
    params.push_back(genome.gene(i));
    csv << std::setprecision(3) << params[i] << ", ";
  }
  float avgScore = 0.0;
  for(int i=0; i< 3; i++) {
    std::string label = std::to_string(genome_number) + "_" + std::to_string(i);
    result res = r->runExperiment(params, 15.0, label);
    avgScore += res.score;
    csv << std::setprecision(2) << res.time << ", ";
    csv << std::setprecision(2) << res.dx << ", ";
    csv << std::setprecision(2) << res.dy << ", ";
    csv << std::setprecision(2) << res.score << ", ";
  }
  avgScore = avgScore/3.0;
  csv << std::setprecision(2) << avgScore << std::endl;
  std::cout << std::setprecision(2) << avgScore << std::endl;
  return avgScore;
}
