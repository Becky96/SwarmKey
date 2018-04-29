//
//  PhraseCell.hpp
//  abletonDemo
//


#pragma once


#ifndef PhraseCell_hpp
#define PhraseCell_hpp

#include <stdio.h>
#include "ofMain.h"

class PhraseCell {
    
    
public:
    
    int x, y;
    int width, height;
    int note;
    bool highlighted = false;
    
    int row, col;
    
    ofColor colour;
    
    void setupCell(int _x, int _y, int _width, int _height, int _note, int _row, int _col);
    void changeColour();
    void displayCell();
    void checkCellPressed();
    
    bool pressed = false;
    bool changedOnce = false;
};
#endif /* PhraseCell_hpp */
