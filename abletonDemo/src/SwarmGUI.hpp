//  SWARM KEY
//  SwarmGUI.hpp

#ifndef SwarmGUI_hpp
#define SwarmGUI_hpp

#include <stdio.h>
#include "ofxDatGui.h"
#include "Swarm.hpp"

//--------------------------------------------------------------
/*Class responsible for the user interface of the swarm it controls. This allows the user to control components of the individual swarm that an object of this class will be responsible for. This will control components of swarm such as melodic intervals, rhythm "speed", distance from target phrase, current target phrase, current velocity, and current octave.*/
class SwarmGUI {
    
public:
    
    int channel;                            //Port channel of individual swarm
    int x, y;                               //Coordinates of UI components
    int textY;
    Swarm * swarm;                          //Pointer to swarm that UI controls
    int vel = 3;                            //Velocity used when resetting particles
    string label;
    
    SwarmGUI() = default;
    SwarmGUI(int _channel, int _x, int _y, Swarm * _swarm, string _label) : channel(_channel), x(_x), y(_y), swarm(_swarm), label(_label) {}
    
    void setupInterface();
    void updateInterface();
    void drawInterface();
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);

    //Resetting particle algorithm processes/components is required when changing any of the musical parameters. When a new value has been defined, the search space changes and the particle's best solution found so far may in fact be an entirely weaker solution, and so each best fitness for each musical segment needs to be reset, as well as the best sequences and velocitys to propel particles towards other new solutions.
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
    
    //Vector containing the individual sliders for the desired user intervals.
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
    ofParameter<int> desiredNoteDistInt;
    ofParameter<int> desiredRhythmDistInt;
    ofParameter<int> searchIntensityInt;
    
    bool assignNewPhrase = false;       //Becomes true/false when using 'selectNewMotif' toggle to select/deselect phrases as targets
    int maxVelocity = 120;              //Maximum output velocity
    int UIWidth = 300;                  //Width of UI components
    float sliderRatio = .45;            //Slider ratio of slider component
    int phraseId;                       //Phrase id of currently selected phrase
    bool phraseIdChanged = false;       //Responds when phrase id is changed if a phrase has been deleted
};
//--------------------------------------------------------------
#endif /* SwarmGUI_hpp */
