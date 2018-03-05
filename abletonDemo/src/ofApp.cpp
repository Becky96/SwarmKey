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
    
    left = new SwarmGUI(1, 1000, 200, &swarms[1]);
    right = new SwarmGUI(2, 1300, 200, &swarms[2]);
    
    left->setupInterface();
   right->setupInterface();
    
    int x = 400;
    int y = 600;
    
    tempoSlider = new ofxDatGuiSlider(tempoInt.set("Tempo", 8, 1, 12));
    tempoSlider->setPosition(x, y);
    tempoSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    y+=tempoSlider->getHeight();
    
    key = new ofxDatGuiDropdown("Select key");
    key->setPosition(x, y);
    key->onDropdownEvent(this, &ofApp::onDropdownEvent);
    
    
    sampleRate = 44100;
    bufferSize = 1025;
    
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2,0, this, maxiSettings::sampleRate, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){


    tempoSlider->update();
    key->update();
    
    left->updateInterface();
    right->updateInterface();


}


void ofApp::audioRequested(float * output, int bufferSize, int nChannels){
    
    if (startSwarm) {
    
        if (swarms[1].readyToPlay == true && changeRhythmInt % 4 == 0) {
            //cout << "REACHED" << endl;
            swarms[1].play = true;
            swarms[1].readyToPlay = false;
            swarms[1].notePlayhead = 0;
            
           // cout << "Beginning note playhead: " << swarms[1].notePlayhead << endl;
        }
        
        if (swarms[2].readyToPlay == true && changeRhythmInt % 4 == 0) {
            swarms[2].play = true;
            swarms[2].readyToPlay = false;
            swarms[2].notePlayhead = 0;
        }
        

    for (int i = 0; i < bufferSize; i++){
        
        currentCount = (int)timer.phasor(tempo);
        
        if (lastCount != currentCount) {
            
            if (playHead % 16 == 0) {

                changeRhythm = true;
                changeRhythmInt++;
                //cout << "Rhythm counter: " << changeRhythmInt << endl;
                
            }
            
            //Determine whether to play first swarm
            if (swarms[1].play == true) {
                
                
                if (swarms[1].bestRhythm->hits[playHead% 16] == 1) {
            
                    if (playHead % 16 == 0) {
                        swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]], swarms[1].bestParticleSwarmVelocity);
                    } else {
                        swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]], swarms[1].bestParticleSwarmVelocity-20);
                    
                    }
                    
                    
                     swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[(swarms[1].best.indFreqs[swarms[1].notePlayhead%4])+2], swarms[1].bestParticleSwarmVelocity-20);
                    //cout << "Note playhead: " << swarms[1].notePlayhead%4 << endl;
                    //cout << "Note played: " << swarms[1].best.indFreqs[swarms[1].notePlayhead%4] << endl;

                    
                    swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]]);

                               swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[(swarms[1].best.indFreqs[swarms[1].notePlayhead%4])+2]);
                    
                    swarms[1].notePlayhead++;

                }
                
                
            }

            
            //Determine whether to play second swarm
            if (swarms[2].play == true) {
                
                
                if (swarms[2].bestRhythm->hits[playHead% 16] == 1) {
                    
                    if (playHead % 16 == 0) {
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]], swarms[2].bestParticleSwarmVelocity);
                    } else {
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]], swarms[2].bestParticleSwarmVelocity-20);
                        
                    }
                    
                    
                    //cout << swarms[2].best.indFreqs[swarms[2].notePlayhead%4] << endl;

                    swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]]);
                    
                    swarms[2].notePlayhead++;
                    
                }
                
                
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
    key->draw();
    
    left->drawInterface();
    right->drawInterface();
  
    
    if (startSwarm == true) {
        
        if (changeRhythmInt % 1 == 0) {
            
            swarms[1].runVelocity();

            swarms[2].runVelocity();

            
        }

        //Determine whether to change rhythm sequences
        if (changeRhythm == true && changeRhythmInt % 4 == 0) {
            
            
            
            if (swarms[2].play == true) {
    
                
                if (swarms[2].bestRhythm->dimensionality != swarms[2].chosenDimension) {
                swarms[2].runRhythm();
                }
                
                swarms[2].run(&swarms[1], playHead, swarms[2].notePlayhead, swarms[1].notePlayhead);
                
                //cout << "playing rhythm" << endl;
            }
            
            
            if (swarms[1].play == true) {
                
                cout << "Dimension: " << swarms[1].bestRhythm->dimensionality << endl;
                if (swarms[1].bestRhythm->dimensionality != swarms[1].chosenDimension) {

                swarms[1].runRhythm();
                }
                swarms[1].run(&swarms[2], playHead, swarms[1].notePlayhead, swarms[2].notePlayhead);
                
                
            }

            
            changeRhythm = false;
            //playHead = 0;
        }
    }
    

    swarms[1].display();

    ofPushMatrix();
    ofTranslate(0, 200);
    swarms[2].display();
    ofPopMatrix();
    
    
    left->displaySwarmParameters();
    right->displaySwarmParameters();

    
    }


//--------------------------------------------------------------
//Handler for sliders
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    
    if (e.target == tempoSlider) {
        tempo = e.value;
    }
    
}



void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e) {
    
    
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    cout << swarms[2].play << endl;
    cout << "Outside GUI: " << swarms[2].seventhPen << endl;
    

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == 's' && startSwarm == false) {
        cout << "Playing" << endl;
        startSwarm = true;
        lastCount = 0;
        currentCount = 0;
        playHead = 0;
        changeRhythmInt = 0;
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
