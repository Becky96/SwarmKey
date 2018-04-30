//  SWARM KEY
//  ofApp.h

#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "Swarm.hpp"
#include "ofxDatGui.h"
#include "SwarmGUI.hpp"
#include "PhraseUI.hpp"

#define SWARM_NUM 2     //Number of swarms

//--------------------------------------------------------------
class ofApp : public ofBaseApp{

	public:
    
        //ORDER OF OFAPP FUNCTIONS//
        void loadFonts();                                 //Loading fonts required for app text.
        void setupUI();                                   //Setup UI appearence/options
		void setup();
		void update();
		void draw();
        void sendMIDI();                                  //Sending MIDI messages of real-time Swarm composition
        void checkSwarmsStopPlaying();                    //Determine whether to run PSO processes.
        void checkSwarmsPlaying();                        //Determine wher to stop PSO processes.
        void playCurrentPhrase();                         //Activated when 'Play Selected Phrase' button is pressed.
        void checkPhraseDeleted();                        //Responds to whether a phrase has been deleted from the Phrase List
        void checkPhraseChanged();                        //Update Swarm's with correct target Phrase note sequences
        void onToggleEvent(ofxDatGuiToggleEvent e);       //Toggle responses
        void onSliderEvent(ofxDatGuiSliderEvent e);       //Slider responses
        void onDropdownEvent(ofxDatGuiDropdownEvent e);   //Dropdown responses
		void mouseDragged(int x, int y, int button);      //Determines which cells have been altered in Phrase grids
		void mouseReleased(int x, int y, int button);     //Determines which cells have been altered in Phrase grids
        void displayIntroScreen();                        //Introducton screen contents
        void displayAreaSegments();                       //Displaying line segments for different screen areas.
        void displayInfoAreaText();                       //Display correct text in information area.
        /////////////////////////////

    //////OBJECTS/VARIABLES//////
    
    //AUDIO//
    int sampleRate;
    int bufferSize;
    /////////
    
    //SWARMS//
    Swarm swarms[SWARM_NUM];                          //Array of swarms as defined by SWARM_NUm const
    Swarm swarmPort;                                  //SwarmPort needed for Ableton
    bool startSwarm = false;                          //Boolean to toggle between running/stopping algorithmic processes
    //////////
    
    //SWARM UI//
    SwarmGUI * left;
    SwarmGUI * right;
    ////////////

    //GLOBAL UI//
    vector<ofxDatGuiComponent*> globalSwarmComponents;
    ofxDatGui* gui;
    ofxDatGuiToggle* playSwarmsToggle;               //Toggle to run/stop both swarms
    int UIWidth = 300;                               //Width of UI buttons
    float sliderRatio = .45;                         //Slider width
    //////////////
    
    //KEY TYPES//
    //User dropdown menu for selecting key type
    vector<string> types = {"Major", "Minor"};
    ofxDatGuiDropdown* keyTypes;
    int getKeyType = 1;                             //Integer represents current key type
    //////////////
    
    //KEY TONICS//
    //User dropdown menu for selecting key tonic
    vector<string> options = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    ofxDatGuiDropdown* key;
    int keyNum = 60;                                //Integer represents key tonic
    //////////////

    //TEMPO VARIABLES//
    float tempo = 4.;                               //Tempo float is converted from BPM in tempo slider to frequency for the 'timer' oscillator
    ofxDatGuiSlider* tempoSlider;
    ofParameter<int> tempoInt;
    ///////////////////
    
    //TIMER VARIABLES//
    //Oscillator used to time the composition based upon the rhythms of the swarms
    maxiOsc timer;
    int currentCount, lastCount;                    //Variables to compare together to check whether there has been a time step
    int rhythmPlayHead = 0;                         //Used to increment through rhythm sequence
    ///////////////////
    
    //VELOCITY//
    //Booleans determine whether to run velocity algorithm process.
    bool noteChangeLeft = false;                    //Swarm 1 check
    bool noteChangeRight = false;                   //Swarm 2 checl
    int maxVelocity = 120;                          //Max velocity of output
    /////////////

    //RHYTHM//
    bool changeRhythm = true;                       //Determines when to run algorithmic rhythm process
    int changeRhythmInt = 0;                        //Determines how many times the rhythm sequence has been played without running the algorithmic process
    //////////

    //PHRASE UI//
    PhraseUI * phraseUI;                            //PhraseUI object holds all functionality for PhraseUI
    int playHeadPhrase = 0;                         //Playhead for MIDI note sending.
    /////////////
    
    //FONTS//
    ofTrueTypeFont titleFont;                       //Introduction title font
    ofTrueTypeFont smallFont;                       //Introduction text font
    ofTrueTypeFont sectionFont;                     //Font for section titles
    ofTrueTypeFont infoFont;                        //Font for information area text
    ////////
    
    //INTRO SCREEN//
    bool introScreen = true;                        //Determines whether to display introduction screen or main program
    ////////////////
};
//--------------------------------------------------------------
