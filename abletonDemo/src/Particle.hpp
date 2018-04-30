//  SWARM KEY
//  Particle.hpp


#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "ofMain.h"

class Particle {
    
public:
    
    void setupParticle();
    
    //////////////////////////////
    //////////NOTES///////////////
    //////////////////////////////

    //Variables for following intervals
    int bestIndFreqs[16];
    int indFreqs[16];
    float indFreqsVel[16];
    
    

    
    int start = int(ofRandom(17, 25));
    
    int fitness;
    int bestFit = 200000;
    int currentOctave;

    
    
    //////////////////////////////
    //////////RHYTHM//////////////
    //////////////////////////////
    vector<float> rhythm;
    vector<float> rhythmVels;
    vector<int> hits;
    vector<float> bestRhythm;
    
    
    vector<float> compareRhythm;
    vector<float> compareRhythmVels;

    
    float validDurations[5] = {4, 2, 1, 0.5, 0.25};
    
    int dimensionality;
    float dimensionalityVel = ofRandom(-2, 2);
    int bestDimensionality;
    
    
    int fitnessRhythm = 200000;
    int bestFitnessRhythm = 200000;
    
    
    void determineRhythm();
    void fillRhythm(float duration);

    
    
    /////////////////////////////////
    ////////////VELOCITY/////////////
    /////////////////////////////////
    int velocityFitness;                    //Rate strength of particle's velocity
    int velocity;                           //Specific particle velocity
    float velocityVel = ofRandom(-0.5, 0.5);      //Rate to accelerate particle's velocity
    int bestParticleVelocityFitness = 999;                       //Particle's personal best velocity
    int bestParticleVelocity;
 
    
    int maxVelocity = 120;
    
    
    
    
    
};
#endif /* Particle_hpp */
