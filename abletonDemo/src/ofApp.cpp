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
    
    
    //Note and rhythm motif for swarm one
    int nMotif1[4] = {24, 22, 17, 22};
    int rMotif1[16] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
    swarms[1].inputMotif(nMotif1, rMotif1);
    
    
    //Note and rhythm motif for swarm two
    int nMotif2[4] = {28, 30, 28, 30};
    int rMotif2[16] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
    swarms[2].inputMotif(nMotif2, rMotif2);
    
    
    //Set up of interfaces for swarms.
    left = new SwarmGUI(1, 1000, 200, &swarms[1]);
    right = new SwarmGUI(2, 1300, 200, &swarms[2]);
    
    left->setupInterface();
    right->setupInterface();
    
    int x = 400;
    int y = 600;
    
    //Swarm global controls (controlling both swarms)
    tempoSlider = new ofxDatGuiSlider(tempoInt.set("Tempo", 240, 1, 480));
    tempoSlider->setPosition(x, y);
    tempoSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    y+=tempoSlider->getHeight();
    
    
    
    
    
    key = new ofxDatGuiDropdown("Select key", options);
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
            }
            
            //Determine whether to play first swarm
            if (swarms[1].play == true) {
                
                
                if (swarms[1].bestRhythm.hits[playHead% 16] == 1) {
            
                    if (playHead % 16 == 0) {
                        swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]], swarms[1].bestParticleSwarmVelocity);
                    } else {
                        swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]], swarms[1].bestParticleSwarmVelocity-20);
                    
                    }
                    
                    
                    if (swarms[1].chordPotential == 1) {
                     swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].bestChord[0]], swarms[1].bestParticleSwarmVelocity-20);

                    }
                    
                    if (swarms[1].chordPotential == 2) {
                        
                         swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].bestChord[1]], swarms[1].bestParticleSwarmVelocity-20);
                        
                        
                    }
                    swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%4]]);

                    if (swarms[1].chordPotential == 1) {
                    swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].bestChord[0]]);
                    }
                    
                    if (swarms[1].chordPotential == 2) {
                        swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].bestChord[1]]);

                    }
                    
                    lastNotePlayheadLeft = swarms[1].notePlayhead;
                    swarms[1].notePlayhead++;
                    
                    if (lastNotePlayheadLeft != swarms[1].notePlayhead) {
                        calculateChordLeft = true;
                    }
                    noteChangeLeft = true;

                }
                
                
            }

            
            //Determine whether to play second swarm
            if (swarms[2].play == true) {
                
                
                if (swarms[2].bestRhythm.hits[playHead% 16] == 1) {
                    
                    if (playHead % 16 == 0) {
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]], swarms[2].bestParticleSwarmVelocity);
                    } else {
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]], swarms[2].bestParticleSwarmVelocity-20);
                        
                    }
                    
                    
                    if (swarms[2].chordPotential == 1) {
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].bestChord[0]], swarms[2].bestParticleSwarmVelocity-20);
                        
                    }
                    
                    if (swarms[2].chordPotential == 2) {
                        
                        swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].bestChord[1]], swarms[2].bestParticleSwarmVelocity-20);
                        
                        
                    }
                    swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%4]]);
                    
                    if (swarms[2].chordPotential == 1) {
                        swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].bestChord[0]]);
                    }
                    
                    if (swarms[2].chordPotential == 2) {
                        swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].bestChord[1]]);
                        
                    }
                    
                    lastNotePlayheadRight = swarms[2].notePlayhead;
                    swarms[2].notePlayhead++;
                    
                    if (lastNotePlayheadRight != swarms[2].notePlayhead) {
                        calculateChordRight = true;
                    }
                    noteChangeRight = true;
                    
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
    
                
                //if (swarms[2].bestRhythm.dimensionality != swarms[2].chosenDimension) {
                swarms[2].runRhythm();
               // }
                
                swarms[2].run(&swarms[1], playHead, swarms[2].notePlayhead, swarms[1].notePlayhead);
                
                //swarms[2].runChord(swarms[2].notePlayhead);
                
                //cout << "playing rhythm" << endl;
            }
            
            
            if (swarms[1].play == true) {
                
                cout << "Dimension: " << swarms[1].bestRhythm.dimensionality << endl;
                //if (swarms[1].bestRhythm->dimensionality != swarms[1].chosenDimension) {

                swarms[1].runRhythm();
                //}
                swarms[1].run(&swarms[2], playHead, swarms[1].notePlayhead, swarms[2].notePlayhead);
                
            }

            
            changeRhythm = false;
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
        tempo = (e.value/60);
    }
    
}



void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e) {
    
    cout << options[e.child] << endl;
    
    if (options[e.child] == "C") {
        swarms[1].calculateKey(60);
        swarms[2].calculateKey(60);
    }
    
    if (options[e.child] == "C#") {
        swarms[1].calculateKey(61);
        swarms[2].calculateKey(61);
    }
    
    if (options[e.child] == "D") {
        swarms[1].calculateKey(62);
        swarms[2].calculateKey(62);
    }
    
    if (options[e.child] == "D#") {
        swarms[1].calculateKey(63);
        swarms[2].calculateKey(63);
    }
    
    if (options[e.child] == "E") {
        swarms[1].calculateKey(64);
        swarms[2].calculateKey(64);
    }
    
    if (options[e.child] == "F") {
        swarms[1].calculateKey(65);
        swarms[2].calculateKey(65);
    }
    
    if (options[e.child] == "F#") {
        swarms[1].calculateKey(66);
        swarms[2].calculateKey(66);
    }
    
    if (options[e.child] == "G") {
        swarms[1].calculateKey(67);
        swarms[2].calculateKey(67);
    }
    
    
    if (options[e.child] == "G#") {
        swarms[1].calculateKey(68);
        swarms[2].calculateKey(68);
    }
    
    if (options[e.child] == "A") {
        swarms[1].calculateKey(69);
        swarms[2].calculateKey(69);
    }
    
    
    if (options[e.child] == "A#") {
        swarms[1].calculateKey(70);
        swarms[2].calculateKey(70);
    }
    
    
    if (options[e.child] == "B") {
        swarms[1].calculateKey(71);
        swarms[2].calculateKey(71);
    }
    
    
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
 
    cout << "Potential: " << swarms[1].chordPotential << endl;
    cout << "Best: " << endl;
    cout << swarms[1].bestChord[0] << endl;
    cout << swarms[1].bestChord[1] << endl;
    cout << swarms[1].bestChord[2] << endl;
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
