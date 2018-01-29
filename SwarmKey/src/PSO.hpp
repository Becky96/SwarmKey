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
#include "ofxDatGui.h"


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
    void resetParticles();
    
    
    int chosenDimension = 1;
    
    void determineRhythm();
    
    int repeated = 0;
    
    vector<Particle*> particles;
    int N = 15;
    float con = 0.8984;         //Constriction factor

    float c1, c2;               //Learning rates
    float r1, r2;               //Stochastic elements
    Particle* best;
    
    int dt = 0.1;
    
    
    void createSequence(int d, Particle * p);
    int createNewDur(int startIndex, int endIndex);
    
        

    float bestFitness = 2000000.;
    
    float semibrevePen = 10000;
    float minimPen = 100;
    float crotchetPen = 50;
    float quaverPen = 25;
    float semiquaverPen = 25;
    
    float validDurations[5] = {4, 2, 1, 0.5, 0.25};

    vector<float> validDur;


};

#endif /* PSO_hpp */
