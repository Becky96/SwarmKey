//  SWARM KEY
//  PhraseUI.hpp

#pragma once

#ifndef PhraseUI_hpp
#define PhraseUI_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Phrase.hpp"
#include "ofxDatGui.h"
#include "ofxMidi.h"

class PhraseUI {
    
    public:
    
    int x = 800;
    int y = 230;
    
    int listX = 800;
    int listY = 300;
    
    int textX = 1120;
    int textY = 315;
    
    int buttonWidth = 225;
    
    int id = 1;
    
    vector<ofxDatGuiComponent*> phraseUIComponents;
    ofxDatGuiButton* addPhrase;
    ofxDatGuiButton* playPhrase;
    ofxDatGuiButton* deletePhrase;
    ofxDatGuiLabel* phraseListLabel;
    
    void onButtonEvent(ofxDatGuiButtonEvent e);

    
    int selectedPhrase = 0;
    
    void setupPhraseUI();
    void updatePhraseUI();
    void displayPhraseUI();
    void displayPhraseList();
    void displaySelectedPhrase();
    void displayCorrectKey();
    
    
    //Size of vector is 36
    //indexes
    //C - 7
    //C# - 8
    //D - 9
    //D# - 10
    //E - 11,
    //F - 12
    //F# - 13
    //G - 14
    //G# - 15
    //A - 16
    //A# - 17
    //B - 18
    
    vector<string> notes =  {"F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
        "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
        "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
        "C6", "C#6", "D6", "D#6", "E6"};
    vector<int> midiNotes = {
        53,	54,	55,	56,	57,	58,	59,
        60,	61,	62,	63,	64,	65,	66,	67,	68,	69,	70,	71,
        72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,
        84,	85,	86,	87,	88};
    
    
    
    //Numbers to show the available notes (vertically to the left of the grid)
    vector<string> currentKeyNotes;
    
    //Storing MIDI notes of available notes for the key
    vector<int> currentMidiNotes;
    
    ofxMidiOut midiOut; //To play phrases
    
    //List of currently made phrases
    vector<Phrase*> phrases;
    
    
    vector<int> phraseUInoteIndexes;
    int swarmNoteIndexes[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    
    void calculatePhraseKey(int keyType, int tonic);

    
    
    
    int currentKey = 60;
    
    
    //Boolean function to play current phrase
    bool playPhraseBool = false;
    
    bool phraseChanged = false;
    
    bool resetPhraseIds = false;
    
    vector<int> currentPhrase = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
};

#endif /* PhraseUI_hpp */
