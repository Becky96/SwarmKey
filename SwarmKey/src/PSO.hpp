//
//  PSO.hpp
//  SwarmKey
//
//  Created by Becky Johnson on 24/01/2018.
//
//

#ifndef PSO_hpp
#define PSO_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Particle.hpp"


class PSO {
public:
    
    void setup();
    void run();
    void display();
    void fitness();
    void checkPersonalBest();
    void calculateBest();
    void updateParticles();
    void mean();
    void checkRepeat();
    void disturb();
    
    
    int repeated = 0;
    
    vector<Particle*> particles;
    int N = 20;
    float c1, c2;               //Learning rates
    float r1, r2;               //Stochastic elements
    Particle best;

};

#endif /* PSO_hpp */
