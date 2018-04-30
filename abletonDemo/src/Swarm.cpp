//  SWARM KEY
//  Swarm.cpp

#include "Swarm.hpp"

//--------------------------------------------------------------
/*ofxMidi addon function to open MIDI ports.*/
void Swarm::openVirtualPort(string portName) {

    //Listing available ports.
    midiOut.listPorts();
    ofxMidiOut::listPorts();
    
    //Opening port to Ableton with 'IAC Driver Swarm1'
    midiOut.openVirtualPort(portName);
    
}
//--------------------------------------------------------------
/*Setup swarm with channel number, initial note number to send, default velocity of 120.*/
void Swarm::setup(int _channel) {
    
    channel = _channel;             //Channel corresponds to the channel of the Port in Ableton.
    
    //Default note and velocity
    stressedVelocity = 100;         //'Stressed' velocity for the first note of the bar to create emphasis.
    notStressedVelocity = 80;       //'notStressed' for teh following notes of the bar.
    
    //Initialising particle population.
    for (int i = 0; i < N; i++) {
        Particle *p = new Particle();
        particles.push_back(p);
        particles[i]->setupParticle();

    }
    
    
    //bestRhythm particle is created and setup to begin the program with a rhythm sequence of (1, 1, 1, 1)
    bestRhythm = *new Particle();
    bestRhythm.setupParticle();
    
    for (int i = 0; i < 16; i++) {
        bestRhythm.hits[i] = 0;
    }
    
    //Creating rhythm hits for bestRhythm.
    bestRhythm.hits[0] = 1;
    bestRhythm.hits[4] = 1;
    bestRhythm.hits[8] = 1;
    bestRhythm.hits[12] = 1;
    
    //Creating rhythm sequence for bestRhyhtm.
    bestRhythm.rhythm.clear();
    bestRhythm.rhythm.push_back(1);
    bestRhythm.rhythm.push_back(1);
    bestRhythm.rhythm.push_back(1);
    bestRhythm.rhythm.push_back(1);
    
    
    //Setting default dimensionality to 4 when program is launched.
    bestRhythm.dimensionality = 4;
    bestRhythm.bestDimensionality = 4;
    
    //Best rhythm of the swarm is set to (1, 1, 1, 1) by default.
    bestRhythm.bestRhythm = bestRhythm.rhythm;
    
    //As the 'rhythm' slider is set to 4 in the main program, until this slider is changed to a different preferred dimensionality, the bestFitnessRhythm is 0 as the bestRhythm particle will have a correct dimensionality of 4.
    bestFitnessRhythm = 0;
    
    //Note sequence learning coefficients
    noteC1 = noteCon * (4.1/2);
    noteC2 = noteC1;
    
    //Rhythm sequence learning coefficients
    rhythmC1 = rhythmCon * (4.1/2.);
    rhythmC2 = rhythmC1;
    
    //Velocity learning coefficients
    velocityC1 = velocityCon * (4.1/2.);
    velocityC2 = velocityC1;
    
    //Calculating the correct notes for the key that the program is loaded as. By default, the program is loaded with C Major as the intiial key.
    calculateKey(tonic, 1);
    
    //Initialising prevBestIndFreqs to check for cases of repetition later on.
    for (int i = 0; i < 16; i++) {
        prevBestIndFreqs[i] = 100;
    }

    
 
    
}
//--------------------------------------------------------------
/*This function takes the selected phrase note sequence and sets it to the swarms target note sequence. This is called whenever a new phrase is selected for the Swarm's target, or when the Swarm's currently selected phrase is altered.*/
void Swarm::inputMotif(int nMotif[16]) {
    
    //Assigning note motif as sthe new swarm warm note motif
    for (int i = 0; i < 16; i++) {
        noteMotif[i] = nMotif[i];
    }
    
    //Calculating octaves of the input target note sequence
    for (int i = 0; i < 16; i++) {
        noteMotifOctaves[i] = (floor(noteMotif[i])/7)+1;
    }

}


//--------------------------------------------------------------
/*This function takes the inputted tonic, and determines the available notes as a vector that the swarm is able to output.*/
void Swarm::calculateKey(int start, int type) {
    
    //Tonic of the key
    tonic = start;
    
    //Clear all previous available notes from the vector.
    availableNotes.clear();
    
    //Type determines whether the key is major or minor
    //Both for loops, depending on whether it is major or minor, load 6 octaves of the key into the available notes vector.
    //As major and minor scales require different interval steps to create them, there are two for loops that are executed dependent on whether the input type is major or minor.
    //If major key:
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
    
    //If minor key:
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

}

