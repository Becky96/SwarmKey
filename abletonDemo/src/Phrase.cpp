//  SWARM KEY
//  Phrase.cpp

#include "Phrase.hpp"

//--------------------------------------------------------------
/*Setup of phrase when created.*/
void Phrase::setupPhrase(int _id, int _buttonX, int _buttonY) {
    
    id = _id;
    buttonX = _buttonX;
    buttonY = _buttonY;
    
    //Setting up grid in which user can input notes for the musical phrase.
    //Grid is composed of 15*16 cells, with variable 'i' corresponding for the note's position in the sequence and 'j' corresponding to the available notes the grid represents.
    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 16; i++) {
            PhraseCell * pC = new PhraseCell;
            pC->setupCell(x+(i*cellWidth), y+(j*cellHeight), cellWidth, cellHeight, 1, ofMap(j, 0, 15, 15, 0), i+1 );
            phraseCells.push_back(pC);
        }
    }
    
    //Set up of phrase button, which user can use to display the musical phrase grid
    //to then edit the notes.
    selectCell = new ofxDatGuiButton(phrase + " " + ofToString(id));
    selectCell->setPosition(buttonX, buttonY);
    selectCell->setWidth(225);
    selectCell->onButtonEvent(this, &Phrase::onButtonEvent);
    selectCell->setBackgroundColor(ofColor(25, 47, 55));
    
}

//--------------------------------------------------------------
/*Updates phrase's UI button*/
void Phrase::updatePhraseButton() {
    
    selectCell->update();
    
}

//--------------------------------------------------------------
/*Draws phrase's UI button*/
void Phrase::displayPhraseButton() {
    
    selectCell->draw();
    
}

//--------------------------------------------------------------
/*Display the phrase's grid if it is currently selected.*/
void Phrase::displayPhraseGrid() {
    
    for (int i = 0; i < phraseCells.size(); i++) {
        phraseCells[i]->displayCell();
    }

}
//--------------------------------------------------------------
/*Function is called when mouse is dragged or released, and the cells that have been pressed are then highlighted or dehighlighted.*/
void Phrase::checkGridPressed() {
    
    //Looping through all phrase cells of the Phrase grid.
    for (int i = 0; i < phraseCells.size(); i++) {
        
        //Checking whether the mouse coordinates lie within the area of the phrase cell coordinates (to check which specific cell has been pressed.
        if (ofGetMouseX() > phraseCells[i]->x && ofGetMouseX() < phraseCells[i]->x+cellWidth && ofGetMouseY() > phraseCells[i]->y && ofGetMouseY() < phraseCells[i]->y+cellHeight) {
            
            //If the cell is not already highlighted, it will become clicked and unhighlight any cells in the same column as it that are highlighted as there cannot be more than 1 note playing at 1 time in the phrase generation.
            if (phraseCells[i]->highlighted == false && phraseCells[i]->changedOnce == false) {
                
                //Highlighting the selected cell
                phraseCells[i]->highlighted = true;
                
                phraseCells[i]->changedOnce = true;
                
                //phraseCells[i]->row-1  (index in midi Note vector)
                //phraseCells[i]->col-1 (index in phrase array)
                phraseList[phraseCells[i]->col-1] = phraseCells[i]->row-1;
                
                //Looping through all other cells and checking whether they lie within the same column, and if they are not the cell that has been selected then they will be unselected if they currently are selected.
                for (int j = 0; j < phraseCells.size(); j++) {
                    if (phraseCells[j]->col == phraseCells[i]->col && i != j) {
                        if (phraseCells[j]->highlighted == true) {
                            phraseCells[j]->highlighted = false;
                            phraseCells[j]->changeColour();
                        }
                    }
                }
                
                //Unselects the cell if it has been pressed and is currently selected.
            } else if (phraseCells[i]->highlighted == true && phraseCells[i]->changedOnce == false) {
                phraseCells[i]->highlighted = false;
                phraseCells[i]->changedOnce = true;
            }
            
            //Changes phrase cell to correct colour based on whether it is highlighted or not.
            phraseCells[i]->changeColour();
            
        }
    }
}

//--------------------------------------------------------------
//Phrase button determines whether to display the Phrase's grid.
void Phrase::onButtonEvent(ofxDatGuiButtonEvent e) {
    
    if (displayGrid == false) {
        //Set stripe colour to white if currently selected phrase
        selectCell->setStripeColor(255);
        displayGrid = true;
        buttonPressed = true;
    } else if (displayGrid == true) {
        //Set stripe colour to black if not currently selected phrase
        selectCell->setStripeColor(0);
        displayGrid = false;
    }
}
//--------------------------------------------------------------