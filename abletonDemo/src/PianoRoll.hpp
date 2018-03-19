//
//  PianoRoll.hpp
//  abletonDemo
//
//  Created by Becky Johnson on 18/03/2018.
//
//

#ifndef PianoRoll_hpp
#define PianoRoll_hpp

#include <stdio.h>
#include "ofMain.h"
#include "PianoRollCell.hpp"
#include "Swarm.hpp"

class PianoRoll {
    
    
public:
    
    int containerX, containerY;
    ofImage piano; //Image to display keys
    
    
    //Load piano image
    void setup(int _x, int _y);
    
    //Colour based on velocity and current swarm
    //Position based on key hit
    //Position based on current hit in swarm sequence
    //Timer that moves through bar
    void displayRoll();
    void resetCells(Swarm * swarm1, Swarm * swarm2);
    
    vector<PianoRollCell> pianoRollCells;
    
    int MIDINum = 83;
    int sequenceNum = 0;
    
    
};

#endif /* PianoRoll_hpp */
