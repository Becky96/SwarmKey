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
        //Default note and velocity

    stressedVelocity = 100;
    notStressedVelocity = 80;
    
    //Initialising population
    for (int i = 0; i < N; i++) {
        Particle *p = new Particle();
        particles.push_back(p);
        particles[i]->setupParticle();

    }
    
    //bestRhythm = *new Particle();
    bestRhythm.setupParticle();
    
    //Learning factors
    noteC1 = noteCon * (4.1/2);
    noteC2 = noteC1;
    
    rhythmC1 = rhythmCon * (4.1/2.);
    rhythmC2 = rhythmC1;
    
    chordC1 = chordCon * (4.1/2);
    chordC2 = chordC1;
    
    velocityC1 = velocityCon * (4.1/2.);
    velocityC2 = velocityC1;
    
    calculateKey(67);
    
    //Initialising prevBestIndFreqs
    prevBestIndFreqs[0] = 100;
    prevBestIndFreqs[1] = 100;
    prevBestIndFreqs[2] = 100;
    prevBestIndFreqs[3] = 100;
    

    
}
//--------------------------------------------------------------
void Swarm::inputMotif(int nMotif[4], int rMotif[16]) {
    
    //Assigning note motif as swarm note motif
    for (int i = 0; i < 4; i++) {
        noteMotif[i] = nMotif[i];
        originalMotif[i] = noteMotif[i];
    }
    
    //Assigning rhythm motif as swarm rhythm motif
    //Setting dimensionality to number of 1s counted in rhythm motid
    for (int i = 0; i < 16; i++) {
        rhythmMotif[i] = rMotif[i];
        if (rhythmMotif[i] == 1) {
            dimensionalityMotif++;
        }
    }
    
    targetDimensionality = dimensionalityMotif;

}


//--------------------------------------------------------------
//Determine viable keys to play
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

}
//--------------------------------------------------------------
//Algorithm functions to determine pitch sequence
void Swarm::run(Swarm * alternateSwarm, int rhythmPlayhead, int notePlayhead, int alternateNotePlayhead) {
   
    
    fitness();
    checkPersonalBest();
    checkSwarmBest();
    
   if (channel == 1) {

    harmonicIntervalFitness(alternateSwarm, rhythmPlayhead, notePlayhead, alternateNotePlayhead);
    fitness();
    checkPersonalBest();
    checkSwarmBest();
    }
    
    //disturb();
    updateParticles();
    
    if (desiredNoteDistance != 0) {
        checkRepeat();
    }
    
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->fitness = 0;
    }
    
    

}


void Swarm::harmonicIntervalFitness(Swarm *alternateSwarm, int rhythmPlayhead, int notePlayhead, int alternateNotePlayhead) {
    
    
    
    int tempNotePlayhead;
    int tempAlternateNotePlayhead;
    
    //Check rhythm is current
    /*
    cout << "NEW: " << endl;
    for (int i = 0; i < 16; i++) {
        cout << alternateSwarm->bestRhythm->hits[i] << ", ";
    }*/
    
    
    //Loop through all particles to compare harmonic intervals between itself and the best particle of the alternate swarm.
    for (int i = 0; i < particles.size(); i++) {
        
        particles[i]->fitness = 0;
        
        float harmonicIntervalSum = 0;
        tempNotePlayhead = notePlayhead;
        tempAlternateNotePlayhead = alternateNotePlayhead;
        
        for (int j = 0; j < 16; j++) {
        
            //If particle has a hit at the same index of the best particle of the alternate swarm, calculate the harmonic interval.
            if (particles[i]->hits[j] == 1) {
            
                if (alternateSwarm->bestRhythm.hits[j] == 1) {
                
                    //cout << "CALCULATE INERVAL" << endl;
                    //Calculate absolute difference
                    int interval = abs(alternateSwarm->best.indFreqs[tempAlternateNotePlayhead]-particles[i]->indFreqs[tempNotePlayhead]);
                    
                    //interval of 1 - 0
                    //2 - 1
                    //3 - 2
                    //4 - 3
                    //5 - 4
                    //6 - 5
                    //7 - 6
                    //8 - 7
                    //Add penalty to intervals of 1, 2, 6, and 7 between swarms.
                    if (interval % 7 == 1 || interval % 7 == 3 || interval % 7 == 6 ) {
                        harmonicIntervalSum += 1000;
                    } else if (interval % 7 == 0 || interval % 7 == 2 || interval % 7 == 4) {
                        harmonicIntervalSum -= 0;
                    }
                    
                    if (interval > 13) {
                        harmonicIntervalSum+=10000;
                    }
                    //Increment the note playheads to calculate other harmonic intervals within the particle sequences.
                    tempAlternateNotePlayhead++;
                    tempNotePlayhead++;
                    
                    
                    
                }
            }
            
        }
        
        particles[i]->fitness+=harmonicIntervalSum;
        
    }
    
    bestFitness = 9999999;
    
    
}

