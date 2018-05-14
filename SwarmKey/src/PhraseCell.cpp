//  SWARM KEY
//  PhraseCell.cpp

#include "PhraseCell.hpp"

//--------------------------------------------------------------
/*Setup function for each cell in the Phrase's grid. Each cell is initialised with a position, width, height, the note that it represents in the grid, and its row and column in the grid. Its row in the grid represents its note in the grid also, and its column represents its position in the note sequence that the grid specifies.*/
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
//--------------------------------------------------------------
/*This function is called from Phrase.cpp when the mouse is dragged or released. When the mouse is dragged in the position of the cell's coordinates, if it is currently highlighted (active in the note sequence), it is set to inactive and vice versa.*/
void PhraseCell::changeColour() {
    
    if (highlighted) {
        colour.set(255, 208, 11);               //If highlighted, set colour back to highlighted colour
    } else {
        colour.set(25, 47, 55);                 //If not highlighted, set colour back to default grid colour
    }
}

//--------------------------------------------------------------
/*Draws the phrase cell to the screen.*/
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
//--------------------------------------------------------------



