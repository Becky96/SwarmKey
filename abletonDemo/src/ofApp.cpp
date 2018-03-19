#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    gui->setAssetPath("");

    ofSetVerticalSync(true);
    
    ofBackground(0);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    for (int i = 1; i < SWARM_NUM+1; i++) {
        
        swarms[i].openVirtualPort("Swarm");
        swarms[i].setup(i);
        
    }
    
    
    //Note and rhythm motif for swarm one
    int nMotif1[4] = {21, 23, 25, 23};
    int rMotif1[16] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
    swarms[1].inputMotif(nMotif1, rMotif1);
    
    
    //Note and rhythm motif for swarm two
    int nMotif2[4] = {28, 30, 28, 30};
    int rMotif2[16] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
    swarms[2].inputMotif(nMotif2, rMotif2);
    
    
    //Set up of interfaces for swarms.
    left = new SwarmGUI(1, 50, 200, &swarms[1]);
    right = new SwarmGUI(2, 350, 200, &swarms[2]);
    
    left->setupInterface();
    right->setupInterface();
    
    int x = 50;
    int y = 50;
    
    
    
    playSwarmsToggle = new ofxDatGuiToggle("Start playing both swarms", false);
    playSwarmsToggle->onToggleEvent(this, &ofApp::onToggleEvent);
    playSwarmsToggle->setPosition(x, y);
    playSwarmsToggle->setChecked(false);
    globalSwarmComponents.push_back(playSwarmsToggle);
    y+=playSwarmsToggle->getHeight();
    x+=100;
    
    keyTypes = new ofxDatGuiDropdown("Select key type", types);
    keyTypes->setPosition(x, y);
    keyTypes->onDropdownEvent(this, &ofApp::onDropdownEvent);
    globalSwarmComponents.push_back(keyTypes);
    y+=keyTypes->getHeight();
    x+=100;
    
    key = new ofxDatGuiDropdown("Select key", options);
    key->setPosition(x, y);
    key->onDropdownEvent(this, &ofApp::onDropdownEvent);
    globalSwarmComponents.push_back(key);
    y+=key->getHeight();
    x+=100;
    
    //Swarm global controls (controlling both swarms)
    tempoSlider = new ofxDatGuiSlider(tempoInt.set("Tempo", 240, 1, 480));
    tempoSlider->setPosition(x, y);
    tempoSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    globalSwarmComponents.push_back(tempoSlider);
    y+=tempoSlider->getHeight();
    
    
    
    
    

    pianoRoll.setup(700, 100);
    
    sampleRate = 44100;
    bufferSize = 1025;
    
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2,0, this, maxiSettings::sampleRate, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){


    for (int i = 0; i < globalSwarmComponents.size(); i++) {
    globalSwarmComponents[i]->update();
    }
   
    
    left->updateInterface();
    right->updateInterface();


}


