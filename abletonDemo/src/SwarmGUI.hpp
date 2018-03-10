//
//  SwarmGUI.hpp
//  abletonDemo
//
//  Created by Becky Johnson on 23/02/2018.
//
//

#ifndef SwarmGUI_hpp
#define SwarmGUI_hpp

#include <stdio.h>
//#include "ofMain.h"
#include "ofxDatGui.h"
#include "Swarm.hpp"

//Class responsible for the user interface of the swarm it controls.
//This allows the user to control components of the individual swarm that
//an object of this class will be responsible for.

//Will control components of swarm such as:
//-Is swarm playing
//-Interval distance
//-Rhythm speed
//-Velocity
//-Tempo
//-Current octave

class SwarmGUI {
    
public:
    
    int channel;
    int x, y;
    int textY;
    Swarm * swarm;
    int vel = 3;
    
    SwarmGUI() = default;
    SwarmGUI(int _channel, int _x, int _y, Swarm * _swarm) : channel(_channel), x(_x), y(_y), swarm(_swarm) {}


    
    void setupInterface();
    void updateInterface();
    void drawInterface();
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void resetParticleIntervals();
    void resetParticleRhythms();
    void resetParticleIntervalVelocity();
    void resetParticleRhythmVelocity();
    void resetParticleVelocity();
    void displaySwarmParameters();
    
    //PSO algorithm components - will not be visible to the user
    //but is used in the meantime for designing the project.
    vector<ofxDatGuiComponent*> algorithmComponents;
    ofxDatGuiSlider* noteConSlider;
    ofxDatGuiSlider* noteC1Slider;
    ofxDatGuiSlider* noteC2Slider;
    ofxDatGuiSlider* rhythmConSlider;
    ofxDatGuiSlider* rhythmC1Slider;
    ofxDatGuiSlider* rhythmC2Slider;
    
    ofParameter<float> noteConFloat;
    ofParameter<float> noteC1Float;
    ofParameter<float> noteC2Float;
    ofParameter<float> rhythmConFloat;
    ofParameter<float> rhythmC1Float;
    ofParameter<float> rhythmC2Float;
    
    vector<ofxDatGuiComponent*> swarmComponents;
    ofxDatGuiToggle* playingToggle;
    ofxDatGuiSlider* rhythmSlider;
    ofxDatGuiSlider* velocitySlider;
    ofxDatGuiSlider* octaveSlider;

    ofParameter<int> rhythmInt;
    ofParameter<int> velocityInt;
    ofParameter<int> octaveInt;
    
    //Interval penalties
    vector<ofxDatGuiComponent*> intervalPenalties;
    ofxDatGuiSlider* firstPen;
    ofxDatGuiSlider* secondPen;
    ofxDatGuiSlider* thirdPen;
    ofxDatGuiSlider* fourthPen;
    ofxDatGuiSlider* fifthPen;
    ofxDatGuiSlider* sixthPen;
    ofxDatGuiSlider* seventhPen;
    ofxDatGuiSlider* eighthPen;
    ofxDatGuiSlider* elsePen;

    ofParameter<int> firstInt;
    ofParameter<int> secondInt;
    ofParameter<int> thirdInt;
    ofParameter<int> fourthInt;
    ofParameter<int> fifthInt;
    ofParameter<int> sixthInt;
    ofParameter<int> seventhInt;
    ofParameter<int> eighthInt;
    ofParameter<int> elseInt;
    
    int playHead;
    
    vector<ofxDatGuiComponent*> motifComponents;
    ofxDatGuiSlider* desiredRhythmDistSlider;
    ofxDatGuiSlider* desiredNoteDistSlider;
    
    ofParameter<int> desiredNoteDistInt;
    ofParameter<int> desiredRhythmDistInt;

};

#endif /* SwarmGUI_hpp */
