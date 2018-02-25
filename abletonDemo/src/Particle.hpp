//
//  Particle.hpp
//  abletonDemo
//
//  Created by Becky Johnson on 30/01/2018.
//
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "ofMain.h"

class Particle {
    
public:
    
    void setupParticle();
    
    //Variables for following intervals
    int bestIndFreqs[4];
    int indFreqs[4];
    float indFreqsVel[4];
    
    
    int start = int(ofRandom(29, 36));
    
    int fitness;
    int bestFit = 200000;
    
    ofColor col = ofColor(ofRandom(100, 255), ofRandom(10, 255), ofRandom(100, 255));
    
    //Rhythm
    vector<float> rhythm;
    vector<float> rhythmVels;
    vector<int> hits;
    vector<float> bestRhythm;
    
    
    vector<float> compareRhythm;
    vector<float> compareRhythmVels;
    vector<float> compareBestRhythm;
    
    float validDurations[5] = {4, 2, 1, 0.5, 0.25};
    
    int dimensionality;
    float dimensionalityVel = ofRandom(-2, 2);
    
    int bestDimensionality;
    
    
    int fitnessRhythm = 200000;
    int bestFitnessRhythm = 200000;
    
    
    void determineRhythm();
    void fillRhythm(float duration);

    
    int currentOctave;
    
    
    //Velocity
    int velocityFitness;                    //Rate strength of particle's velocity
    int velocity;                           //Specific particle velocity
    int velocityVel = ofRandom(-2, 2);      //Rate to accelerate particle's velocity
    int bestParticleVelocityFitness = 999;                       //Particle's personal best velocity
    int bestParticleVelocity;
    
    
    
    
};
#endif /* Particle_hpp */
