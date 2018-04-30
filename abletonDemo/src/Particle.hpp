//  SWARM KEY
//  Particle.hpp


#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "ofMain.h"

//--------------------------------------------------------------
class Particle {
    
public:
    
    //Initialising particle with randomised note sequence, rhythm sequence, and velocity
    void setupParticle();

    //NOTE SEQUENCES
    int bestIndFreqs[16];                           //Best found note sequence
    int indFreqs[16];                               //Note sequence
    float indFreqsVel[16];                          //Velocitys to update note seuqnece
    int start = int(ofRandom(17, 25));              //The first note of the sequence is initialised within this range of the available notes vector
    int fitness;                                    //Fitness of the particle's note sequence
    int bestFit = 200000;                           //Particles best note sequence fitness
    int currentOctave;                              //Current octave the swarm is targetting

    //RHYTHM//
    vector<float> rhythm;                           //Vector containing rhythm sequence
    vector<float> rhythmVels;                       //Vector containing velocitys to update rhythm sequence
    vector<int> hits;                               //Hit serves as a binary representation of the rhythm sequence
    vector<float> bestRhythm;                       //Particle's best rhythm sequence
    float validDurations[5] = {4, 2, 1, 0.5, 0.25}; //Valid note duratons that can be used in the particles' rhythm sequence
    int dimensionality;                             //Dimensionality represents the number of values in the rhythm sequence
    float dimensionalityVel = ofRandom(-2, 2);      //The velocity of the dimensionality is used in the PSO update equation
    int bestDimensionality;                         //Best dimensionality of the best-found rhythm sequence
    int fitnessRhythm = 200000;                     //Current rhythm fitness
    int bestFitnessRhythm = 200000;                 //Best found rhythm fitness
    void determineRhythm();                         //Create initial rhythm sequence
    
    //VELOCITY//
    int velocityFitness;                            //Rate strength of particle's velocity
    int velocity;                                   //Specific particle velocity
    float velocityVel = ofRandom(-0.5, 0.5);        //Rate to accelerate particle's velocity
    int bestParticleVelocityFitness = 999;          //Particle's personal best velocity
    int bestParticleVelocity;                       //Best found particle velocity
    int maxVelocity = 120;                          //Maximum velocity
    
};
//--------------------------------------------------------------
#endif /* Particle_hpp */