//--------------------------------------------------------------
/*This function holds all of the algorithmic processes for determining the 'strongest' note sequences. This is based upon the desired octave, desired distance from the target phrase note sequence, and the defined interval preferences if the desired phrase note distance is more than 0.
    The number of times the PSO algorithm process is run is determined by the 'searchIntensity' slider, present in SwarmGUI. If the user would like the Swarm to be more likely to find the exact phrase note sequence straight away, they can increase the value of this slider in order for the algorithmic process to be run up to 200 times before deciding on the note sequence to be outputted.
 By increasing this slider's value, this grants a far higher likelihood of the swarm finding the target note sequence, compared to performing one iteration of the algorithm process.*/
void Swarm::run(Swarm * alternateSwarm, int rhythmPlayhead, int notePlayhead, int alternateNotePlayhead) {
    
    
    //numOfIterations is defined by the 'searchIntensity' slider.
    //When the program is run, it is by default set to 1.
    //By increasing this number, there is a far higher chance of the swarm finding the correct note sequence at a faster rate.
    for (int i = 0; i < numOfIterations; i++) {
        
        fitness();                  //Calculate the fitness of each particle
        checkPersonalBest();        //Check whether the particle has a new personal best
        checkSwarmBest();           //Check whether the particle has a new overall best of the swarm
    
    
    //This if statement checks whether or not the Swarm is Swarm 1 or Swarm 2. If the swarm is Swarm 1, it will craete new fitnesses for each particle in the population that check the intervals between it's individual note sequence and the note sequence of the best particle of Swarm 2. This allows for a higher chance of more consonant harmonic intervals.
    //This process is only run for Swarm 1, as it would be meaningless to perform this process for both.
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
    
    

}

//--------------------------------------------------------------
/*This function assesses the harmonic intervals between the currently assessed Particle of the swarm, and the best-ranked particle of Swarm 2. The algorithmic process for Swarm 2 is done before Swarm 1, so that Swarm 1's particles can be assessed for their harmonic intervals and a new best particle can be determined based on this factor of their note sequence as well.*/
void Swarm::harmonicIntervalFitness(Swarm *alternateSwarm, int rhythmPlayhead, int notePlayhead, int alternateNotePlayhead) {
    
    
    int tempNotePlayhead;               //Playhead to iterate through the note sequences of the particles
    int tempAlternateNotePlayhead;      //Playhead to iterate through the note sequences of the best particle of Swarm 2
    
    
    //Loop through all particles to compare harmonic intervals between itself and the best particle of the Swarm 2.
    for (int i = 0; i < particles.size(); i++) {
        
        particles[i]->fitness = 0;                      //Each particle's fitness is reset to 0.
        
        float harmonicIntervalSum = 0;                  //A sum is calculated of the harmonic intervals between each note of its 16-length array against the 16-length array of the best particle of swarm 2.
        
        //tempoNotePlayhead and tempAlternateNotePlayhead are set to the playheads of Swarm 1 and Swarm 2 present in the ofApp functionality
        tempNotePlayhead = notePlayhead;
        tempAlternateNotePlayhead = alternateNotePlayhead;
        
        
        //Looping through all 16 indexes of the particle's note sequence
        for (int j = 0; j < 16; j++) {
        
            //If particle has a hit at the same index of the best particle of the alternate swarm, calculate the harmonic interval.
            if (particles[i]->hits[j] == 1) {
                
                //Checks that the best particle of Swarm 2 also has a rhythm hit at the same time.
                if (alternateSwarm->bestRhythm.hits[j] == 1) {
                
                    //Calculate absolute difference between the two note sequences.
                    int interval = abs(alternateSwarm->best.indFreqs[tempAlternateNotePlayhead]-particles[i]->indFreqs[tempNotePlayhead]);

                    //Add penalty to intervals of 2, 4, and 7 between swarms as we do not want occurances of these harmonic intervals.
                    if (interval % 7 == 1 || interval % 7 == 3 || interval % 7 == 6 ) {
                        harmonicIntervalSum += 5000;
                        
                    //Add 'bonuses' to intervals of 1, 3, and 5 as we do want occurances of these harmonic intervals.
                    } else if (interval % 7 == 0 || interval % 7 == 2 || interval % 7 == 4) {
                        harmonicIntervalSum -= 1000;
                    }
                    
                    //Increment the note playheads to calculate other following harmonic intervals within the particle sequences.
                    tempAlternateNotePlayhead++;
                    tempNotePlayhead++;
                    
                }
            }
        }
        
        //Particles fitness is then set to overall sum of harmonicIntervalSum
        particles[i]->fitness+=harmonicIntervalSum;
        
    }
    
    //Best fitness is reset in order to assess whether it's new candidate solution that will be based upon the harmonic interval testing will become a new best of the swarm.
    bestFitness = 9999999;
    
    
}
//--------------------------------------------------------------
/*Fitness function for note sequences. This function determines the fitness of the particle by assessing it's:
 -Distance from the phrase target note sequence. The desired distance is changeable by the user, so the overall distance calculated by the 'linear least squares' method is then subtracted from the desired distance that the user wants. 
 -The desired octave level
 -The melodic intervals (when the desired phrase distance is more than 0)
 -Whether the intervals follow an ascending or descending contour (when the desired phrase distance is more than 0)*/
void Swarm::fitness() {
    
    for (int i = 0; i < particles.size(); i++) {
        
        //Variable to store overall fitness of current particle based upon factors listed above.
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
         for (int j = 0; j < 16; j++) {
            
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
        int intervals[15];

        //Determine distance of notes 2, 3, and 4 from note 1.
        for (int j = 0; j < 15; j++) {

            
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
            //The sign of the first interval is calculated to determine whether there is a descending or ascending route from the first note sequence note to the second. All other intervals are then assessed to check whether they follow the same ascending or descending pattern.
            //This is to stop a 'random' feeling in the output, which as humans we are able to detect if there is not a logical contour in note sequences.
            int barSign = ofSign(intervals[0]);
            for (int j = 0; j < 15; j++) {
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
    
        //Particles fitness adds the fitnessSum variable to its overall fitness.
        //For Swarm 1, this now becomes an overall sum of this fitnessSum, and the fitnessSum calculated by the harmonic intervals.
        particles[i]->fitness += fitnessSum;
        
    }
    
    
}
//--------------------------------------------------------------
/*This function checks whether each particle has a new personal best for it's note sequence candidate solution.*/
void Swarm::checkPersonalBest() {

    //Loop through all particles
    for (int i = 0; i < N; i++) {
        
        //If particle's current fitness is lower than it's individual bestFitness, its current fitness becomes the new best fitness, and it's current note sequence is stored as its best note sequence.
        if (particles[i]->fitness < particles[i]->bestFit) {
            
            particles[i]->bestFit = particles[i]->fitness;
            
            for (int j = 0; j < 16; j++) {
                particles[i]->bestIndFreqs[j] = particles[i]->indFreqs[j];
            }
        }
    }
}

//--------------------------------------------------------------
/*This function checks whether any particle has a note sequence which is assessed as the best note sequence of the swarm.*/
void Swarm::checkSwarmBest() {
    
    //Loop through all particles
    for (int i = 0; i < particles.size(); i++) {
        
        //If particle's current fitness is lower than the global bestFitness, bestFitness is now equal to the particle's current fitness and the 'best' particle points to this particle.
        if (particles[i]->fitness < bestFitness) {
            bestFitness = particles[i]->fitness;
            best = *particles[i];
            bestIndex = i;
        }
    }
    
}

//--------------------------------------------------------------
/*This function performs the PSO update equation on each particle. Each note is updated using it's corresponding note velocity in the direction of the global best note sequence and its individual best note sequence.*/
void Swarm::updateParticles() {
    
    //Looping through all particles
    for (int i = 0; i < particles.size(); i++) {
        
        r1 = ofRandom(1);
        r2 = ofRandom(1);
        
        //Updating each note in the particle's note sequence
        for (int j = 0; j < 16; j++) {
            
            //Velocity update
            particles[i]->indFreqsVel[j] = noteCon * (particles[i]->indFreqsVel[j] + (noteC1*r1*(particles[i]->bestIndFreqs[j]-particles[i]->indFreqs[j])+ noteC2*r2*(best.bestIndFreqs[j]-particles[i]->indFreqs[j])));
            
            //Particle note update.
            //Each note is represented as the index in the availableNotes vector so that particle's may only offer note sequences of notes that adhere to the current key. In order to stop sporadic movement across the available notes, each update may only propel the note to 8 note indexes above or below it.
            particles[i]->indFreqs[j] = ofClamp(int(((particles[i]->indFreqs[j]+particles[i]->indFreqsVel[j]))), particles[i]->indFreqs[j]-8, particles[i]->indFreqs[j]+8);

        }
    }
}

//--------------------------------------------------------------
/*This function aids the swarm in becoming "unstuck" in a sequence that is a local optima as defined by the fitness function. This also allows for variation by taking the repeated sequence and changing it slightly, similar to the mutation operation of a genetic algorithm.
 When the desired phrase distance is more than 0, in order to stop the current best-defined note sequence from being repeated for too many iterations, a check is implemented that will spur particles to explore more of the search space in order to discover other viable solutions.*/
void Swarm::checkRepeat() {
    
    int indexCheck = 0;
    
    //Check if previous frequency indexes of the last iteration matches the current frequency indexes of the current iteration of the best particle.
    //If yes, increment for each
    for (int i = 0; i < 16; i++) {
        if (prevBestIndFreqs[i] == best.indFreqs[i]) {
            indexCheck++;
        }
    }
    
    //If all indexes matched, this means that the note sequence is the same and so 'repeated' is incremented by 1. 2 repetitions are allowed before forcing best particle to randomise or vary
    if (indexCheck == 16) {
        repeated++;
    }
    
    //If sequence has been repeated twice, loop through all particles and randomly reset some of them based upon random variable r. Also using a random variable, alter some of the values of the best particle's indFreq array to hopefully create some variation of the sequence.
    if (repeated == 2) {
        
        //Loop through all particles
        for (int i= 0; i < N; i++) {
            
            float r = ofRandom(1);
            
            //If r is larger than 0.75, displace the particle's note sequence by a note one above or below the current note.
            if (r > 0.75) {
                for (int j = 0; j < 16; j++) {
                    if (particles[i]->indFreqs[j] >= 2 && particles[i]->indFreqs[j] <= availableNotes.size()-1) {
                    particles[i]->indFreqs[j] = particles[i]->indFreqs[j]+int(ofRandom(-1, 1));
                    }
                }
            }
        }
        
        
        //All 16 notes of the sequences are iterated through, if the value of r is larger than 0.25, displace the specific note by either 2 notes above or below the current note.
        for (int i = 0; i < 16; i++) {
            float r = ofRandom(1);
            
            if (r > 0.25) {
                best.indFreqs[i] = best.indFreqs[i]+int(ofRandom(-2, 2));
                prevBestIndFreqs[i] = 100;
            }
        }
        
        //Reset repeated to 0
        repeated = 0;

    } else {
        
        //If the note sequence has not been repeated, set the previousBestIndFreqs to best.indFreqs to check in the next iteration.
        for (int i = 0; i < 16; i++) {
            
            prevBestIndFreqs[i] = best.indFreqs[i];
            
        }
    }
}

//--------------------------------------------------------------
/*This function borrows the disturbance threshold mechanism from Dispersive Flies Optimisation in order to increase diversity among the population. If the randomly generated variable r exceeds a certain variable, the particle will reset its note sequence to discover potentially stronger soltuions.*/
void Swarm::disturb() {
    
    //Loop through all particles
    for (int i = 0; i < N; i++) {
        
        float r = ofRandom(1);
        
        //If r is smaller than the disturbance threshold, reset the note sequence and velocities of the particle.
        if (r < dt) {
            
            for (int j = 0; j < 16; j++) {

                particles[i]->indFreqs[j] = int(ofRandom(21-8, 21+8));
                particles[i]->indFreqsVel[j] = ofRandom(-2, 2);
                
            }
        }
    }
}

//--------------------------------------------------------------
/*Closing the MIDI port when the program is closed.*/
void Swarm::exit() {
    midiOut.closePort();
}

//--------------------------------------------------------------
/*This function encompasses the algorithmic processes for the rhythm candidate solutions of the particles.*/
void Swarm::runRhythm() {
    
    //Check fitness of particle's candidate solutions
    fitnessRhythm();
    
    //Evaluate if candidate solution is particle's new personal best
    checkPersonalBestRhythm();
    
    //Evaluate if candidate solution is best solution of the swarm
    calculateBestRhythm();
    
    //Perform update process on rhythm candidate soltuion
    updateParticlesRhythm();
}

//--------------------------------------------------------------
/*Fitness function of rhythm candidate solutions.*/
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
   
        //Particle's fitness for rhythm component equates to overall fitness sum.
        particles[i]->fitnessRhythm = fitnessSum;
        
    }
}

//--------------------------------------------------------------
/*This function determines whether each particle has a new personal best rhythm sequence.*/
void Swarm::checkPersonalBestRhythm() {
    
    //Loop through all particles
    for (int i = 0; i < N; i++) {
        
        
        /*If the particle's current fitness for it's rhythm candidate solution is less than its overall best rhythm fitness, its 'bestRhythm' vector, storing it's previous best rhythm sequence, is cleared and the variable storing the number for it's previous best rhythm dimensionality is replaced with its current rhythm sequence and rhythm dimensionality.*/
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
/*This function determines if any particle offers a rhythm candidate solution that is 'stronger' than any previous candidate solutions in the overall swarm.*/
void Swarm::calculateBestRhythm() {
    
    for (int i = 0; i < N; i++) {
        
        //If rhythm fitness is lower than the previous global best rhythm fitness of the swarm, assign the 'bestRhythm' particle to point at the particle that offers the new fittest solution.
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
/*This function contains the PSO rhythm update process for each particle. The velocity is calculated using the PSO algorithm equation, and then added to the particle's 'dimensionality' value. Based upon the new 'dimensionality' number, which is clamped between 1 and 16 to allow for valid rhythm sequences, the particle's 'rhythm' and 'hit' vectors are recalculated.*/
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
        
        
        //Create new rhythm and hit sequence for the particle based upon its newly calculated dimensionality.
        createSequenceRhythm(particles[i]->dimensionality, particles[i]);
    }

}

//--------------------------------------------------------------
/*This function creates a new rhythm sequence for each particle based upon its newly defined dimensionality. This function is called upon each update iteration.*/
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
        
        
    //If the newly added note is not 4
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
    
    //Based upon the newly generated rhythm sequence, the hit sequence is then filled with the corresponding binary representation of the sequence.
    //For example, for a note duration of 2, the hit sequence would push (1, 0, 0, 0, 0, 0, 0, 0) into the hits vector.
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
//VELOCITY FUNCTIONALITY
//--------------------------------------------------------------
/*This function encompassess all of the algorithmic processes for the velocity candidate solutions.*/
void Swarm::runVelocity() {
    
    fitnessVelocity();                  //Assessing each particle's fitness
    checkPersonalBestVelocity();        //Checking whether each particle's current fitness is a new personal best
    calculateBestVelocity();            //Checking whether each particle's current fitness is a new global best
    updateParticleVelocity();           //Updating the particle's velocity candidate solution
    
}

//--------------------------------------------------------------
/*Fitness function for velocity*/
void Swarm::fitnessVelocity() {
    
    //Loop through all particles
    for (int i = 0; i < N; i++) {
    
        //The particles' velocity fitness is defined by the distance between it's current velocity and the desired velocity as set by the user.
        //By using the 'least squares' method, velocitys that are further away from the desired velocity have a much lower fitness than by a linear subtraction method.
        particles[i]->velocityFitness = (desiredVelocity-particles[i]->velocity) * (desiredVelocity-particles[i]->velocity);
    }
}
//--------------------------------------------------------------
/*This function assesses whether each particle has a new personal best velocity candidate solution.*/
void Swarm::checkPersonalBestVelocity() {
    
    //Loop through all particles
    for (int i = 0; i < particles.size(); i++) {
        
        //If the particle's current fitness is lower than its personal best, its current velocity and fitness becomes the new personal best velocity and fitness.
        if (particles[i]->velocityFitness < particles[i]->bestParticleVelocityFitness) {
            particles[i]->bestParticleVelocity = particles[i]->velocity;
            particles[i]->bestParticleVelocityFitness = particles[i]->velocityFitness;
        }
    }
}

//--------------------------------------------------------------
/*This function assesses whether a particle has a new global best velocity candidate solution.*/
void Swarm::calculateBestVelocity() {
    
    //Loop through all particles
    for (int i = 0; i < N; i++) {
        
        //If particle's fitness is lower than the global best fitness, the particles velocity and fitness becomes the new best global velocity and fitness.
        if (particles[i]->velocityFitness < bestVelocityFitness) {
            bestParticleSwarmVelocity = particles[i]->velocity;
            bestVelocityFitness = particles[i]->velocityFitness;
        }
    }
}

//--------------------------------------------------------------
/*This function performs the PSO algorithm update equation on the particle's velocity candidate solution.*/
void Swarm::updateParticleVelocity() {
    
    //Loop through all particles
    for (int i = 0; i < N; i++) {
        r1 = ofRandom(1);
        r2 = ofRandom(1);
        
        //Update the velocity's velocity
        particles[i]->velocityVel = velocityCon * (particles[i]->velocityVel + (velocityC1*r1*(particles[i]->bestParticleVelocity - particles[i]->velocity) + (velocityC2*r2*(bestParticleSwarmVelocity - particles[i]->velocity))));
        
        //Add velocity's velocity to velocity (Not the best named)
        particles[i]->velocity = ofClamp(int(particles[i]->velocity + particles[i]->velocityVel), 1, maxVelocity);
    }
}

//--------------------------------------------------------------


