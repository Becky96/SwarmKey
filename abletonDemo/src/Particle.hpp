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
    
    
    //Variables for following intervals
    int bestIndFreqs[4];
    int indFreqs[4];
    float indFreqsVel[4];
    
    
    int start = int(ofRandom(29, 36));
    
    int fitness;
    int bestFit = 200000;
    
    ofColor col = ofColor(ofRandom(100, 255), ofRandom(10, 255), ofRandom(100, 255));
    
    Particle() {

        //Variables for following intervals
        indFreqs[0] = start;
        indFreqs[1] = start+int(ofRandom(0, 8));
        indFreqs[2] = start+int(ofRandom(0, 8));
        indFreqs[3] = start+int(ofRandom(0, 8));
        
        
        bestIndFreqs[0] = start;
        bestIndFreqs[1] = start+int(ofRandom(0, 8));
        bestIndFreqs[2] = start+int(ofRandom(0, 8));
        bestIndFreqs[3] = start+int(ofRandom(0, 8));
        
        //Frequency velocities
        indFreqsVel[0] = ofRandom(-2, 2);
        indFreqsVel[1] = ofRandom(-2, 2);
        indFreqsVel[2] = ofRandom(-2, 2);
        indFreqsVel[3] = ofRandom(-2, 2);
        
    }
    
    
    void display();
    
    
    
};
#endif /* Particle_hpp */
