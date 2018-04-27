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
    //midiOut.openPort(0);
    
    
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
    

    velocityC1 = velocityCon * (4.1/2.);
    velocityC2 = velocityC1;
    
    calculateKey(tonic, 1);
    
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

    
    //Calculating octaves of phrases
    noteMotifOctaves[0] = (floor(noteMotif[0])/7)+1;
    noteMotifOctaves[1] = (floor(noteMotif[1])/7)+1;
    noteMotifOctaves[2] = (floor(noteMotif[2])/7)+1;
    noteMotifOctaves[3] = (floor(noteMotif[3])/7)+1;
    
    cout << "Octave of note: " << noteMotif[0] << "is: " << noteMotifOctaves[0] << endl;
    cout << "Octave of note: " << noteMotif[1] << "is: " << noteMotifOctaves[1] << endl;
    cout << "Octave of note: " << noteMotif[2] << "is: " << noteMotifOctaves[2] << endl;
    cout << "Octave of note: " << noteMotif[3] << "is: " << noteMotifOctaves[3] << endl;
    
    distMotifOctave = chosenOctave - noteMotifOctaves[0];
    cout << "octave distance: " << distMotifOctave << endl;
}


//--------------------------------------------------------------
//Determine viable keys to play in major scale
void Swarm::calculateKey(int start, int type) {
    
    tonic = start;
    
    availableNotes.clear();
    
    //If major key
    if (type == 1) {
        for (int i = -3; i < 3; i++) {
            availableNotes.push_back((start+(i*12)));
            availableNotes.push_back((start+(i*12))+2);
            availableNotes.push_back((start+(i*12))+4);
            availableNotes.push_back((start+(i*12))+5);
            availableNotes.push_back((start+(i*12))+7);
            availableNotes.push_back((start+(i*12))+9);
            availableNotes.push_back((start+(i*12))+11);
    
    }
    }
    
    //If minor key
    if (type == 2) {
        for (int i = -3; i < 3; i++) {
            availableNotes.push_back((start+(i*12)));
            availableNotes.push_back((start+(i*12))+2);
            availableNotes.push_back((start+(i*12))+3);
            availableNotes.push_back((start+(i*12))+5);
            availableNotes.push_back((start+(i*12))+7);
            availableNotes.push_back((start+(i*12))+8);
            availableNotes.push_back((start+(i*12))+10);
            
        }
        
    }
    
    
    
    for (int i = 0; i < availableNotes.size(); i++) {
        cout << i << ": " << availableNotes[i] << endl;
    }

}

