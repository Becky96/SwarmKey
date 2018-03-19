//
//  PianoRollCell.cpp
//  abletonDemo
//
//  Created by Becky Johnson on 18/03/2018.
//
//

#include "PianoRollCell.hpp"

void PianoRollCell::setup(int _x, int _y, int _MIDINum, int _sequenceNum) {
    x = _x;
    y = _y;
    MIDINum = _MIDINum;
    sequenceNum = _sequenceNum;
    
 
    
    cout << "x: " << x << endl;
    cout << "y: " << y << endl;
    
    
    cellWidth = 35;
    cellHeight = 14;
}


void PianoRollCell::displayCell() {
    

    if (highlighted == false) {
        setHighlight = false;
    } else {
        setHighlight = true;
    }
    
    if (setHighlight == false) {
    ofNoFill();
    ofSetColor(255);
    ofDrawRectangle(x, y, 35, 14);
    } else {
        
        cout << "HIGHLIGHTING" << endl;
        ofFill();
        ofSetColor(255, 0, 0);
        ofDrawRectangle(x, y, 35, 14);
    }
    
    
    string sequence = ofToString(MIDINum);
    ofDrawBitmapStringHighlight(sequence, x, y+5);

    
   // ofDrawRectangle(600, 600, 50, 100);
}