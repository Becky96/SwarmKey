//
//  Swarm.cpp
//  abletonDemo
//
//  Created by Becky Johnson on 29/01/2018.
//
//

#include "Swarm.hpp"

//ofxMidi function to open port.
void Swarm::openVirtualPort(string portName) {
    
    
    //Listing available ports.
    midiOut.listPorts();
    ofxMidiOut::listPorts();
    
    
    //Opening port to Ableton with 'IAC Driver Swarm1'
    midiOut.openVirtualPort(portName);
    
    
    
    
    
}
//--------------------------------------------------------------

//Setup swarm with channel number, initial note number to send, default velocity of 120.
void Swarm::setup(int _channel) {
    
    channel = _channel;
    
    note = 1;
    velocity = 120;
    
    
    
    //Initialising population
    for (int i = 0; i < N; i++) {
        Particle *p = new Particle();
        particles.push_back(p);
    }
    
    //Learning factors
    c1 = con * (4.1/2);
    c2 = c1;
    
    
    calculateKey(62);
    
    //Initialising prevBestIndFreqs
    prevBestIndFreqs[0] = 100;
    prevBestIndFreqs[1] = 100;
    prevBestIndFreqs[2] = 100;
    prevBestIndFreqs[3] = 100;
    


    
}

void Swarm::calculateKey(int start) {
    
    availableNotes.clear();
    
    
    for (int i = -4; i < 4; i++) {
    availableNotes.push_back((start+(i*12)));
    availableNotes.push_back((start+(i*12))+2);
    availableNotes.push_back((start+(i*12))+4);
    availableNotes.push_back((start+(i*12))+5);
    availableNotes.push_back((start+(i*12))+7);
    availableNotes.push_back((start+(i*12))+9);
    availableNotes.push_back((start+(i*12))+11);
    
    }

    
    for (int i = 0; i < availableNotes.size(); i++) {
        cout << availableNotes[i] << endl;
    }
    
    
}
//--------------------------------------------------------------

void Swarm::run() {
    
    fitness();
    checkPersonalBest();
    checkSwarmBest();
    disturb();
    updateParticles();
    checkRepeat();

}
//--------------------------------------------------------------

void Swarm::fitness() {
    
    
    
    for (int i = 0; i < particles.size(); i++) {
        
        float fitnessSum = 0;
        

        int intervals[3];
        
        cout << i << endl;
        
        //Determine whether the bar is ascending or descending in pitch.
        //sign = -1 if descending
        //sign = 0 if there is no change between note 1 and note 2
        //sign = 1 if ascending
      
        //Determine distance of notes 2, 3, and 4 from note 1.
        for (int j = 0; j < 3; j++) {
            /*
            if (particles[i]->indFreqs[j] > 40) {
                fitnessSum+=10000;
            }
            
            if (particles[i]->indFreqs[j] < 20) {
                fitnessSum+=10000;
            }
            */
            
            //Distance between current note and note 1
            int dist = (particles[i]->indFreqs[j+1] - particles[i]->indFreqs[j]);
            int sign = ofSign(particles[i]->indFreqs[j+1] - particles[i]->indFreqs[j]);
            
            cout << "distance and sign: " << dist << " and " << sign << endl;

            
            if (dist == 0) {    //Same note = perfect unison - slight weight for this note
                
                fitnessSum += firstPen;
                intervals[j] = 0;
            }
            else if (dist == sign*1 || dist == (-sign)*1) {    //Second - heavy weight, do not want occurances of this.
                
                fitnessSum += secondPen;
                intervals[j] = 1;
            }
            
            
            else if (dist == sign*2 || dist == (-sign)*2) {    //Third - no weight, do want occurrences of this interval.
                
                fitnessSum += thirdPen;
                intervals[j] = 2;
            }
            else if (dist == sign*3 || dist == (-sign)*3) {    //Fourth - slightly penalty, want slight occurences of this
                
                fitnessSum += fourthPen;
                intervals[j] = 3;
            }
            else if (dist == sign*4 || dist == (-sign)*4) {    //Fifth - no weight
                
                fitnessSum += fifthPen;
                intervals[j] = 4;
            }
            else if (dist == sign*5 || dist == (-sign)*5) {    //Sixth - slight weight
                
                fitnessSum += sixthPen;
                intervals[j] = 5;
            }
            else if (dist == sign*6 || dist == (-sign)*6) {    //Seventh - heavy weight, do not want this
                
                fitnessSum += seventhPen;
                intervals[j] = 6;
            }
            else if (dist == sign*7 || dist == (-sign)*7) {    //Octave - slight weight, would like occurrences
                
                fitnessSum += eighthPen;
                intervals[j] = 7;
                
            } else {
                
                fitnessSum+=elsePen;  //Moderaly heavy weight for any notes that escape the boundary of a distance of an octave. Occurrences of this are okay but in very slight moderation.
            }
            
            cout << fitnessSum << endl;
            
            /*
            //Checking intervals between consecutive notes to maintain a linear progression of  either ascending or descending.
            //If there is no difference between the 1st and 2nd note, add a penalty.
            if (sign == 0) {
                
                fitnessSum+=2000;
            }
            
            //If notes are ascending
            else if (sign == 1) {
                
                
                if (particles[i]->indFreqs[j+1] > particles[i]->indFreqs[j]) {
                    fitnessSum-=100;
                    
                } else if (particles[i]->indFreqs[j+1] <= particles[i]->indFreqs[j]) {
                    fitnessSum+=10000;
                }
                
            } else if (sign == -1) {
                
                if (particles[i]->indFreqs[j+1] < particles[i]->indFreqs[j]) {
                    fitnessSum-=100;
                    
                } else if (particles[i]->indFreqs[j+1] >= particles[i]->indFreqs[j]) {
                    fitnessSum+=10000;
                }
                
            }
            
            if (abs(particles[i]->indFreqs[j+1])- particles[i]->indFreqs[j] == 0) {
                fitnessSum+=10000;
            }
        
             */
        }
        
        particles[i]->fitness = fitnessSum;
        cout << "Fitness of " << i << ": " << particles[i]->fitness << endl;
        
    }
    
    
}
//--------------------------------------------------------------

