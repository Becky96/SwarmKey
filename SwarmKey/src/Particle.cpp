//  SWARM KEY
//  Particle.cpp

#include "Particle.hpp"

//--------------------------------------------------------------
/*Function initialises each particle with a note sequence and velocitys, rhythm sequence and velocitys, and velocity level.*/
void Particle::setupParticle() {
    
    //Initialising first element of note sequence and best note sequence array.
    indFreqs[0] = start;
    bestIndFreqs[0] = start;

    //Assigning all remaining notes in the note sequence and best note sequence array with a note that is at a random distance (between 0 and 8) intervals away. Velocities between -3 and 3 are created for each note in the sequence to use when updating.
    for (int i = 1; i < 16; i++) {
        indFreqs[i] = start+int(ofRandom(0, 8));
        bestIndFreqs[i] = start+int(ofRandom(0, 8));
        indFreqsVel[i] = ofRandom(-3, 3);
    }

    //Determine rhythm will create a valid rhythm sequence in that only durations of 4, 2, 1, 0.5, and 0.25 are available and each rhythm sequence will add up to 4.
    determineRhythm();
    
    //Initialising best rhythm sequence as the size of the newly generated rhythm sequence and as 0.
    for (int i = 0; i < rhythm.size(); i++) {
        bestRhythm.push_back(0);
    }
    
    //Assigning velocity as a random value between 20 and maximum velocity level.
    velocity = int(ofRandom(20, maxVelocity));
    
}

//--------------------------------------------------------------
/*Function initialises a rhythm sequence for each particle when it is created. A sequence of durations is created that adds to 4, and then an array 'hits' is created as a binary representation of the rhythm sequence to signal in ofApp::sendMidi() when to send the correct MIDI messages.*/
void Particle::determineRhythm() {
    
    //Reset dimensionality to 0
    dimensionality = 0;
    //Clear all previous rhythm and hit sequence values.
    rhythm.clear();
    hits.clear();
    
    //r is a random integer between 0 and 5 that acts as an index in the 'validDurations' array. This will act as the first duration of the rhythm sequence.
    int r = int(ofRandom(0, 5));
    
    //Adding first note duration.
    rhythm.push_back(validDurations[r]);
    
    //Sum is total of all rhythm sequence durations.
    float sum = rhythm[0];
    
    //Until sum is equal to 4, each if statement acts as a threshold to determine which durations can be added to the rhythm sequence based on the overall sum. This is so that it is not too computationally expensive, as if there is no threshold to only allow certain durations to be added at certain points, and the sum total has to equal 4, for a population size of 50 this has caused an incredible delay and make the output impossible to run. Once it has been determined which valid duration can be added to the sequence based on the current rhythm sequence total, the new duration is added to the sequence until the sum is 4.
    while (sum != 4) {
        
        //If the current sum total is above or equal to 3.75, the only valid duration that can be added is 0.25.
        if (sum >= 3.75) {
            r = 4;
            rhythm.push_back(validDurations[r]);
        
        //If the current sum total is above 3 and below or equal to 3.5, the only valid durations that can be added are 1, 0.5, and 0.25.
        } else if (sum > 3 && sum <= 3.5) {
            
            r = int(ofRandom(3, 5));
            rhythm.push_back(validDurations[r]);
        
        //If the current sum total is above 2 and below or equal to3, the possible valid durations that can be added are 1, 0.5, and 0.25.
        } else if (sum > 2 && sum <= 3) {
            
            r = int(ofRandom(2, 5));
            rhythm.push_back(validDurations[r]);
        
        //If the current total is larger than 0 but lower than 2, then the possible valud durations are 2, 1, 0.5, and 0.25.
        } else if (sum > 0 && sum <= 2) {
            
            r = int(ofRandom(1, 5));
            rhythm.push_back(validDurations[r]);
            
        }
        
        //Sum is reset, and then recalculated as the sum of all of the rhythm sequence durations.
        sum = 0;
        
        for (int i = 0; i < rhythm.size(); i++) {
            
            sum+=rhythm[i];
            
        }
        
        //Each time this iteration occurs, a new note duration is added and so the dimensionality is incremented. The largest dimensionality possible is 16 elements.
        dimensionality++;
        
    }
    
    //Fill in hit sequence as a binary representation of the note duration. This is used in ofApp to signal when to send a MIDI message to turn on/off the current note in the note sequence.
    //For example, a note duration of 0.25 is represented as (1), whereas a note duration of 1 is represented as (1, 0, 0, 0). Each particle hits sequence is a 16-integer array and so each 1 represents the start of a note duration, and the distance between 1s in the sequence represents the durations of the notes.
    for (int i = 0; i < rhythm.size(); i++) {
        
        if (rhythm[i] == 0.25) {
            hits.push_back(1);

        } else if (rhythm[i] == 0.5) {
            hits.push_back(1);
            hits.push_back(0);

        } else if (rhythm[i] == 1) {
            hits.push_back(1);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
        } else if (rhythm[i] == 2) {
            hits.push_back(1);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
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
            
        }
    }

}

//--------------------------------------------------------------

