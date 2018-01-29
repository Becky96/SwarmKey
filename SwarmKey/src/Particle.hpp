//
//  Particle.hpp
//  SwarmKey
//
//  Created by Becky Johnson on 24/01/2018.
//
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "ofMain.h"

class Particle {
    
public:
    
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
    
    
    int fitness = 200000;
    int bestFitness = 200000;
    
    
    void determineRhythm();

    
    void fillRhythm(float duration);
    
    
    void setup();
    
};

#endif /* Particle_hpp */
