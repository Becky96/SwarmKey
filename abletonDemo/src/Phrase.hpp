//  SWARM KEY
//  Phrase.hpp

#pragma once

#ifndef Phrase_hpp
#define Phrase_hpp

#include <stdio.h>
#include "ofMain.h"
#include "PhraseCell.hpp"
#include "ofxDatGui.h"

class Phrase {
    
public:
    
    //FUNCTIONS
    void setupPhrase(int _id, int _buttonX, int _buttonY);      //Setup of phrase.
    void updatePhraseButton();                                  //Update the phrase's button.
    void displayPhraseButton();                                 //Display the phrase's selecting button in the phrase list.
    void displayPhraseGrid();                                   //Display the phrase's grid on screen.
    void checkGridPressed();                                    //Used to determine which cells have been pressed when the mouse is dragged or released.
    void onButtonEvent(ofxDatGuiButtonEvent e);                 //UI response to selectCell button

    
    //VARIABLES/OBJECTS
    int id;                             //Phrase's personal id
    int prevId;                         //If phrase list is reorganised (when another phrase has been deleted), prevId is used to reselect correct phrase for a swarm if it is using it as a target phrase.
    string phrase = "phrase";           //String used for display in button
    int cellWidth = 25;                 //Width and height of grid cells
    int cellHeight = 25;
    int x = 1150;                       //Starting coordinates of Phrase's grid.
    int y = 300;
    ofxDatGuiButton* selectCell;        //Button used to select/deselect cell.
    int buttonX, buttonY;               //Coordinates of Phrase's individual button used to select as a target by the Swarms or to edit it.
    vector<PhraseCell*> phraseCells;    //Vector storing the phraseCells that compose the grid.
    bool displayGrid = false;           //Boolean is set to true when phrase is selected and is used to determine which phrase to display.
    bool buttonPressed;                 //Determines if selectCell button has been pressed.
    vector<int> phraseList = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};  //Vector stores the note sequence that the phrase's grid represents. The numeric values represent it's index in the available notes grid.

};

#endif /* Phrase_hpp */
