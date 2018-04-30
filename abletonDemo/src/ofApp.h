#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "Swarm.hpp"
#include "ofxDatGui.h"
#include "SwarmGUI.hpp"
#include "PhraseUI.hpp"

//Number of swarms
#define SWARM_NUM 2


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void mouseDragged(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    
    ofTrueTypeFont titleFont;
    ofTrueTypeFont smallFont;



    void loadFonts();           //Loading fonts required for app text.
    void sendMIDI();
    void displayIntroScreen();      //Introducton screen contents
    void displayAreaSegments();     //Displaying line segments for different screen areas.
    void checkPhraseDeleted();
    void checkPhraseChanged();
    
    int sampleRate;
    int bufferSize;
    
    Swarm swarms[SWARM_NUM];
    Swarm swarmPort;
    
    
    SwarmGUI * left;
    SwarmGUI * right;

    //UI COMPONENTS
    vector<ofxDatGuiComponent*> globalSwarmComponents;
    ofxDatGui* gui;
    void setupUI();
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    //KEY TYPES
    //User dropdown menu for selecting key type
    vector<string> types = {"Major", "Minor"};
    ofxDatGuiDropdown* keyTypes;
    int getKeyType = 1;
    
    
    //KEY TONICS
    //User dropdown menu for selecting key tonic
    vector<string> options = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    ofxDatGuiDropdown* key;
    int keyNum = 60;

    
    
    int UIWidth = 300;                  //Width of UI buttons
    float sliderRatio = .45;            //Slider width
    
    ofxDatGuiToggle* playSwarmsToggle;
    
    //TEMPO VARIABLES
    float tempo = 4.;
    ofxDatGuiSlider* tempoSlider;
    ofParameter<int> tempoInt;
    
    
    
    
    //TIMER VARIABLES
    //Oscillator used to time the composition based upon the rhythms of the swarms
    maxiOsc timer;
    int currentCount, lastCount;
    int rhythmPlayHead = 0;
    
    
    bool changeNotesLeft = true;
    bool changeNotesRight = true;
    bool startSwarm = false;
    

    bool noteChangeLeft = false;
    bool noteChangeRight = false;



    float vel = 3;

    bool changeRhythm = true;
    int changeRhythmInt = 0;

    int pitchPlayheadLeft = 0;
    int pitchPlayheadRight = 0;

    int lastNotePlayheadLeft;
    bool calculateChordLeft = false;
    
    int lastNotePlayheadRight;
    bool calculateChordRight = false;
    
    
    int maxVelocity = 120;
   
    
    
    //PHRASE USER INTERFACE
    PhraseUI * phraseUI;
    void playCurrentPhrase();               //Activated when 'Play Selected Phrase' button is pressed.
    int playHeadPhrase = 0;                 //Playhead for MIDI note sending.

    

    
    
    //INFORMATION AREA FUNCTIONALITY
    void displayInfoAreaText();
    ofTrueTypeFont infoFont;                //Font for information area texr
    
    
    //APP LAYOUT FUNCTIONALITY
    void displaySections();
    ofTrueTypeFont sectionFont;             //Font for section titles


    
    //Introduction screen
    void introductionScreen();
    bool introScreen = true;
};
