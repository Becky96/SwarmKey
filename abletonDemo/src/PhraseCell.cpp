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
    
}


void PhraseCell::displayCell() {

    //Outline of cell
    ofNoFill();
    ofSetColor(0);
    ofDrawRectangle(x, y, width, height);
    
    if (highlighted) {
        
        
        
        ofFill();
        ofSetColor(255, 0, 0);
        
        
    } else {
        

        
        ofFill();
        ofSetColor(255);
        
    }

    ofDrawRectangle(x, y, width, height);
    
}


