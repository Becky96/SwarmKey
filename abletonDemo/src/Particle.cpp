//
//  Particle.cpp
//  abletonDemo
//
//  Created by Becky Johnson on 30/01/2018.
//
//

#include "Particle.hpp"


void Particle::setupParticle() {
    
    
    //////////////////////////////
    //////////NOTES///////////////
    //////////////////////////////
    
    //Variables for following intervals
    indFreqs[0] = start;
    bestIndFreqs[0] = start;

    
    for (int i = 1; i < 16; i++) {
        indFreqs[i] = start+int(ofRandom(0, 8));
        bestIndFreqs[i] = start+int(ofRandom(0, 8));
        indFreqsVel[i] = ofRandom(-3, 3);
    }

    
    
    
    
    //////////////////////////////
    //////////RHYTHM//////////////
    //////////////////////////////
    determineRhythm();
    
    for (int i = 0; i < rhythm.size(); i++) {
        bestRhythm.push_back(0);
    }
    
    
    
    
    //////////////////////////////
    //////////VELOCITY////////////
    //////////////////////////////
    velocity = int(ofRandom(20, maxVelocity));
}


//--------------------------------------------------------------
void Particle::determineRhythm() {
            
            
            dimensionality = 0;
            rhythm.clear();
            hits.clear();
            
            int r = int(ofRandom(0, 5));
            
            
            rhythm.push_back(validDurations[r]);

            
            float sum = rhythm[0];
            
            while (sum != 4) {
                
                if (sum >= 3.75) {
                    
                    r = 4;
                    rhythm.push_back(validDurations[r]);
                    
                    
                } else if (sum > 3 && sum <= 3.5) {
                    
                    r = int(ofRandom(3, 5));
                    rhythm.push_back(validDurations[r]);
                    
                    
                } else if (sum > 2 && sum <= 3) {
                    
                    r = int(ofRandom(2, 5));
                    rhythm.push_back(validDurations[r]);
                    
                    
                    
                } else if (sum > 0 && sum <= 2) {
                    
                    r = int(ofRandom(1, 5));
                    rhythm.push_back(validDurations[r]);
                    
                }

                sum = 0;
                for (int i = 0; i < rhythm.size(); i++) {
                    
                    sum+=rhythm[i];
                    
                }
                
                dimensionality++;
                
                
            }
            
            
            
            //Fill in hit sequence
            for (int i = 0; i < rhythm.size(); i++) {
                
   
                if (rhythm[i] == 0.25) {
                    
                    hits.push_back(1);
                    compareRhythm.push_back(0.25);
                    
                } else if (rhythm[i] == 0.5) {
                    hits.push_back(1);
                    hits.push_back(0);
                    
                    for (int j = 0; j < 2; j++) {
                        compareRhythm.push_back(0.5);
                    }
                    
                    
                } else if (rhythm[i] == 1) {
                    hits.push_back(1);
                    hits.push_back(0);
                    hits.push_back(0);
                    hits.push_back(0);
                    
                    for (int j = 0; j < 4; j++) {
                        compareRhythm.push_back(1);
                    }
                    
                } else if (rhythm[i] == 2) {
                    hits.push_back(1);
                    hits.push_back(0);
                    hits.push_back(0);
                    hits.push_back(0);
                    
                    hits.push_back(0);
                    hits.push_back(0);
                    hits.push_back(0);
                    hits.push_back(0);
                    
                    
                    for (int j = 0; j < 8; j++) {
                        compareRhythm.push_back(2);
                    }
                    
                    
                    
                } else if (rhythm[i] == 4) {
                    
                    hits.push_back(1);
                    hits.push_back(0);
                    hits.push_back(0);
                    hits.push_back(0);
                    
                    hits.push_back(0);
                    hits.push_back(0);
                    hits.push_back(0);
                    hits.push_back(0);
                    
                    hits.push_back(0);
                    hits.push_back(0);
                    hits.push_back(0);
                    hits.push_back(0);
                    
                    hits.push_back(0);
                    hits.push_back(0);
                    hits.push_back(0);
                    hits.push_back(0);
                    
                    
                    for (int j = 0; j < 16; j++) {
                        compareRhythm.push_back(4);
                    }
                    
                }
            }
            
            
            for (int i = 0; i < compareRhythm.size(); i++) {
                compareRhythmVels.push_back(ofRandom(-2, 2));
            }
        }
         

//--------------------------------------------------------------

void Particle::fillRhythm(float duration) {
             
             if (duration == 4) {
                 
                 rhythm.push_back(4);
                 hits.push_back(1);
                 
                 for (int j = 0; j < 15; j++) {
                     hits.push_back(0);
                 }
                 
                 
                 for (int j = 0; j < 16; j++) {
                     compareRhythm.push_back(4);
                 }
                 
                 
             }
    
             if (duration == 2) {
                 
                 rhythm.push_back(2);
            
                 hits.push_back(1);
                 hits.push_back(0);
                 hits.push_back(0);
                 hits.push_back(0);
                 
                 hits.push_back(0);
                 hits.push_back(0);
                 hits.push_back(0);
                 hits.push_back(0);
                 
                 
                 for (int j = 0; j < 8; j++) {
                     compareRhythm.push_back(2);
                 }
                 
                 
                 
             }
             
             
             if (duration == 1) {
                 rhythm.push_back(1);
                 
                 hits.push_back(1);
                 hits.push_back(0);
                 hits.push_back(0);
                 hits.push_back(0);
                 
                 for (int j = 0; j < 4; j++) {
                     compareRhythm.push_back(1);
                 }
                 
             }
             
             
             if (duration == 0.5) {
                 
                 rhythm.push_back(0.5);
                 
                 hits.push_back(1);
                 hits.push_back(0);
                 
                 for (int j = 0; j < 2; j++) {
                     compareRhythm.push_back(0.5);
                 }
                 
                 
             }
             
             if (duration == 0.25) {
                 
                 rhythm.push_back(0.25);
                 
                 hits.push_back(1);
                 compareRhythm.push_back(0.25);
                 
                 
             }
             
         }

//--------------------------------------------------------------

