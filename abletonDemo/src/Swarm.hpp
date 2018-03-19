//
//  Swarm.hpp
//  abletonDemo
//
//  Created by Becky Johnson on 29/01/2018.
//
//

#ifndef Swarm_hpp
#define Swarm_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxMaxim.h"
#include "Particle.hpp"

class Swarm {
    
    
public:
    
    void openVirtualPort(string portName);
    
    void setup(int _channel);
    
    void inputMotif(int nMotif[4], int rMotif[16]);
    void exit();
    
    ofxMidiOut midiOut;


    //Starting scale is C major
    int tonic = 60;
    
    int channel, note, velocity;
    
    int tempo = 120;
    
    
    //Algorithm functions
    void run(Swarm * alternateSwarm, int rhythmPlayhead, int notePlayhead, int alternateNotePlayhead);
    void fitness();
    void checkPersonalBest();
    void checkSwarmBest();
    void harmonicIntervalFitness(Swarm * alternateSwarm, int rhythmPlayhead, int notePlayhead, int alternateNotePlayhead);
    void updateParticles();
    void checkRepeat();
    void disturb();
    
    int previousBestFitness;
    
    void display();
    
    int repeated = 0;
    
    //Algoirthm variables
    vector<Particle*> particles;
    int N = 50;                 //Number of particles
    float noteCon = 0.7984;         //Constriction factor
    float noteC1, noteC2;               //Learning rates
    float r1, r2;               //Stochastic elements
    Particle best;

    
    float rhythmCon = 0.4;
    float rhythmC1, rhythmC2;

    int prevBestIndFreqs[4];
    
    
    float dt = 0.2; //Like DFO


    void calculateKey(int start, int type);
   
    vector<int> availableNotes;
    
    
    float bestFitness = 9999999999.;
    
    int bestIndex;
    
    int target = 24;
    
    int firstPen = 10;         //Interval of one (same note)
    int secondPen = 100;      //Interval of two
    int thirdPen = 0;        //Interval of three
    int fourthPen = 50;      //Interval of four
    int fifthPen = 0;         //Interval of five
    int sixthPen = 50;        //Interval of six
    int seventhPen = 100;       //Interval of seven
    int eighthPen = 0;          //Octave
    int elsePen = 100;                //Other inval

    
    
    //Rhythm
    //Rhythm functionality
    void runRhythm();
    void fitnessRhythm();
    void checkPersonalBestRhythm();
    void calculateBestRhythm();
    void updateParticlesRhythm();
    void createSequenceRhythm(int d, Particle * p);
    
    int chosenDimension;
    float validDurations[5] = {4, 2, 1, 0.5, 0.25};
    vector<float> validDur;
    int bestFitnessRhythm = 200000;
    Particle bestRhythm;
    
    int chosenOctave = 4;
    int determineParticleOctave(int index);
    
    
    bool play = false;
    bool readyToPlay = false;
    
    int stressedVelocity;
    int notStressedVelocity;
    
    
    //Velocity
    void runVelocity();
    void fitnessVelocity();
    void checkPersonalBestVelocity();
    void calculateBestVelocity();
    void updateParticleVelocity();
    
    int desiredVelocity = 40;

    int bestParticleSwarmVelocity;
    float bestVelocityFitness = 999;
    
    float velocityCon = 0.7;
    float velocityC1, velocityC2;

    int notePlayhead = 0;
    
    int maxVelocity = 80;
    
    //Motif/Phrase
    int desiredRhythmDistance = 0;
    int desiredNoteDistance = 0;
    int targetDimensionality;
    
    //Note motif of continuous tonic of whatever key currently in.
    int noteMotif[4] = {28, 30, 32, 30};
    int originalMotif[4];
    int rhythmMotif[16] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
    int dimensionalityMotif = 0;
    
    
    int chordPotential = 0;
    int prevChordPotential;
    int chordBestFitness = 9999;
    int bestChord[3];
    
    float chordCon = 0.78;
    float chordC1, chordC2;
    
    void runChord(int notePlayhead);
    void randomiseParticleChord(int p);
    void removeParticleChordIndex();
    void fitnessChord();
    void checkPersonalBestChord();
    void checkBestChord();
    void updateParticleChord();
    
    int currentNotePlayhead;
    
    
    
    //Octave
    int noteMotifOctaves[4];
    int distMotifOctave;
    
    bool playFinalNote = false;
};

#endif /* Swarm_hpp */
