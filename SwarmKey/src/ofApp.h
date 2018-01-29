#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "PSO.hpp"

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
    
    ofTrueTypeFont myfont;
    
    vector<float> rhythm;
    vector<int> hits;
    float validDurations[5] = {4, 2, 1, 0.5, 0.25};
    
		
    
    void audioOut(float *output, int bufferSize, int nChannels);
    unsigned bufferSize;
    unsigned sampleRate;
    
    //Sequencing
    int currentCount, lastCount;
    maxiOsc timer;
    int playHead = 0;                       //Playhead for rhythm sequence
    int playHeadR = 0;

    maxiEnv semibreve, minim, crotchet, quaver, semiquaver, chosen;
    maxiOsc osc;
    
    bool play = false;

    
    vector<PSO*> psoSwarms;
    int numSwarms = 1;
    
    ofxDatGui* gui;
    
    vector<ofxDatGuiComponent*> penalties;
    void onSliderEvent(ofxDatGuiSliderEvent e);
    ofxDatGuiSlider* semibrevePen;
    ofxDatGuiSlider* minimPen;
    ofxDatGuiSlider* crotchetPen;
    ofxDatGuiSlider* quaverPen;
    ofxDatGuiSlider* semiquaverPen;
    ofxDatGuiSlider* dimension;
    
    
    
    ofParameter<float> semibrevePenF;
    ofParameter<float> minimPenF;
    ofParameter<float> crotchetPenF;
    ofParameter<float> quaverPenF;
    ofParameter<float> semiquaverPenF;
    ofParameter<int> dimensionI;
    
    
    //PSO GUI
    vector<ofxDatGuiComponent*> PSOcomponents;
    ofxDatGuiSlider* numOfPSO;
    ofxDatGuiSlider* constrictionRate;
    ofxDatGuiSlider* PSOc1;
    ofxDatGuiSlider* PSOc2;
    //ofxDatGuiSlider* PSOdt;
    ofParameter<int> numOfPSOI;
    ofParameter<float> constrictionRateF;
    ofParameter<float> c1F;
    ofParameter<float> c2F;
    //ofParameter<float> PSOdtF;
    
    bool changeRhythm = false;

    
    int tempo = 4;
    
    ofxDatGuiSlider* tempoSlider;
    ofParameter<int> tempoInt;
};