int Swarm::determineParticleOctave(int index) {
    
        int octave;
    
        if (index >= 0 && index <= 6) {
                octave = 1;
        } else if (index >= 7 && index <= 13) {
                    octave = 2;
            
        } else if (index >= 14 && index <= 20) {
            
             octave = 3;
            
        } else if (index >= 21 && index <= 27) {
            
             octave = 4;
            
        } else if (index >= 28 && index <= 34) {
            
             octave = 5;
            
        } else if (index >= 35 && index <= 41) {
            
             octave = 6;
            
        } else if (index >= 42 && index <= 48) {
            
            
             octave = 7;
            
        } else if (index >= 49 && index<= 55) {
            
             octave = 8;
            
        }
    

return octave;
    
}
//--------------------------------------------------------------
//Fitness function for pitch sequence
void Swarm::fitness() {
    
    
    for (int i = 0; i < particles.size(); i++) {
        
        
        float fitnessSum = 0;
        //cout << " " << endl;
        //cout << "Particle number: " << i << endl;
        
        //Calculating motif fitness
        double noteDistance = 0;
        for (int j = 0; j < 4; j++) {
            
          //  cout << "Particle freq: " << particles[i]->indFreqs[j] << endl;
            noteDistance += (noteMotif[j] - particles[i]->indFreqs[j]) * (noteMotif[j] - particles[i]->indFreqs[j]);
           // cout << "Current note distance: " << noteDistance << endl;
        }
       // cout << "Overall note distance: " << noteDistance;
        fitnessSum = abs(desiredNoteDistance - noteDistance);
        
        
        
        //Calculating interval fitness
        
        if (desiredNoteDistance != 0) {
        int intervals[3];
        //Determine whether the bar is ascending or descending in pitch.
        //sign = -1 if descending
        //sign = 0 if there is no change between note 1 and note 2
        //sign = 1 if ascending
      
        //Determine distance of notes 2, 3, and 4 from note 1.
        for (int j = 0; j < 3; j++) {

            
            //Distance between current note and note 1
            int dist = (particles[i]->indFreqs[j+1] - particles[i]->indFreqs[j]);
            int sign = ofSign(particles[i]->indFreqs[j+1] - particles[i]->indFreqs[j]);
            
            //cout << "distance and sign: " << dist << " and " << sign << endl;

            
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
            
   
        }
        
        }
        
        
        //Calculating fitness from desired octave
        /*for (int j = 0; j < 4; j++) {
            
            int octave = determineParticleOctave(particles[i]->indFreqs[j]);
            fitnessSum += (abs(chosenOctave-octave)*abs(chosenOctave-octave)) * 10000.;
            
        }*/
        
    
        particles[i]->fitness = fitnessSum;
        
        
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
            particles[i]->indFreqsVel[j] = noteCon * (particles[i]->indFreqsVel[j] + (noteC1*r1*(particles[i]->bestIndFreqs[j]-particles[i]->indFreqs[j])+ noteC2*r2*(best.bestIndFreqs[j]-particles[i]->indFreqs[j])));
            
        
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
////////////RHYTHM////////////
void Swarm::runRhythm() {
    fitnessRhythm();
    checkPersonalBestRhythm();
    calculateBestRhythm();
    updateParticlesRhythm();
}

//--------------------------------------------------------------

void Swarm::fitnessRhythm() {
    
    for (int i = 0; i < N; i++) {
        
        float fitnessSum = 0;
      

        double rhythmDistance = 0;

        cout << targetDimensionality << endl;
        cout << "particle's dimension: " << particles[i]->dimensionality << endl;
        rhythmDistance = abs(targetDimensionality-particles[i]->dimensionality);
        cout << "rhythm fitness: " << rhythmDistance << endl;
        fitnessSum = (rhythmDistance);
   

      
        particles[i]->fitnessRhythm = fitnessSum;
        
    }
}

void Swarm::checkPersonalBestRhythm() {
    
    for (int i = 0; i < N; i++) {
        
        if (particles[i]->fitnessRhythm < particles[i]->bestFitnessRhythm) {
            
            particles[i]->bestRhythm.clear();
            particles[i]->bestDimensionality = particles[i]->dimensionality;
            
            for (int j = 0; j < particles[i]->rhythm.size(); j++) {
                particles[i]->bestRhythm.push_back(particles[i]->rhythm[j]);
            }
            
            particles[i]->bestFitnessRhythm = particles[i]->fitnessRhythm;
            
        }
    }
    
}

//--------------------------------------------------------------

void Swarm::calculateBestRhythm() {
    
    for (int i = 0; i < particles.size(); i++) {
        
        
        if (particles[i]->fitnessRhythm < bestFitnessRhythm) {
            
        
        
            bestFitnessRhythm = particles[i]->fitnessRhythm;
            
            
            bestRhythm = *particles[i];
            bestRhythm.dimensionality = particles[i]->dimensionality;
            bestRhythm.hits = particles[i]->hits;
            bestRhythm.bestRhythm = particles[i]->rhythm;
            bestRhythm.bestDimensionality = particles[i]->dimensionality;
            
           // cout << "New best rhythm value" << endl;
            //for (int j = 0; j < 16; j++) {
              //  cout << bestRhythm.hits[j] << ", ";
           // }
        }
        
    }
    
    
}

//--------------------------------------------------------------

void Swarm::updateParticlesRhythm() {
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->rhythm.clear();
        particles[i]->hits.clear();
        
        r1 = ofRandom(1);
        r2 = ofRandom(1);
        
        particles[i]->dimensionalityVel = (rhythmCon * (particles[i]->dimensionalityVel + (rhythmC1*r1*(particles[i]->bestDimensionality-particles[i]->dimensionality) + (rhythmC2*r2*(bestRhythm.bestDimensionality - particles[i]->dimensionality)))))*5.;
        
        particles[i]->dimensionality = (particles[i]->dimensionality + particles[i]->dimensionalityVel);
        
        //if (targetDimensionality <= dimensionalityMotif) {
        //particles[i]->dimensionality = int(ofClamp(particles[i]->dimensionality, 1, dimensionalityMotif));
        //} else if (targetDimensionality >= dimensionalityMotif) {
            particles[i]->dimensionality = int(ofClamp(particles[i]->dimensionality, 1, 16));

       // }
        
        createSequenceRhythm(particles[i]->dimensionality, particles[i]);
    }
    
    
}

//--------------------------------------------------------------

void Swarm::createSequenceRhythm(int d, Particle * p) {
   
    //Clear all previous rhythms, hits, and available note durations
    p->rhythm.clear();
    p->hits.clear();
    validDur.clear();
    
    
    //Generate new note duration from global valid duration list.
    int r = int(ofRandom(0, 5));
    float newDur = validDurations[r];
    
    //Add new note duration to particle's rhythm sequence
    p->rhythm.push_back(newDur);
    
    //Create sum total of particle's rhythm sequence.
    float sum = p->rhythm[0];
    
    //Determine if added note is 4 and if dimensionality is 1
    if (p->rhythm[0] == 4 &&  d == 1) {
        
        
    //If not
    } else {
        
        //If dimensionality is 1, only allow for 4
        if (d == 1) {
            validDur.push_back(4);
        }
        
        //If dimensionality is 2, only allow for 2
        if (d == 2) {
            validDur.push_back(2);
            
        //If dimensionality is 3, only allow for 2, 1
        } else if (d == 3) {
            validDur.push_back(2);
            validDur.push_back(1);
            
        //If dimensionality is 4, only allow for 1
        } else if (d == 4) {
            validDur.push_back(1);
            
        //If dimensionality is 5, only allow for 0.25, 0.5, 1, 2
        } else if (d == 5) {
            validDur.push_back(0.25);
            validDur.push_back(0.5);
            validDur.push_back(1);
            validDur.push_back(2);
        
        //If dimensionality is 6, only allow for 0.5, 1, 2
        } else if (d == 6) {
            validDur.push_back(0.5);
            validDur.push_back(1);
            validDur.push_back(2);
            
        //7 dimensionality - 0.25, 0.5, 1
        } else if (d == 7) {
            validDur.push_back(0.25);
            validDur.push_back(0.5);
            validDur.push_back(1);
            
        //8 dimensionality - 0.5
        } else if (d == 8) {
            validDur.push_back(0.5);
            
        
        } else if (d >= 9 && d <= 15) {
            validDur.push_back(0.25);
            validDur.push_back(0.5);
            
        } else if (d == 16) {
            validDur.push_back(0.25);
            
        }
        
        //While total sum is not equal to 4 and the size of particle's rhythm sequence is not equal to it's dimensionality
        while (sum != 4 || p->rhythm.size() != d) {
            
            //Create new note duration from particle's specific validDurations.
            r = int(ofRandom(0, validDur.size()));
            newDur = validDur[r];
            
            //Reset sum
            sum = 0;
            
            //Create total of particle's current rhythm sequence
            for (int i = 0; i < p->rhythm.size(); i++) {
                sum+=p->rhythm[i];
            }
            
            //If particle's rhythm sequence is larger than it's dimensionality clear the rhythm and current sequence
            if (p->rhythm.size() > d) {
                p->rhythm.clear();
                sum = 0;
            }
            
            //If the total sum with the newly generated note duration does not exceed 4, add the new note to the particle's rhythm sequence.
            if (sum + newDur <= 4) {
                p->rhythm.push_back(newDur);
                sum+=newDur;
            
                //If adding the new note duration exceeds 4, clear the rhythm sequence and set sum to 0
            } else {
                p->rhythm.clear();
                sum = 0;
            }
            
            //If rhythm sequence size exceeds dimensionality or sum exceeds 4, clear the rhythm sequence.
            if (p->rhythm.size() > d || sum > 4) {
                p->rhythm.clear();
                sum = 0;
            }
            
            //If rhythm sequence is equal to 4 but does not have the same size as the dimensionality, clear the
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
    
}

//--------------------------------------------------------------
//Velocity functionality
//--------------------------------------------------------------

void Swarm::runVelocity() {
    
    fitnessVelocity();
    checkPersonalBestVelocity();
    calculateBestVelocity();
    updateParticleVelocity();
    
}

//--------------------------------------------------------------

void Swarm::fitnessVelocity() {
    
    
    for (int i = 0; i < particles.size(); i++) {
    
        particles[i]->velocityFitness = (desiredVelocity-particles[i]->velocity) * (desiredVelocity-particles[i]->velocity);
        
    }
    
}
//--------------------------------------------------------------

void Swarm::checkPersonalBestVelocity() {
    
    for (int i = 0; i < particles.size(); i++) {
        
        if (particles[i]->velocityFitness < particles[i]->bestParticleVelocityFitness) {
            
            particles[i]->bestParticleVelocity = particles[i]->velocity;
            particles[i]->bestParticleVelocityFitness = particles[i]->velocityFitness;
        }
        
    }
    
}

//--------------------------------------------------------------

void Swarm::calculateBestVelocity() {
    
    for (int i = 0; i < particles.size(); i++) {
        
        if (particles[i]->velocityFitness < bestVelocityFitness) {
            
            bestParticleSwarmVelocity = particles[i]->velocity;
            bestVelocityFitness = particles[i]->velocityFitness;
            
        }
    }
    
}

//--------------------------------------------------------------

void Swarm::updateParticleVelocity() {
    
    for (int i = 0; i < particles.size(); i++) {
        r1 = ofRandom(1);
        r2 = ofRandom(2);
        
        particles[i]->velocityVel = velocityCon * (particles[i]->velocityVel + (velocityC1*r1*(particles[i]->bestParticleVelocity - particles[i]->velocity) + (velocityC2*r2*(bestParticleSwarmVelocity - particles[i]->velocity))));
        
        particles[i]->velocity = ofClamp(int(particles[i]->velocity + particles[i]->velocityVel), 1, 125);
    }
}

//--------------------------------------------------------------
//------Chord functionality------------------------
//--------------------------------------------------------------

void Swarm::runChord(int notePlayhead) {
    
    currentNotePlayhead = notePlayhead;
    
    fitnessChord();
    checkPersonalBestChord();
    checkBestChord();
    updateParticleChord();
}

//--------------------------------------------------------------

void Swarm::randomiseParticleChord(int p) {

    for (int i = 0; i < particles.size(); i++) {
        for (int j = chordPotential; j < p; j++) {
         
            particles[i]->chord[j] = int(ofRandom(0, availableNotes.size()-1));
            particles[i]->chordVel[j] = ofRandom(-2, 2);
                                        
        }
    }
    
    
}
//--------------------------------------------------------------

void Swarm::removeParticleChordIndex() {
    
    
}

//--------------------------------------------------------------

void Swarm::fitnessChord() {
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->chordFitness = 0;
        for (int j = 0; j < chordPotential; j++) {
            
            int interval = abs(best.indFreqs[currentNotePlayhead%4] - particles[i]->chord[j]);

            if (interval % 8 == 0 || interval % 8 == 7 || interval > 8) {
                particles[i]->chordFitness+=1000;
            } else {
                particles[i]->chordFitness -= 100;
            }
            //cout << "fitness: " << particles[i]->chordFitness << endl;
            
        }
    }
    
}

//--------------------------------------------------------------

void Swarm::checkPersonalBestChord() {
    
    for (int i = 0; i < particles.size(); i++) {
     
        if (particles[i]->chordFitness < particles[i]->chordBestFitness) {
            particles[i]->chordBestFitness = particles[i]->chordFitness;
            particles[i]->bestChord[0] = particles[i]->chord[0];
             particles[i]->bestChord[1] = particles[i]->chord[1];
             particles[i]->bestChord[2] = particles[i]->chord[2];
        }
    }
}
//--------------------------------------------------------------

void Swarm::checkBestChord() {
    
    for (int i = 0; i < particles.size(); i++) {

        if (particles[i]->chordFitness < chordBestFitness) {
            chordBestFitness = particles[i]->fitness;
            for (int j = 0; j < chordPotential; j++) {
                bestChord[j] = particles[i]->chord[j];
            }
            
        }
    }
    
}
//--------------------------------------------------------------

void Swarm::updateParticleChord() {
    
    for (int i = 0; i < particles.size(); i++) {
        r1 = ofRandom(1);
        r2 = ofRandom(2);
        for (int j = 0; j < chordPotential; j++) {
            particles[i]->chordVel[j] = chordCon * (particles[i]->chordVel[j] + (chordC1 * r1 * (particles[i]->bestChord[j] - particles[i]->chord[j]) + chordC2 * r2 * (bestChord[j]-particles[i]->chord[j])));
            
            //Chord update
            particles[i]->chord[j] = ofClamp(int(particles[i]->chord[j]+particles[i]->chordVel[j]), 0, availableNotes.size()-1);
        }
    }
    
}

