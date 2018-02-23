//
//  PSO.cpp
//  SwarmKey
//
//  Created by Becky Johnson on 24/01/2018.
//
//

#include "PSO.hpp"


void PSO::setup() {
    //Initialising population
    for (int i = 0; i < N; i++) {
        Particle *p = new Particle();
        particles.push_back(p);
        p->setup();
    }
    
    
    //Learning factors
    c1 = con * (4.1/2);
    c2 = c1;
    
    
    best = new Particle();
    best->setup();
    
  //  for (int i = 0; i < 16; i++) {
   // best->hits.push_back(0);
    //}
}

void PSO::run() {
    
    fitness();
    checkPersonalBest();
    calculateBest();
    updateParticles();
    
    
    
}

void PSO::fitness() {
    
    
    for (int i = 0; i < N; i++) {
        
        
        float sum = 0;
        
      //  if (particles[i]->dimensionality != chosenDimension) {
     //       sum+=10000000;
       // }
        
        
        sum+= abs(chosenDimension-particles[i]->dimensionality)*100.;
        /*
        for (int j = 0; j < particles[i]->rhythm.size(); j++) {
            
            if (particles[i]->rhythm[j] == 4) {
                //cout << "penalty 1 " << endl;
                sum+=semibrevePen;
                
            } else if (particles[i]->rhythm[j] == 2){
                //cout << "penalty 2 " << endl;

                sum+= minimPen;
                
            }else if (particles[i]->rhythm[j] == 1){
                //cout << "penalty 3 " << endl;

                sum+= crotchetPen;

            }else if (particles[i]->rhythm[j] == 0.5){
                //cout << "penalty 4 " << endl;

                sum+= quaverPen;

            }else if (particles[i]->rhythm[j] == 0.25){
                //cout << "penalty 5 " << endl;

                sum+= semiquaverPen;
                
            }
            
        }
        */
        particles[i]->fitness = sum;
        
        //cout << "Particle num: " << i << endl;
        //cout << "Fitness: " << particles[i]->fitness << endl;
    }
    
    
}

void PSO::checkPersonalBest() {
    
    for (int i = 0; i < N; i++) {
        
        if (particles[i]->fitness < particles[i]->bestFitness) {
            
            particles[i]->bestRhythm.clear();
            particles[i]->bestDimensionality = particles[i]->dimensionality;
            for (int j = 0; j < particles[i]->rhythm.size(); j++) {
                particles[i]->bestRhythm.push_back(particles[i]->rhythm[j]);
                
            }
            particles[i]->bestFitness = particles[i]->fitness;
            
        }
    }
    
    
}


void PSO::calculateBest() {
    
    for (int i = 0; i < particles.size(); i++) {
        
        if (particles[i]->fitness < bestFitness) {
            
            bestFitness = particles[i]->fitness;
            best = particles[i];
            best->bestDimensionality = particles[i]->dimensionality;
        }
    }

    
}


void PSO::updateParticles() {
    
    
    for (int i = 0; i < particles.size(); i++) {
        
        particles[i]->rhythm.clear();
        particles[i]->hits.clear();
        
        r1 = ofRandom(1);
        r2 = ofRandom(1);
        
        
    
        particles[i]->dimensionalityVel = (con * (particles[i]->dimensionalityVel + (c1*r1*(particles[i]->bestDimensionality-particles[i]->dimensionality) + (c2*r2*(best->bestDimensionality - particles[i]->dimensionality)))));
        
//            cout << " " << endl;
//        cout << " " << endl;
//        cout << "PARTICLE NUMBER: " << i << endl;
//        
//        cout << "vel: " << particles[i]->dimensionalityVel << endl;
//        
        
        //particles[i]->dimensionalityVel/=10.;
        
       // cout << "Dimension before: " << particles[i]->dimensionality << endl;

        
        particles[i]->dimensionality = particles[i]->dimensionality + particles[i]->dimensionalityVel;
        
        particles[i]->dimensionality = int(ofClamp(particles[i]->dimensionality, 1, 16));
        
        //cout << "Dimension after: " << particles[i]->dimensionality << endl;
        createSequence(particles[i]->dimensionality, particles[i]);
    }

    
}

