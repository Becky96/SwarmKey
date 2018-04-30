
#ifndef SwarmGUI_hpp
#define SwarmGUI_hpp

#include <stdio.h>
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
    string label;
    
    SwarmGUI() = default;
    SwarmGUI(int _channel, int _x, int _y, Swarm * _swarm, string _label) : channel(_channel), x(_x), y(_y), swarm(_swarm), label(_label) {}


    
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
    

    //Vector containing interface components for individual components of swarm.
    vector<ofxDatGuiComponent*> swarmComponents;
    ofxDatGuiLabel* swarmLabel;
    ofxDatGuiSlider* rhythmSlider;
    ofxDatGuiSlider* velocitySlider;
    ofxDatGuiSlider* octaveSlider;
    ofxDatGuiSlider* chordSlider;

    ofParameter<int> rhythmInt;
    ofParameter<int> velocityInt;
    ofParameter<int> octaveInt;
    ofParameter<int> chordInt;
    
    //Vector containing the individual sliders for the desired intervals that the user wants.
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
    
   
    vector<ofxDatGuiComponent*> motifComponents;
    ofxDatGuiSlider* desiredRhythmDistSlider;
    ofxDatGuiSlider* desiredNoteDistSlider;
    ofxDatGuiSlider* searchIntensitySlider;
    ofxDatGuiToggle* selectMotifToggle;
    ofxDatGuiLabel* currentMotifLabel;
    
    bool assignNewPhrase = false;

    
    ofParameter<int> desiredNoteDistInt;
    ofParameter<int> desiredRhythmDistInt;
    ofParameter<int> searchIntensityInt;
    

    
    int maxVelocity = 120;

    int playHead;
    
    int UIWidth = 300;
    float sliderRatio = .45;
    
    int phraseId;
    
    bool phraseIdChanged = false;
};

#endif /* SwarmGUI_hpp */