void ofApp::audioRequested(float * output, int bufferSize, int nChannels){
    
    if (startSwarm) {
    
        if (swarms[1].readyToPlay == true && changeRhythmInt % 4 == 0) {
            swarms[1].play = true;
            swarms[1].readyToPlay = false;
            swarms[1].notePlayhead = 0;
            

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
            }
            
            //Determine whether to play first swarm
            if (swarms[1].play == true) {
                
                float valR1, valR2;
                if (swarms[1].playFinalNote == false) {
                if (swarms[1].bestRhythm.hits[playHead% 16] == 1) {
            
                    
                    //Send MIDI on messages
                    if (playHead % 16 == 0) {
                        swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]], swarms[1].bestParticleSwarmVelocity);
                    } else {
                        swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]], swarms[1].bestParticleSwarmVelocity-20);
                    
                    }
                    
                  
                    if (swarms[1].chordPotential > 5) {
                        float r = ofRandom(100);
                        
                        if (r < swarms[1].chordPotential) {
//                     swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].bestChord[0]], swarms[1].bestParticleSwarmVelocity-20);
                        swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]+3], swarms[1].bestParticleSwarmVelocity-20);
                        }
                        
                        valR1 = r;
                    }
                    
                    if (swarms[1].chordPotential > 50) {
                        float r = ofRandom(100);
                        
                        if (r < swarms[1].chordPotential) {
                        // swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].bestChord[1]], swarms[1].bestParticleSwarmVelocity-20);
                        swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]+5], swarms[1].bestParticleSwarmVelocity-20);
                        }
                        valR2 = r;
                    }
                    
                    //Send MIDI off messages
                    swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]]);

                    if (swarms[1].chordPotential > 5) {
                    //swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].bestChord[0]]);
                        if (valR1 < swarms[1].chordPotential) {
                        swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]+3]);
                        }
                    }
                    
                    if (swarms[1].chordPotential > 50) {
                        if (valR2 < swarms[2].chordPotential) {
                        //swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].bestChord[1]]);
                        swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]+5]);
                        }
                    }
                    
                    lastNotePlayheadLeft = swarms[1].notePlayhead;
                    swarms[1].notePlayhead++;
                    
                    if (lastNotePlayheadLeft != swarms[1].notePlayhead) {
                        calculateChordLeft = true;
                    }
                    noteChangeLeft = true;

                }
                } else if (swarms[1].readyToPlay == false) {
                    cout << "REACHED " << endl;

                    if (swarms[1].bestRhythm.hits[playHead% 16] == 1) {
                        cout << "TRUE2 Swarm 1" << endl;
                        swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].tonic, swarms[1].bestParticleSwarmVelocity);
                        
                        swarms[1].playFinalNote = false;
                        swarms[1].play = false;
                        swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].tonic);
                        
                    }

   
                }
                
                
            }

            
            //Determine whether to play second swarm
            if (swarms[2].play == true) {
                float val1R, val2R;
                
                if (swarms[2].playFinalNote == false) {

                if (swarms[2].bestRhythm.hits[playHead% 16] == 1) {
                    
                    
                    //Send MIDI on messages
                    if (playHead % 16 == 0) {
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]], swarms[2].bestParticleSwarmVelocity);
                    } else {
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]], swarms[2].bestParticleSwarmVelocity-20);
                        
                    }
                    
                    
                    if (swarms[2].chordPotential > 5) {
                        float r = ofRandom(100);
                        
                        if (r < swarms[2].chordPotential) {
                        //swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].bestChord[0]], swarms[2].bestParticleSwarmVelocity-20);
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]+3], swarms[2].bestParticleSwarmVelocity-20);
                        }
                        val1R = r;
                    }
                    
                    if (swarms[2].chordPotential > 50) {
                        float r = ofRandom(100);
                        if (r < swarms[2].chordPotential) {
                        //swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].bestChord[1]], swarms[2].bestParticleSwarmVelocity-20);
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]+5], swarms[2].bestParticleSwarmVelocity-20);
                        }
                        val2R = r;
                    }
                    
                    
                    //Send MIDI off messages
                    swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]]);
                    
                    if (swarms[2].chordPotential > 5) {
                        
                        if (val1R < swarms[2].chordPotential) {
                      //  swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].bestChord[0]]);
                            swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]+3]);
                        }
                    }
                    
                    if (swarms[2].chordPotential > 50) {
                       // swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].bestChord[1]]);
                        
                        if (val2R < swarms[2].chordPotential) {
                            swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]+5]);
                        }

                        
                    }
                    
                    lastNotePlayheadRight = swarms[2].notePlayhead;
                    swarms[2].notePlayhead++;
                    
                    if (lastNotePlayheadRight != swarms[2].notePlayhead) {
                        calculateChordRight = true;
                    }
                    noteChangeRight = true;
                    
                }
                
                
                } else if (swarms[2].readyToPlay == false) {
                    cout << "REACHED " << endl;
                    if (swarms[2].bestRhythm.hits[playHead% 16] == 1) {
                        cout << "TRUE2 Swarm 2" << endl;
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].tonic, swarms[2].bestParticleSwarmVelocity);
                        
                        swarms[2].playFinalNote = false;
                        swarms[2].play = false;
                        
                          swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].tonic);
                    }
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
    
    
    //pianoRoll.displayRoll();
    
    for (int i = 0; i < globalSwarmComponents.size(); i++) {
        globalSwarmComponents[i]->draw();
    }
    
    
    left->drawInterface();
    right->drawInterface();
    
    if (swarms[1].readyToPlay == false && swarms[2].readyToPlay == false && swarms[1].play == false && swarms[2].play == false && swarms[1].playFinalNote == false && swarms[2].playFinalNote == false) {
        //cout << "OFF" << endl;
        startSwarm = false;
    }
  
    
    if (startSwarm == true) {
        
        if (noteChangeLeft == true) {
            
            swarms[1].runVelocity();
            noteChangeLeft = false;
            if (calculateChordLeft == true) {
                swarms[1].runChord(swarms[1].notePlayhead);
                calculateChordLeft = false;
            }
            
        } if (noteChangeRight == true) {
            swarms[2].runVelocity();
            noteChangeRight = false;
            if (calculateChordRight == true) {
                swarms[2].runChord(swarms[2].notePlayhead);
                calculateChordRight = false;
            }
            
        }

        //Determine whether to change rhythm sequences
        if (changeRhythm == true && changeRhythmInt % 2 == 0) {
            
            
            
            if (swarms[2].play == true) {
    

                swarms[2].runRhythm();
                
                swarms[2].run(&swarms[1], playHead, swarms[2].notePlayhead, swarms[1].notePlayhead);
                

            }
            
            
            if (swarms[1].play == true) {
                
                swarms[1].runRhythm();
                swarms[1].run(&swarms[2], playHead, swarms[1].notePlayhead, swarms[2].notePlayhead);
                
                //pianoRoll.resetCells(&swarms[1], &swarms[2]);
                
            }

            
            changeRhythm = false;
        }
    }
    
