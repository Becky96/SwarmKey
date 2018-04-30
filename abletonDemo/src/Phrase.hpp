//
//  Phrase.hpp
//  abletonDemo
//
//  Created by Becky Johnson on 25/04/2018.
//
//

#pragma once


#ifndef Phrase_hpp
#define Phrase_hpp

#include <stdio.h>
#include "ofMain.h"
#include "PhraseCell.hpp"
#include "ofxDatGui.h"

class Phrase {
    
    
public:
    
    int id;
    string phrase = "phrase";
    
    void setupPhrase(int _id, int _buttonX, int _buttonY);
    void updatePhraseButton();
    void displayPhraseButton();
    void displayPhraseGrid();
    void checkGridPressed();
    
    int cellWidth = 25;
    int cellHeight = 25;
    
    int x = 1150;
    int y = 300;
    
    int buttonX, buttonY;
    
    vector<PhraseCell*> phraseCells;
    
    ofxDatGuiButton* selectCell;
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    
    bool displayGrid = false;

    bool buttonPressed;
    
    
   vector<int> phraseList = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
    
    int prevId;
    
};

#endif /* Phrase_hpp */