void PSO::createSequence(int d, Particle * p) {
    p->rhythm.clear();
    p->hits.clear();
    validDur.clear();
   // cout << "dimension in function: " << d << endl;
    
    
    
    
    int r = int(ofRandom(0, 5));
    float newDur = validDurations[r];
    
    p->rhythm.push_back(newDur);
   // cout << "first: " << p->rhythm[0] << endl;
    
    float sum = p->rhythm[0];
    
    if (p->rhythm[0] == 4 &&  d == 1) {
        
        
    } else {
    
        if (d == 1) {
            validDur.push_back(4);
        }
        if (d == 2) {
            validDur.push_back(2);
        } else if (d == 3) {
            validDur.push_back(2);
            validDur.push_back(1);
        } else if (d == 4) {
            validDur.push_back(1);
        } else if (d == 5) {
            validDur.push_back(0.25);
            validDur.push_back(0.5);
            validDur.push_back(1);
            validDur.push_back(2);
        } else if (d == 6) {
            validDur.push_back(0.5);
            validDur.push_back(1);
            validDur.push_back(2);
        } else if (d == 7) {
            validDur.push_back(0.25);
            validDur.push_back(0.5);
            validDur.push_back(1);
            
        } else if (d == 8) {
            validDur.push_back(0.5);
            
        } else if (d >= 9 && d <= 15) {
            validDur.push_back(0.25);
            validDur.push_back(0.5);
            
        } else if (d == 16) {
            validDur.push_back(0.25);
            
        }
    
        while (sum != 4 || p->rhythm.size() != d) {
            
       
            r = int(ofRandom(0, validDur.size()));
            //cout << "index: " << r << endl;
            newDur = validDur[r];
            
            sum = 0;
            for (int i = 0; i < p->rhythm.size(); i++) {
                sum+=p->rhythm[i];
            }
            
            
            if (p->rhythm.size() > d) {
                p->rhythm.clear();
                sum = 0;
            }
            
            if (sum + newDur <= 4) {
                p->rhythm.push_back(newDur);
                sum+=newDur;
            } else {
                p->rhythm.clear();
                sum = 0;
            }
            
            
            
            if (p->rhythm.size() > d || sum > 4) {
                p->rhythm.clear();
                sum = 0;
            }
            
            if (p->rhythm.size() < d && sum == 4) {
                p->rhythm.clear();
                sum = 0;
            }
            
        }
        
    }
    
    
    
    
    
    for (int i = 0; i < p->rhythm.size(); i++) {
        
        if (p->rhythm[i] == 4) {
            p->hits.push_back(1);
            for (int j = 0; j < 15; j++) {
                p->hits.push_back(0);
            }
            
        } else if (p->rhythm[i] == 2) {
            p->hits.push_back(1);
            for (int j = 0; j < 7; j++) {
                p->hits.push_back(0);
            }

            
        } else if (p->rhythm[i] == 1) {
            p->hits.push_back(1);
            for (int j = 0; j < 3; j++) {
                p->hits.push_back(0);
            }

            
        } else if (p->rhythm[i] == 0.5) {
            p->hits.push_back(1);
            p->hits.push_back(0);
            
            
        }
        else if (p->rhythm[i] == 0.25) {
            p->hits.push_back(1);


            
        }
    }
    
    
    /*

    for (int i = 0; i < p->rhythm.size(); i++) {
        cout << p->rhythm[i] << ", ";
    }
    cout << endl;
    for (int i = 0; i < 16; i++) {
        cout << p->hits[i];
    }
    
    cout << "Rhythm created" << endl;*/
    
}


void PSO::display() {
    
    ofSetColor(255);
    for (int i = 1; i <= 16; i++) {
        ofDrawRectangle(100+(i*50), 100, 20, 20);
    }
    
    
    ofSetColor(255, 0, 100, 100);
    for (int i = 0; i < particles.size(); i++) {
        ofDrawRectangle(100+(particles[i]->dimensionality*50), 100, 10, 10);
    }
}

int PSO::createNewDur(int sInd, int eInd) {
    
    int r = int(ofRandom(sInd, eInd));
    return validDurations[r];
    
}

void PSO::resetParticles() {
    
 
}
void PSO::disturb() {
    
    
}

void PSO::determineRhythm() {
    
    
}