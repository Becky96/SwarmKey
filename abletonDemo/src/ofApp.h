#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "Swarm.hpp"
#include "ofxDatGui.h"
#include "SwarmGUI.hpp"
#include "PianoRoll.hpp"

//Number of swarms
#define SWARM_NUM 2


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		

    void audioRequested (float * input, int bufferSize, int nChannels); // Adapted from Maximilan examples

    
    int sampleRate;
    
    int bufferSize;
    
    Swarm swarms[SWARM_NUM];
    Swarm swarmPort;
    
    SwarmGUI * left;
    SwarmGUI * right;
    
    
    //Object responsible for the 
    PianoRoll pianoRoll;
    
    vector<ofxDatGuiComponent*> globalSwarmComponents;
    ofxDatGuiToggle* playSwarmsToggle;
    
    //Tempo variables
    float tempo = 4.;
    ofxDatGuiSlider* tempoSlider;
    ofParameter<int> tempoInt;
    
    vector<string> types = {"Major", "Minor"};
    ofxDatGuiDropdown* keyTypes;
    int getKeyType = 1;
    
    
    vector<string> options = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    ofxDatGuiDropdown* key;
    int keyNum = 60;
    
    maxiOsc timer;
    int currentCount, lastCount;
    int playHead = 0;
    
    bool changeNotesLeft = true;
    bool changeNotesRight = true;
    bool startSwarm = false;
    

    bool noteChangeLeft = false;
    bool noteChangeRight = false;

    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    ofxDatGui* gui;

    float vel = 3;

    bool changeRhythm = true;
    int changeRhythmInt = 0;

    int pitchPlayheadLeft = 0;
    int pitchPlayheadRight = 0;

    int lastNotePlayheadLeft;
    bool calculateChordLeft = false;
    
    int lastNotePlayheadRight;
    bool calculateChordRight = false;
    
    
    //Temporary variable for recording with Disklavier 
    int maxVelocity = 80;
   
};
