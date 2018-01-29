#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    
    ofBackground(255);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    for (int i = 0; i < SWARM_NUM+1; i++) {
        
        swarms[i].openVirtualPort("IAC Driver Swarm1");
        swarms[i].setup(i);
       // swarms[i].exit();
        swarms[i].play();
        
    }
    
    
    sampleRate = 44100;
    bufferSize = 1025;
    
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    
    
    
    // Make sure to call this at the end of setup!!
    
    ofSoundStreamSetup(2,0, this, maxiSettings::sampleRate, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){

}


void ofApp::audioRequested  (float * output, int bufferSize, int nChannels){
    
    
    
    // This is the main function which calls all of the Pulse and Performer objects to play, keeping them within 3 bars of each other.
    
    // This function has been adapted from Maximilian examples
    /*
    for (int i = 0; i < bufferSize; i++){
        

        
        for (int i = 1; i < SWARM_NUM+1; i++){
            
            swarms[i].play();
            
    
            
        }
        
    } */// end for (bufferSize)
    
    
    
}   // end audioRequested

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    if (key == 'c') {
        swarms[1].play();
    
    }
    
    if (key == 'v') {
        swarms[2].play();
        
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
