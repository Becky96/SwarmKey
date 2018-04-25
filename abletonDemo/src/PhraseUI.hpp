//
//  PhraseUI.hpp
//  abletonDemo
//


#pragma once


#ifndef PhraseUI_hpp
#define PhraseUI_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Phrase.hpp"
#include "ofxDatGui.h"

class PhraseUI {
    
    
    public:
    
    int x = 800;
    int y = 800;
    
    int listX = 800;
    int listY = 700;
    
    int id = 1;
    
    vector<ofxDatGuiComponent*> phraseUIComponents;
    ofxDatGuiButton* addPhrase;

    void onButtonEvent(ofxDatGuiButtonEvent e);

    
    int selectedPhrase = 0;
    
    void setupPhraseUI();
    void updatePhraseUI();
    void displayPhraseUI();
    void displayPhraseList();
    void displaySelectedPhrase();
    void displayCorrectKey();
    
    vector<string> notes =  {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    
    
    
    //List of currently made phrases
    vector<Phrase*> phrases;
    
    
};

#endif /* PhraseUI_hpp */
