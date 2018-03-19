//
//  PianoRollCell.hpp
//  abletonDemo
//
//  Created by Becky Johnson on 18/03/2018.
//
//

#ifndef PianoRollCell_hpp
#define PianoRollCell_hpp

#include <stdio.h>
#include "ofMain.h"

class PianoRollCell {
    
public:
    
    void setup(int _x, int _y, int _MIDINum, int _sequenceNum);
    void displayCell();
    
    int x, y;
    int MIDINum;
    int sequenceNum;
    
    int cellWidth = 35;
    int cellHeight = 14;
    
    bool highlighted = false;
    bool setHighlight = false;
};

#endif /* PianoRollCell_hpp */