//--------------------------------------------------------------
//Algorithm functions to determine pitch sequence
void Swarm::run(Swarm * alternateSwarm, int rhythmPlayhead, int notePlayhead, int alternateNotePlayhead) {
   
    
    fitness();
    checkPersonalBest();
    checkSwarmBest();
    
    
    //Only calculate harmonic interval fitness for one swarm.
   if (channel == 1 && desiredNoteDistance != 0) {
       
       
            //Calculate fitness taking into account harmonic intervals with alternate swarm
            harmonicIntervalFitness(alternateSwarm, rhythmPlayhead, notePlayhead, alternateNotePlayhead);
            //Calculate original fitness value (melodic intervals, distance from motif, etc) and reassess the particle's personal best candidate soltuion and best candidate solution of the swarm.
            fitness();
            checkPersonalBest();
            checkSwarmBest();
   
    }
    
    disturb();
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
                        harmonicIntervalSum += 5000;
                    } else if (interval % 7 == 0 || interval % 7 == 2 || interval % 7 == 4) {
                        harmonicIntervalSum -= 1000;
                    }
                    
                    if (interval > 8) {
                   //     harmonicIntervalSum+=1000;
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
        
        
        //Variable to store overall fitness of current particle
        float fitnessSum = 0;

        
        //Variable to store the overall note distance of the particle's candidate solution against the current phrase
        double noteDistance = 0;
        
        
        
        /*distMotifOctave is the difference between octave defined by the 'octave' slider (the octave the user and the octave of the inputted phrase notes.
        wants the swarm to operate in.)
         As there are 7 viable notes in the current scale per octave, 
         The distMotifOctave is calculated by the determining which octave the currently selected phrase mainly occupies, and depending on the distMotifOctave, this influences what octave the swarm should stive towards.
         For example, for a noteMotifOctave of 14, 16, 14, 16, this lies in octave 3.
         If the distMotifOctave is 4, the difference between the original phrase octave and the desired octave is 1 (4-3). 
         
         As an example equation:
         noteMotif = [14, 16, 18, 16] 
         particles[0]->indFreqs = [21, 28, 22, 20]
         octave of phrase = 3
         chosen octave = 4
         distMotifOctave = 4-3 = 1
         
         iteration 1:
         noteDistance += ( (14 + (1*7)) - 21) * ( (14 + (1*7)) - 21)
                      += ( 21 - 21 ) * (21 - 21)
                      += 0 * 0
                      += 0
         
         iteration 2: 
         noteDistance += (16 + (1*7)) - 28) * ( (16 + (1*7)) - 28)
                      += (23 - 28 ) * (23 - 28)
                      += (-5) * (-5)
                      += 25
         
         iteration 3:
         noteDistance += (18 + (1*7)) - 22) * ( (18 + (1*7)) - 22)
                      += (25 - 22 ) * (25 - 22)
                      += (3 * 3)
                      += 9
         
         iteration 4:
         noteDistance += (16 + (1*7)) - 20) * ( (16 + (1*7)) - 20)
                      += (23 - 20 ) * (23 - 20)
                      += (3 * 3)
                      += 9
         
         Total noteDistance = 0 + 25 + 9 + 9 = 43
         

         This calculates an overall distance of the note sequence that the particle offers and the inputted phrase by the user.
         
         */
         for (int j = 0; j < 4; j++) {
            
            noteDistance += ( (noteMotif[j]+(distMotifOctave*7)) - particles[i]->indFreqs[j]) * ( (noteMotif[j]+(distMotifOctave*7)) - particles[i]->indFreqs[j]);

        }

        
        
        
        //The total noteDistance from the particles' offered solution and the inputted phrase (according to the octave that the user would like it be searched for)
        //The fitness sum of the particle becomes the desired distance (that the user would like) subtracted from the desired phrase ditance the user would like.
        //For example, if the desired phrase distance is 0, the user would like the swarm to search for the precise phrase notes (in the desired octave). The larger the distance inputted by the user, the further away from the original phrase they would like the current output to sound.
        //Melodic intervals are not accounted for when the desiredNoteDistance is 0, as the swarm should only search for the direct notes inputted in the phrase.
        fitnessSum = (abs(desiredNoteDistance - noteDistance)*1000);
        
        
        
        //If the desired note distance is not 0 (the particle is not aiming towards the current inputted phrase, then the melodic intervals that the user would like is
        //evaluated using the interactive penalties that the user can alter.
        if (desiredNoteDistance != 0) {
            
            
        //Array to store melodic intervals of the candidate solution.
        int intervals[3];

        //Determine distance of notes 2, 3, and 4 from note 1.
        for (int j = 0; j < 3; j++) {

            
            //Distance between successive note and the current note that j indexes.
            int dist = (particles[i]->indFreqs[j+1] - particles[i]->indFreqs[j]);
            
            
            //Variable to determine the sign of the distance between the two notes.
            int sign = ofSign(particles[i]->indFreqs[j+1] - particles[i]->indFreqs[j]);
            
            
            //Assigning interval to array of intervals.
            intervals[j] = (particles[i]->indFreqs[0] - particles[i]->indFreqs[j+1])*ofSign( (particles[i]->indFreqs[0] - particles[i]->indFreqs[j+1]));
            
            
            
            //If distance is 0, this evaluates to an interval of 1 (perfect unison)
            if (dist == 0) {
                
                fitnessSum += firstPen/10;
               
            }
            
            //If distance is 1, this evaluates to an interval of 2 (slight disonnance)
            else if (dist == sign*1 || dist == (-sign)*1) {
                
                fitnessSum += secondPen/10;
                
            }
            
            //If distance is 2, this evaluates to an interval of 3 (third)
            else if (dist == sign*2 || dist == (-sign)*2) {
                
                fitnessSum += thirdPen/10;
             
            }
            
            //If ditance is 3, this evaluates to an interval of 4
            else if (dist == sign*3 || dist == (-sign)*3) {
                
                fitnessSum += fourthPen/10;
                
            }
            
            //If distance is 4, this evaluates to an interval of 5
            else if (dist == sign*4 || dist == (-sign)*4) {
                
                fitnessSum += fifthPen/10;
             
            }
            
            //If distance is 5, this evaluates to an interval of 6
            else if (dist == sign*5 || dist == (-sign)*5) {
                
                fitnessSum += sixthPen/10;
                
            }
            
            //If distance is 6, this evaluates to an interval of 7
            else if (dist == sign*6 || dist == (-sign)*6) {
                
                fitnessSum += seventhPen/10;
            
            }
            
            //If distance is 7, this evaluates to an interval of 8 (octave)
            else if (dist == sign*7 || dist == (-sign)*7) {
                
                fitnessSum += eighthPen/10;
              
            
            //Else statement evaluates to distances above 7, so any intervals higher than an octave.
            } else {
                
                fitnessSum+=elsePen/10;
            }
            
   
        }
            
            
            //Determine fitness based on whether particle candidate solution adheres to an ascending or descending contour. 
            int barSign = ofSign(intervals[0]);
            for (int j = 0; j < 2; j++) {
                if (barSign == 0) {
                    
                    if (intervals[j+1] == 0) {
                        fitnessSum+=100000;
                    }
                }
                
                if (barSign == -1) {
                    if (intervals[j+1] > intervals[j]) {
                        fitnessSum+=100000;
                    }
                    
                } else if (barSign == 1) {
                    if (intervals[j+1] < intervals[j]) {
                        fitnessSum+=100000;
                
                    }
                }
                
                if (intervals[j+1] > 7) {
                    fitnessSum+=500000;
                }
            }
        
        }
    
        particles[i]->fitness += fitnessSum;
        
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
            particles[i]->indFreqs[j] = ofClamp(int(((particles[i]->indFreqs[j]+particles[i]->indFreqsVel[j]))), particles[i]->indFreqs[j]-8, particles[i]->indFreqs[j]+8);

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

void Swarm::exit() {
    
    midiOut.closePort();
}


//--------------------------------------------------------------
////////////RHYTHM////////////////////
void Swarm::runRhythm() {
    
    //Check fitness of particle's candidate solutions
    fitnessRhythm();
    
    //Evaluate if candidate solution is particle's new personal best
    checkPersonalBestRhythm();
    
    //Evaluate if candidate solution is best solution of the swarm
    calculateBestRhythm();
    
    //Perform update process on candidate soltuion
    updateParticlesRhythm();
}

//--------------------------------------------------------------
//Fitness function of rhythm component
void Swarm::fitnessRhythm() {
    
    //Checking each particle's rhythm candidate solution
    for (int i = 0; i < N; i++) {
        
        //Variable to store overall fitness
        float fitnessSum = 0;

        
        //Rhythm distance evaluates as the particle's distance from the chosen dimensionality
        double rhythmDistance = 0;
        rhythmDistance = abs(targetDimensionality-particles[i]->dimensionality);
        
        //Fitness sum evaluates as the particle's distance from the target rhythm dimensionality
        fitnessSum = (rhythmDistance);
   
        //Evaluate particle's fitness for rhythm component as overall fitness sum.
        particles[i]->fitnessRhythm = fitnessSum;
        
    }
}

//--------------------------------------------------------------
//Determine if particle's rhythm candidate solution is it's new personal best.
void Swarm::checkPersonalBestRhythm() {
    
    for (int i = 0; i < N; i++) {
        
        
        //If the particle's current fitness for it's rhythm candidate solution is less than it's overall best rhythm fitness,
        //clear the vector 'bestRhythm' storing it's previous best rhythm sequence, and the variable storing the number
        //for it's previous best rhythm dimensionality and replace with it's current rhythm sequence and rhythm dimensionality.
        if (particles[i]->fitnessRhythm < particles[i]->bestFitnessRhythm) {
            
            //Clearing previous bestRhythm vector and bestDimensionality with current rhythm sequence and dimensionality.
            particles[i]->bestRhythm.clear();
            particles[i]->bestDimensionality = particles[i]->dimensionality;
            
            
            //Storing value of current rhythm sequence into personal best rhythm sequence.
            for (int j = 0; j < particles[i]->rhythm.size(); j++) {
                particles[i]->bestRhythm.push_back(particles[i]->rhythm[j]);
            }
            
            //Best rhythm fitness now becomes the current rhythm fitness of the particle.
            particles[i]->bestFitnessRhythm = particles[i]->fitnessRhythm;
            
        }
    }
    
}

//--------------------------------------------------------------
//Determine if any particle offers a rhythm candidate solution that is 'stronger' than any previous candidate solutions.
void Swarm::calculateBestRhythm() {
    
    for (int i = 0; i < N; i++) {
        
        //If rhythm fitness is lower than the previous global best rhythm fitness of the swarm, assign the 'bestRhythm' particle
        //to point at the particle offer the new fittest solution.
        if (particles[i]->fitnessRhythm < bestFitnessRhythm) {
            
            bestFitnessRhythm = particles[i]->fitnessRhythm;
            
            
            bestRhythm = *particles[i];
            bestRhythm.dimensionality = particles[i]->dimensionality;
            bestRhythm.hits = particles[i]->hits;
            bestRhythm.bestRhythm = particles[i]->rhythm;
            bestRhythm.bestDimensionality = particles[i]->dimensionality;
            
        }
        
    }
    
    
}

//--------------------------------------------------------------
//Rhythm update process for each particle. The velocity is calculated using the PSO algorithm equation, and then added to the particle's 'dimensionality' value.
//Based upon the new 'dimensionality' number, which is clamped between 1 and 16 to allow for valid rhythm sequences, the particle's 'rhythm' and 'hit' vectors
//are recalculated.
void Swarm::updateParticlesRhythm() {
    
    for (int i = 0; i < particles.size(); i++) {
        
        //Clear particle vectors that store the numeric rhythm values and hits of the rhythm to update their dimensionality and create new rhythm and hit sequences.
        particles[i]->rhythm.clear();
        particles[i]->hits.clear();
        
        
        //Algorithm stochastic variables
        r1 = ofRandom(1);
        r2 = ofRandom(1);
        
        //Calculating new velocity for the dimensionality update process of the individual particle.
        particles[i]->dimensionalityVel = (rhythmCon * (particles[i]->dimensionalityVel + (rhythmC1*r1*(particles[i]->bestDimensionality-particles[i]->dimensionality) + (rhythmC2*r2*(bestRhythm.bestDimensionality - particles[i]->dimensionality)))))*5.;
        
        //Adding velocity to current dimensionality
        particles[i]->dimensionality = (particles[i]->dimensionality + particles[i]->dimensionalityVel);
        
        //As the rhythm can only contain the range of a 4-beat, to 1/16 beats, the range of hits that can be contained in the hit vector ranges from 1 to 16.
        particles[i]->dimensionality = int(ofClamp(particles[i]->dimensionality, 1, 16));
        
        
        //Create new rhythm and hit sequence for the particle.
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
        
        particles[i]->velocity = ofClamp(int(particles[i]->velocity + particles[i]->velocityVel), 1, maxVelocity);
    }
}

