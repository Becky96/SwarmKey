//
//  Particle.cpp
//  abletonDemo
//
//  Created by Becky Johnson on 30/01/2018.
//
//

#include "Particle.hpp"


void Particle::setupParticle() {
    
    
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

    
    for (int i = 0; i < 16; i++) {
        compareBestRhythm.push_back(0);
    }
    
    determineRhythm();
    
    for (int i = 0; i < rhythm.size(); i++) {
        bestRhythm.push_back(0);
    }
    
    
    velocity = int(ofRandom(40, 100));
}


//--------------------------------------------------------------
void Particle::determineRhythm() {
            
            
            dimensionality = 0;
            rhythm.clear();
            hits.clear();
            
            int r = int(ofRandom(0, 5));
            
            
            rhythm.push_back(validDurations[r]);
            cout << r << endl;
            cout << rhythm[0] << endl;
            
            float sum = rhythm[0];
            
            while (sum != 4) {
                
                //cout << "Current sum: " << sum << endl;
                
                if (sum >= 3.75) {
                    
                    r = 4;
                    rhythm.push_back(validDurations[r]);
                    // cout << "3.75 picked" << endl;
                    
                    
                } else if (sum > 3 && sum <= 3.5) {
                    
                    r = int(ofRandom(3, 5));
                    rhythm.push_back(validDurations[r]);
                    // cout << "3.5 picked" << endl;
                    
                    
                } else if (sum > 2 && sum <= 3) {
                    
                    r = int(ofRandom(2, 5));
                    rhythm.push_back(validDurations[r]);
                    // cout << "3 picked" << endl;
                    
                    
                    
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
