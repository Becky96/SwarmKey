#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"

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
    
    
    vector<float> rhythm;
    vector<int> hits;
    float validDurations[5] = {4, 2, 1, 0.5, 0.25};
    
    void changeRhythm();
		
    
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

};
