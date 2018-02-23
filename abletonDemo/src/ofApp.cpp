#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    gui->setAssetPath("");

    ofSetVerticalSync(true);
    
    ofBackground(255);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    for (int i = 1; i < SWARM_NUM+1; i++) {
        
        swarms[i].openVirtualPort("Swarm");
        swarms[i].setup(i);
        
    }
    
    left = new SwarmGUI(1, 700, 200, &swarms[1]);
    right = new SwarmGUI(2, 1000, 200, &swarms[2]);
    
    left->setupInterface();
   right->setupInterface();
    
    int x = 400;
    int y = 200;
    
    tempoSlider = new ofxDatGuiSlider(tempoInt.set("Tempo", 8, 1, 12));
    tempoSlider->setPosition(x, y);
    tempoSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    y+=tempoSlider->getHeight();
    
    
    sampleRate = 44100;
    bufferSize = 1025;
    
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2,0, this, maxiSettings::sampleRate, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){


    tempoSlider->update();
    
    left->updateInterface();
    right->updateInterface();


}


void ofApp::audioRequested(float * output, int bufferSize, int nChannels){
    
    if (startSwarm) {
    
    for (int i = 0; i < bufferSize; i++){
        
        currentCount = (int)timer.phasor(tempo);
        
        if (lastCount != currentCount) {
            
            if (playHead % 16 == 0) {
                changeRhythm = true;
                changeRhythmInt++;
            }
            
            //Determine whether to play first swarm
            if (swarms[1].play == true) {
                
                
                if (swarms[1].bestRhythm->hits[playHead% 16] == 1) {
            
                    if (playHead % 16 == 0) {
                        swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[pitchPlayheadLeft%4]], swarms[1].stressedVelocity);
                    } else {
                        swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[pitchPlayheadLeft%4]], swarms[1].notStressedVelocity);
                    
                    }
                    
                    
                    swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[pitchPlayheadLeft%4]]);

                    pitchPlayheadLeft++;

                }
                
                
            }

            
            //Determine whether to play second swarm
            if (swarms[2].play == true) {
                
                
                if (swarms[2].bestRhythm->hits[playHead% 16] == 1) {
                    
                    if (playHead % 16 == 0) {
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[pitchPlayheadRight%4]], swarms[2].stressedVelocity);
                    } else {
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[pitchPlayheadRight%4]], swarms[2].notStressedVelocity);
                        
                    }
                    
                    
                    swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[pitchPlayheadRight%4]]);
                    
                    pitchPlayheadRight++;
                    
                }
                
                
            }



            if (pitchPlayheadLeft % 4 == 3 || pitchPlayheadRight % 4 == 3) {
                changeNotes = true;
            }
            
            
            playHead++;
            lastCount = 0;
            
        
        }
        
    }
  }
    
    
    
}   // end audioRequested

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    tempoSlider->draw();
    
    left->drawInterface();
    right->drawInterface();
  
    
    if (startSwarm == true) {
        
        //Determine whether to change note sequences
        if (changeNotes == true) {

            if (pitchPlayheadLeft == 3) {
            if (swarms[1].play == true) {
                swarms[1].run();

            }
            }
            
            if (pitchPlayheadRight == 3) {
            if (swarms[2].play == true) {
                swarms[2].run();
                cout << "playing notes" << endl;
            }
            }
            
            pitchPlayheadLeft = 0;
            pitchPlayheadRight = 0;
            changeNotes = false;
        }
        
        //Determine whether to change rhythm sequences
        if (changeRhythm == true) {
            
            if (swarms[1].play == true) {
                swarms[1].runRhythm();


            }
            
            
            if (swarms[2].play == true) {
                swarms[2].runRhythm();
                cout << "playing rhythm" << endl;
            }
            
            changeRhythm = false;
        }
    }
    

    swarms[1].display();

    ofPushMatrix();
    ofTranslate(0, 400);
    swarms[2].display();
    ofPopMatrix();

    
    string sequence = "Sequence: " + ofToString(swarms[1].best.indFreqs[0]) + ", " + ofToString(swarms[1].best.indFreqs[1]) + ", " + ofToString(swarms[1].best.indFreqs[2]) + ", " + ofToString(swarms[1].best.indFreqs[3]);
    ofDrawBitmapStringHighlight(sequence, 20, 200);

    sequence = "MIDI equivalent: " + ofToString(swarms[1].availableNotes[swarms[1].best.indFreqs[0]]) + ", " + ofToString(swarms[1].availableNotes[swarms[1].best.indFreqs[1]]) + ", " + ofToString(swarms[1].availableNotes[swarms[1].best.indFreqs[2]]) + ", " + ofToString(swarms[1].availableNotes[swarms[1].best.indFreqs[3]]);
    ofDrawBitmapStringHighlight(sequence, 20, 250);



    string sequence2 = "Sequence 2: " + ofToString(swarms[2].best.indFreqs[0]) + ", " + ofToString(swarms[2].best.indFreqs[1]) + ", " + ofToString(swarms[2].best.indFreqs[2]) + ", " + ofToString(swarms[2].best.indFreqs[3]);
    ofDrawBitmapStringHighlight(sequence2, 20, 300);
    
    sequence2 = "MIDI equivalent 2: " + ofToString(swarms[2].availableNotes[swarms[2].best.indFreqs[0]]) + ", " + ofToString(swarms[2].availableNotes[swarms[2].best.indFreqs[1]]) + ", " + ofToString(swarms[2].availableNotes[swarms[2].best.indFreqs[2]]) + ", " + ofToString(swarms[2].availableNotes[swarms[2].best.indFreqs[3]]);
    ofDrawBitmapStringHighlight(sequence2, 20, 325);
    
    
    string bestFit = "Best fitness currently: " + ofToString(swarms[1].bestFitness);
    ofDrawBitmapStringHighlight(bestFit, 20, 360);

    
    
    string rhythmSequence = "Sequence: " + ofToString(swarms[1].bestRhythm->rhythm);
    string rhythmBest = "Dimensionality: " + ofToString(swarms[1].bestRhythm->dimensionality);

    ofDrawBitmapStringHighlight(rhythmSequence, 20, 400);
     ofDrawBitmapStringHighlight(rhythmBest, 20, 440);
}


//--------------------------------------------------------------
//Handler for sliders
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    
    if (e.target == tempoSlider) {
        tempo = e.value;
    }
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    cout << swarms[2].play << endl;
    cout << "Outside GUI: " << swarms[2].seventhPen << endl;
    

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == 's' && startSwarm == false) {
        startSwarm = true;
        lastCount = 0;
        currentCount = 0;
    } else if (key == 's' && startSwarm == true) {
        startSwarm = false;
    }

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