void Swarm::checkPersonalBest() {
    
    for (int i = 0; i < N; i++) {
        
        
        if (particles[i]->fitness < particles[i]->bestFit) {
            
            particles[i]->bestFit = particles[i]->fitness;
            
            for (int j = 0; j < 4; j++) {
                
                particles[i]->bestIndFreqs[j] = particles[i]->indFreqs[j];
                
            }
        }
        
    }

}

//--------------------------------------------------------------

void Swarm::checkSwarmBest() {
    

    for (int i = 0; i < particles.size(); i++) {
        
        if (particles[i]->fitness < bestFitness) {
            
            bestFitness = particles[i]->fitness;
            best = *particles[i];
            bestIndex = i;
            
        }
    }

    
}
//--------------------------------------------------------------

void Swarm::updateParticles() {
    
    for (int i = 0; i < particles.size(); i++) {
        
        
        r1 = ofRandom(1);
        r2 = ofRandom(1);
        
        for (int j = 0; j < 4; j++) {
            
            //Velocity update
            particles[i]->indFreqsVel[j] = con * (particles[i]->indFreqsVel[j] + (c1*r1*(particles[i]->bestIndFreqs[j]-particles[i]->indFreqs[j])+ c2*r2*(best.bestIndFreqs[j]-particles[i]->indFreqs[j])));
            
        
            //Position update
            particles[i]->indFreqs[j] = ofClamp(int(((particles[i]->indFreqs[j]+particles[i]->indFreqsVel[j]))), 0, availableNotes.size()-1);

        }
        
    }

}

//--------------------------------------------------------------
//This will aid the swarm in becoming "unstuck" in a sequence that is a local optima as defined by the fitness function. This also allows for variation by taking the repeated sequence and changing it slightly, similar to the mutation operation of a genetic algorithm.
void Swarm::checkRepeat() {
    
    int indexCheck = 0;
    
    //Check if previous frequency indexes matches the current frequency indexes of the best particle.
    //If yes, increment for each
    for (int i = 0; i < 4; i++) {
        
        if (prevBestIndFreqs[i] == best.indFreqs[i]) {
            indexCheck++;
            
        }
        
    }
    
    
    //if all indexes matched, increment 'repeated' by 1. Allow 2 repetitions before forcing best particle to randomise or vary
    if (indexCheck == 4) {
        repeated++;
        
    }
    
    //If sequence has been repeated 4 times, loop through all particles and randomly reset some of them based upon random variable r. Also using a random variable, alter some of the values of the best particle's indFreq array to hopefully create some variation of the sequence.
    if (repeated == 2) {
        
        //cout << "REPEATED " << endl;
        
        //Reset particles so they may discover other possible viable solutions
        for (int i= 0; i < N; i++) {
            
            float r = ofRandom(1);
            
            if (r > 0.75) {
                for (int j = 0; j < 4; j++) {
                    if (particles[i]->indFreqs[j] >= 2 && particles[i]->indFreqs[j] <= availableNotes.size()-1) {
                    particles[i]->indFreqs[j] = particles[i]->indFreqs[j]+int(ofRandom(-1, 1));
                    }
                }
            }
        }
        
        
        
        for (int i = 0; i < 4; i++) {
            float r = ofRandom(1);
            
            if (r > 0.25) {
                best.indFreqs[i] =best.indFreqs[i]+int(ofRandom(-2, 2));
                prevBestIndFreqs[i] = 100;
            }
        }
        
        //bestFitness = 20000000;
        repeated = 0;
        
        
    } else {
        
        for (int i = 0; i < 4; i++) {
            
            prevBestIndFreqs[i] = best.indFreqs[i];
            
        }
    }
    
    
}
//--------------------------------------------------------------
//Similar to the DFO disturbance threshold, this will also aid the particle system in exploring the search area. If a randomly generated variable r exceeds a certain variable, the fly will reset it's variables/features.
void Swarm::disturb() {
    
    for (int i = 0; i < N; i++) {
        float r = ofRandom(1);
        if (r < dt) {
            
            for (int j = 0; j < 4; j++) {

                particles[i]->indFreqs[j] = int(ofRandom(0, availableNotes.size()));
                particles[i]->indFreqsVel[j] = ofRandom(-2, 2);
                
            }
            
        }
        
    }
    
}

