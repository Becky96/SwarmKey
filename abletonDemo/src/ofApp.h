#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "Swarm.hpp"
#include "ofxDatGui.h" 

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
    
    int tempo = 2;
    ofxDatGuiSlider* tempoSlider;
    ofParameter<int> tempoInt;
    
    maxiOsc timer;
    int currentCount, lastCount;
    int playHead = 0;
    
    bool changeNotes = true;
    
    
    
    
    //PSO GUI
    vector<ofxDatGuiComponent*> PSOcomponents;
    ofxDatGuiSlider* numOfPSO;
    ofxDatGuiSlider* constrictionRate;
    ofxDatGuiSlider* PSOc1;
    ofxDatGuiSlider* PSOc2;
    ofxDatGuiSlider* PSOdt;
    ofxDatGuiSlider* PSOtarget;
    ofParameter<int> numOfPSOI;
    ofParameter<float> constrictionRateF;
    ofParameter<float> c1F;
    ofParameter<float> c2F;
    ofParameter<float> PSOdtF;
    ofParameter<int> PSOtI;
    
    
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
    
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
    ofxDatGui* gui;

    float vel = 3;


};
