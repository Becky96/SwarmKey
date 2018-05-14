//  SWARM KEY
//  Swarm.hpp

#ifndef Swarm_hpp
#define Swarm_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxMaxim.h"
#include "Particle.hpp"

//--------------------------------------------------------------
class Swarm {
    
public:
    
    //MIDI//
    void openVirtualPort(string portName);  //Opening port with correct port name
    void setup(int _channel);               //Setting up swarm in unique channel
    void exit();                            //Closing port
    int channel, note, velocity;
    ofxMidiOut midiOut;                     //ofxMidi object used to send messages to Ableton


    //CURRENT KEY//
    int tonic = 60;                         //Starting scale is C major
    void inputMotif(int nMotif[16]);        //Takes phrase note sequence as target note sequence
    void calculateKey(int start, int type);
    vector<int> availableNotes;             //Vector to store MIDI note numbers which correspond with the current key.
    int notePlayhead = 0;                   //Note playhead to determine which note to play in the sequence
    
    
    //SWARM PLAY FUNCTIONALITY
    bool play = false;                      //Determines whether to run algorithmic process on swarm
    bool readyToPlay = false;               //Prepares algorithmic process
    bool playFinalNote = false;             //Determines whether to play final note
    

    //PARTICLE POPULATION
    vector<Particle*> particles;
    
    
    //PHRASES
    int noteMotif[16] = {21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21}; //Array holds note sequence of target phrase's note sequence
    int desiredNoteDistance = 0;            //Desired input from target phrase
    int phraseId;                           //Id of target phrase
    int noteMotifOctaves[16];               //Used to store octaves of phrase target note sequence's octaves
    int distMotifOctave;                    //Stores distance between user-preferred octave and octave of candidate solution.
    int chosenOctave;                       //User-defined octave

    
    //NOTE SEQUENCE//
    void run(Swarm * alternateSwarm, int rhythmPlayhead, int notePlayhead, int alternateNotePlayhead);
    void fitness();
    void checkPersonalBest();
    void checkSwarmBest();
    void harmonicIntervalFitness(Swarm * alternateSwarm, int rhythmPlayhead, int notePlayhead, int alternateNotePlayhead);
    void updateParticles();
    void disturb();
    
    //Algoirthm variables
    int N = 20;                             //Number of particles
    float noteCon = (0.7984);                 //Constriction factor
    float noteC1, noteC2;                   //Learning rates
    float r1, r2;                           //Stochastic elements
    float dt = 0.01;                         //Dispersive Flies Optimisation - disturbance threshold
    Particle best;                          //Pointer to best particle of note sequence calculations
    float bestFitness = 9999999999.;        //Best fitness of swarm
    int bestIndex;                          //Index of best particle in particles vector
    int numOfIterations = 1;                //Defined by searchIntensity slider in ofApp, determines how many times to run the algorithmic process.
    int prevBestIndFreqs[16];
    int repeated = 0;                       //Used to determine how many times the note sequence has been repeated.
    
    //Note sequence interval penalties
    int firstPen = 10;                      //Interval of one (same note)
    int secondPen = 100;                    //Interval of two
    int thirdPen = 0;                       //Interval of three
    int fourthPen = 50;                     //Interval of four
    int fifthPen = 0;                       //Interval of five
    int sixthPen = 50;                      //Interval of six
    int seventhPen = 100;                   //Interval of seven
    int eighthPen = 0;                      //Octave
    int elsePen = 100;                      //Intervals higher than an octave
    
    
    //Chords
    int chordPotential = 0;                 //Used to determine likelihood of playing chord

    
    //RHYTHM//
    //Rhythm functionality
    void runRhythm();
    void fitnessRhythm();
    void checkPersonalBestRhythm();
    void calculateBestRhythm();
    void updateParticlesRhythm();
    void createSequenceRhythm(int d, Particle * p);
    
    //Algorithm variables
    float rhythmCon = 0.45;                  //Constraint factor
    float rhythmC1, rhythmC2;               //Learning coefficients
    int targetDimensionality;               //The target rhythm dimensionality as defined as the user
    float validDurations[5] = {4, 2, 1, 0.5, 0.25};
    vector<float> validDur;                 //Vector holding the available note durations: 4, 2, 1, 0.5, 0.25
    int bestFitnessRhythm = 200000;         //Global best fitness
    Particle bestRhythm;                    //Pointer to particle with best rhythm sequence
    int chosenDimension;                     //User defined preferred rhythm dimensionality
    int rhythmPopSize = 20;

    
    //VELOCITY//
    void runVelocity();                     //Encompasses all following algorithm functions
    void fitnessVelocity();                 //Determine particle's velocity fitness
    void checkPersonalBestVelocity();       //Determine whether their current velocity is a new personal best
    void calculateBestVelocity();           //Determine whether their current velocity is a new swarm best
    void updateParticleVelocity();          //Update velocity using PSO equation
    
    //Algorithm variables
    float velocityCon = 0.65;                //Constraint factor
    float velocityC1, velocityC2;           //Learning coefficients
    int desiredVelocity = 60;               //Initial desired velocity when prorgam is launched
    int bestParticleSwarmVelocity;          //Best velocity of the swarm
    float bestVelocityFitness = 999;        //Global best fitness
    int maxVelocity = 120;                  //Maximum possible velocity to output
    int stressedVelocity;                   //Velocity of bars 1st note
    int notStressedVelocity;                //Velocity of following notes
    
    
    //ALGORITHM ANALYSIS FUNCTIONALITY
    //These functions/variables were used in the analysis of the algorithms performance and are not run typically during the run-time of the program.
    //They have been included to show the methodology of testing the algorithm's diversity and error margin.
    int iteration = 0;                      //Keeping track of current iteration
    void diversityAnalysis();               //Function containing diversity calculations
    vector<float> noteDimensionsAverage = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};        //Array storing averages of all dimensions values
    float noteDiversity;//Overall diversity
    void errorAnalysis();
    float noteError;
    bool changePhraseDist1 = true;
     bool changePhraseDist2 = true;
     bool changePhraseDist3 = true;
    vector<string> noteDiversityCoord;
    vector<string> bestFitnessCoord;
    bool printVals = true;
    
    
    //Rhythm analysis
    void rhythmDiversityAnalysis();
    void rhythmErrorAnalysis();
    float rhythmDiversity;
    float rhythmError;
    float rhythmAverage;
    bool changeRhythm0 = true;
    bool changeRhythm1 = true;
    bool changeRhythm2 = true;
    bool changeRhythm3 = true;
    vector<string> rhythmDiversityCoord;
    vector<string> rhythmErrorCoord;
    bool printRhythmVals = true;
    int rhythmIteration = 0;
    
    
    //Velocity analysis
    void velocityDiversityAnalysis();
    void velocityErrorAnalysis();
    float velocityDiversity;
    float velocityAverage;
    float velocityError;
    bool changeVelocity0 = true;
    bool changeVelocity1 = true;
    bool changeVelocity2 = true;
    bool changeVelocity3 = true;
    vector<string> velocityDiversityCoord;
    vector<string> velocityErrorCoord;
    bool printVelocityVals = true;
    
    
    
    
    
    
    

};
//--------------------------------------------------------------
#endif /* Swarm_hpp */
