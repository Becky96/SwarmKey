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

//--------------------------------------------------------------
class PhraseUI {
    
    public:
    
    void setupPhraseUI();                               //Setup of UI parameters
    void updatePhraseUI();                              //Updates UI parameters
    void displayPhraseUI();                             //Displays UI parameters
    void displayPhraseList();                           //Displays current phrase list
    void displaySelectedPhrase();                       //Displays selected phrase's grid for editing
    void calculatePhraseKey(int keyType, int tonic);    //Calculates the viable notes based on the current defined key
    void displayCorrectKey();                           //Displays list of correct key notes
    
    //Coordinates of PhraseUI UI buttons
    int x = 800;
    int y = 230;
    
    //Coordinates of Phrase List UI
    int listX = 800;
    int listY = 300;
    
    //Coordinates of list of available notes
    int textX = 1120;
    int textY = 315;
    
    //Width of UI controls
    int buttonWidth = 225;
    
    //Id to classify phrases, increments/decreases whenever phrases are added/deleted
    int id = 1;
    
    //UI Components
    vector<ofxDatGuiComponent*> phraseUIComponents;
    ofxDatGuiButton* addPhrase;
    ofxDatGuiButton* playPhrase;
    ofxDatGuiButton* deletePhrase;
    ofxDatGuiLabel* phraseListLabel;
    
    //Response to UI components
    void onButtonEvent(ofxDatGuiButtonEvent e);

    //Selected phrase holds the unique id of the currently selected phrase
    int selectedPhrase = 0;
    
    //notes stores all of the possible notes that may be displayed within the Phrase area. The notes of the current key are picked from this vector and added to the currentKeyNotes to determine which correct notes to display and use.
    vector<string> notes =  {"F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
        "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
        "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
        "C6", "C#6", "D6", "D#6", "E6"};
    
    //midiNotes stores all of the possible MIDI notes that may be used by the phrase editor. The MIDI notes of the current key are used when playing the phrase.
    vector<int> midiNotes = {
        53,	54,	55,	56,	57,	58,	59,
        60,	61,	62,	63,	64,	65,	66,	67,	68,	69,	70,	71,
        72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,
        84,	85,	86,	87,	88};

    //Stores only the viable notes of the current key
    vector<string> currentKeyNotes;
    
    //Storing MIDI notes of available notes for the key
    vector<int> currentMidiNotes;
    
    //List of currently made phrases
    vector<Phrase*> phrases;
    
    //Stores the note sequence of the currently selected grid to play if 'playPhrase' is selected
    vector<int> currentPhrase = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
    
    //Array stores the note sequence of the phrase and is stores the phrase's note sequence in terms of the global 'availableNotes' vector
    int swarmNoteIndexes[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    //Phrase UI is set up to load C Major as the default key
    int currentKey = 60;
    
    //Boolean function to play current phrase
    bool playPhraseBool = false;
    
    //Boolean becomes true whenever a phrase's note sequence is editted
    bool phraseChanged = false;
    
    //Boolean becomes true whenever a phrase is deleted
    bool resetPhraseIds = false;

};
//--------------------------------------------------------------

#endif /* PhraseUI_hpp */
