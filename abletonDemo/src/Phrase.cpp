//
//  Phrase.cpp
//  abletonDemo
//
//  Created by Becky Johnson on 25/04/2018.
//
//

#include "Phrase.hpp"


void Phrase::setupPhrase(int _id, int _buttonX, int _buttonY) {
    id = _id;
    buttonX = _buttonX;
    buttonY = _buttonY;
    
    
    
    //Setting up grid in which user can input notes for the musical phrase
    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 16; i++) {
            
            
            PhraseCell * pC = new PhraseCell;
            pC->setupCell(x+(i*cellWidth), y+(j*cellHeight), cellWidth, cellHeight, 1, j+1, i+1);
            phraseCells.push_back(pC);
            
        }
    }
    
    //Set up of phrase button, which user can use to display the musical phrase grid
    //to then edit the notes.
    selectCell = new ofxDatGuiButton(phrase + " " + ofToString(id));
    selectCell->setPosition(buttonX, buttonY);
    selectCell->onButtonEvent(this, &Phrase::onButtonEvent);
    

}

//Update Phrase button
void Phrase::updatePhraseButton() {
    
    selectCell->update();
}

//Draw Phrase button
void Phrase::displayPhraseButton() {
    
    selectCell->draw();
}


//Display Phrase's grid
void Phrase::displayPhraseGrid() {
    
    for (int i = 0; i < phraseCells.size(); i++) {
        phraseCells[i]->displayCell();
    
    }


}


void Phrase::checkGridPressed() {
  
    
    //Looping through all phrase cells of the Phrase grid.
    for (int i = 0; i < phraseCells.size(); i++) {
        
        
        //Checking whether the mouse coordinates lie within the area of the phrase cell coordinates (to check which specific cell has been pressed.
        if (ofGetMouseX() > phraseCells[i]->x && ofGetMouseX() < phraseCells[i]->x+cellWidth && ofGetMouseY() > phraseCells[i]->y && ofGetMouseY() < phraseCells[i]->y+cellHeight) {
            
            
            //If the cell is not already highlighted, it will become clicked and unhighlight any cells in the same column as it that are highlighted as there cannot be more than 1 note playing at 1 time in the phrase generation.
            if (phraseCells[i]->highlighted == false) {
                
                //Highlighting the selected cell
                phraseCells[i]->highlighted = true;
                
                
                //Looping through all other cells and checking whether they lie within the same column, and if they are not the cell that has been selected then they will be unselected if they currently are selected.
                for (int j = 0; j < phraseCells.size(); j++) {
                    if (phraseCells[j]->col == phraseCells[i]->col && i != j) {
                        if (phraseCells[j]->highlighted == true) {
                            phraseCells[j]->highlighted = false;
                        }
                    }
                }
        
            
                
                //Unselects the cell if it has been pressed and is currently selected.
            } else if (phraseCells[i]->highlighted == true) {
                phraseCells[i]->highlighted = false;
                
            }
        
            
        }
        
    }
    
   
}





//Phrase button determines whether to display the Phrase's grid.
void Phrase::onButtonEvent(ofxDatGuiButtonEvent e) {
    
    if (displayGrid == false) {
        displayGrid = true;
        buttonPressed = true;
    } else if (displayGrid == true) {
        displayGrid = false;
    }
}