/*
    swarms[1].display();

    ofPushMatrix();
    ofTranslate(0, 200);
    swarms[2].display();
    ofPopMatrix();
    */
    
    left->displaySwarmParameters();
    right->displaySwarmParameters();

    
    }



//--------------------------------------------------------------

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e) {
    
    //Set swarm to play.
    if (e.target == playSwarmsToggle && e.checked == true && startSwarm == false) {
        
        cout << "Playing" << endl;
        startSwarm = true;
        lastCount = 0;
        currentCount = 0;
        playHead = 0;
        changeRhythmInt = 0;
        
        left->playingToggle->setChecked(true);
        right->playingToggle->setChecked(true);
        swarms[1].readyToPlay = true;
        swarms[1].playFinalNote = false;
        swarms[1].notePlayhead = 0;
        
        swarms[2].readyToPlay = true;
        swarms[2].playFinalNote = false;
        swarms[2].notePlayhead = 0;
    }
    
    //Set swarm to stop playing.
    if (e.target == playSwarmsToggle && e.checked == false && startSwarm == true) {
        //startSwarm = false;
        cout << startSwarm << endl;
        swarms[1].playFinalNote = true;
        swarms[2].playFinalNote = true;
        
        left->playingToggle->setChecked(false);
        right->playingToggle->setChecked(false);

        
    }
    
    
}

//--------------------------------------------------------------
//Handler for sliders
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    //Tempo is converted from bpm to hertz for the oscillator to time the program.
    //1 hertz = 60 bpm
    if (e.target == tempoSlider) {
        tempo = (e.value/60);
    }
    
}



void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e) {
    
    cout << options[e.child] << endl;
    
    if (types[e.child] == "Major") {
        getKeyType = 1;
    }
    
    if (types[e.child] == "Minor") {
        
        getKeyType = 2;
        
    }
    if (options[e.child] == "C") {
        keyNum = 60;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
    }
    
    if (options[e.child] == "C#") {
        keyNum = 61;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
    }
    
    if (options[e.child] == "D") {
        keyNum = 62;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
    }
    
    if (options[e.child] == "D#") {
        keyNum = 63;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
    }
    
    if (options[e.child] == "E") {
        keyNum = 64;

        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
    }
    
    if (options[e.child] == "F") {
        keyNum = 65;

        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
    }
    
    if (options[e.child] == "F#") {
        keyNum = 66;

        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
    }
    
    if (options[e.child] == "G") {
        keyNum = 67;

        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
    }
    
    
    if (options[e.child] == "G#") {
        keyNum = 68;

        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
    }
    
    if (options[e.child] == "A") {
        keyNum = 69;

        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
    }
    
    
    if (options[e.child] == "A#") {
        keyNum = 70;

        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
    }
    
    
    if (options[e.child] == "B") {
        keyNum = 71;

        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
    }
    
    
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
 

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