//--------------------------------------------------------------
void Swarm::display() {
    
    
    //1st index
    ofFill();
    ofSetColor(0);
    for (int i = 0; i < availableNotes.size(); i++) {
        ofDrawRectangle(10+ (i*20), 50, 10, 10);
    }
    
    ofSetColor(200, 100, 20, 80);
    for (int i = 0; i < particles.size(); i++) {
        ofSetColor(particles[i]->col);

        ofDrawEllipse(10+(particles[i]->indFreqs[0]*20), 50, 10, 10);
    }
    
    ofSetColor(255, 0, 0);
    ofDrawEllipse(10+(best.indFreqs[0]*20), 50, 20, 20);
    
    //2nd index
    ofFill();
    ofSetColor(0);
    for (int i = 0; i < availableNotes.size(); i++) {
        ofDrawRectangle(10+ (i*20), 75, 10, 10);
    }
    
    for (int i = 0; i < particles.size(); i++) {
        ofSetColor(particles[i]->col);

        ofDrawEllipse(10+(particles[i]->indFreqs[1]*20), 75, 10, 10);
    }
    
    ofSetColor(255, 0, 0);
    ofDrawEllipse(10+(best.indFreqs[1]*20), 75, 20, 20);
    
    //3rd index
    ofFill();
    ofSetColor(0);
    for (int i = 0; i < availableNotes.size(); i++) {
        ofDrawRectangle(10+ (i*20), 100, 10, 10);
    }
    
    ofSetColor(200, 100, 20, 80);
    for (int i = 0; i < particles.size(); i++) {
        ofSetColor(particles[i]->col);

        ofDrawEllipse(10+(particles[i]->indFreqs[2]*20), 100, 10, 10);
    }
    
    ofSetColor(255, 0, 0);
    ofDrawEllipse(10+(best.indFreqs[2]*20), 100, 20, 20);
    
    //4th index
    ofFill();
    ofSetColor(0);
    for (int i = 0; i < availableNotes.size(); i++) {
        ofDrawRectangle(10+ (i*20), 125, 10, 10);
    }
    
    ofSetColor(200, 100, 20, 80);
    for (int i = 0; i < particles.size(); i++) {
        ofSetColor(particles[i]->col);

        ofDrawEllipse(10+(particles[i]->indFreqs[3]*20), 125, 10, 10);
    }
    
    ofSetColor(255, 0, 0);
    ofDrawEllipse(10+(best.indFreqs[3]*20), 125, 20, 20);
    
}

//--------------------------------------------------------------

void Swarm::exit() {
    
    midiOut.closePort();
}


//--------------------------------------------------------------


void Swarm::play() {
    cout << channel << endl;
    if (channel == 1) {
        note = 60;
        midiOut.sendNoteOn(channel, note, velocity);
        
        note = 64;
        midiOut.sendNoteOn(channel, note, velocity);

        
        note = 67;
        midiOut.sendNoteOn(channel, note, velocity);

    }
    
    if (channel == 2) {
        note = 62;
        midiOut.sendNoteOn(channel, note, velocity);
        
        note = 65;
        midiOut.sendNoteOn(channel, note, velocity);
        
        note = 69;
        midiOut.sendNoteOn(channel, note, velocity);

    }
    
}