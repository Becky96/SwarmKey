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
#include "ofxMidi.h"
#include "ofxMaxim.h"
#include "Particle.cpp"

class Swarm {
    
    
public:
    
    void openVirtualPort(string portName);
    
    void setup(int _channel);
    
    void exit();
    
    void play();
    
    
    ofxMidiOut midiOut;
    
    int channel, note, velocity;
    
    int tempo = 120;
    
    
    //Algorithm functions
    void run();
    void fitness();
    void checkPersonalBest();
    void checkSwarmBest();
    void updateParticles();
    void checkRepeat();
    void disturb();
    
    
    void display();
    
    int repeated = 0;
    
    //Algoirthm variables
    vector<Particle*> particles;
    int N = 100;                 //Number of particles
    float con = 0.7984;         //Constriction factor
    float c1, c2;               //Learning rates
    float r1, r2;               //Stochastic elements
    Particle best;


    int prevBestIndFreqs[4];
    
    
    float dt = 0.01; //Like DFO


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

};

#endif /* Swarm_hpp */