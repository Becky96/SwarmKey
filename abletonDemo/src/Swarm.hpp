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


    void calculateKey(int start);
    vector<int> availableNotes;
    
    
    float bestFitness = 9999999999.;
    
    int bestIndex;
    
    int target = 24;
    
    int firstPen = 100;         //Interval of one (same note)
    int secondPen = 10000;      //Interval of two
    int thirdPen = 100;        //Interval of three
    int fourthPen = 10000;      //Interval of four
    int fifthPen = 100;         //Interval of five
    int sixthPen = 1000;        //Interval of six
    int seventhPen = 10000;       //Interval of seven
    int eighthPen = 100;          //Octave
    int elsePen = 10000;                //Other inval

    
    
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
    
    int desiredVelocity = 100;

    int bestParticleSwarmVelocity;
    float bestVelocityFitness = 999;
    
    float velocityCon = 0.78;
    float velocityC1, velocityC2;

    int notePlayhead = 0;
    
    
    
    //Motif/Phrase
    int desiredRhythmDistance = 0;
    int desiredNoteDistance = 0;
    
    //Note motif of continuous tonic of whatever key currently in.
    int noteMotif[4] = {28, 30, 32, 30};
    int rhythmMotif[16] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
    int dimensionalityMotif = 0;
    
    
    //TESTING DFO
    void calculateBestFly();
    void calculateBestNeighbour();
    void updateFly();
    void clampParticles();
    
};

#endif /* Swarm_hpp */
