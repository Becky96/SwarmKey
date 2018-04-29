//
//  PhraseCell.cpp
//  abletonDemo


#include "PhraseCell.hpp"


void PhraseCell::setupCell(int _x, int _y, int _width, int _height, int _note, int _row, int _col) {
    x = _x;
    y = _y;
    width = _width;
    height = _height;
    note = _note;
    
    row = _row;
    col = _col;
    
    colour.set(25, 47, 55);
    
}

void PhraseCell::changeColour() {
 
    
    if (highlighted) {
        
        
        colour.set(255, 208, 11);
        
    } else {
        
        colour.set(25, 47, 55);
        
    }

    
}


void PhraseCell::displayCell() {

    //Outline of cell
    ofNoFill();
    ofSetColor(255);
    ofDrawRectangle(x, y, width, height);
    
    
    //Main colour of cell
    ofFill();
    ofSetColor(colour);
    ofDrawRectangle(x, y, width, height);
    
}


