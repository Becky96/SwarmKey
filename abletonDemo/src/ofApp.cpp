#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
    //Assigning correct path for ofxDatGui addon to assets folder.
    gui->setAssetPath("");

    //ofSetVerticalSync(true);
    
    //ofBackground(2, 18, 23);
    ofBackground(2, 20, 26);
    
   // ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    ////////////////////////////////////////////////////////////////
    //MIDI PORT SETUP
    //Setting up MIDI port for swarms
    ////////////////////////////////////////////////////////////////
    for (int i = 1; i < SWARM_NUM+1; i++) {
        
        swarms[i].openVirtualPort("Swarm");
        swarms[i].setup(i);
        
    }
    
    
    
    ////////////////////////////////////////////////////////////////
    //INDIVIDUAL SWARM INTERFACES
    ////////////////////////////////////////////////////////////////
    
    //Set up of individual interfaces for swarms.
    left = new SwarmGUI(1, 50, 230, &swarms[1], "Left Swarm");
    right = new SwarmGUI(2, 400, 230, &swarms[2], "Right Swarm");
    
    
    //Set up of individual interfaces for swarms.
    left->setupInterface();
    right->setupInterface();
    
    int x = 160;
    int y = 82;
    
    
    ////////////////////////////////////////////////////////////////
    //GLOBAL USER INTERFACE
    ////////////////////////////////////////////////////////////////
    
    //Toggle to start/stop playing swarms
    playSwarmsToggle = new ofxDatGuiToggle("Play Swarms", false);
    playSwarmsToggle->onToggleEvent(this, &ofApp::onToggleEvent);
    playSwarmsToggle->setPosition(x, y);
    playSwarmsToggle->setChecked(false);
    playSwarmsToggle->setWidth(UIWidth);
    globalSwarmComponents.push_back(playSwarmsToggle);
    x+=playSwarmsToggle->getWidth()+50;
    
    //Swarm global controls (controlling both swarms)
    tempoSlider = new ofxDatGuiSlider(tempoInt.set("Tempo", 240, 1, 480));
    tempoSlider->setPosition(x, y);
    tempoSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    globalSwarmComponents.push_back(tempoSlider);
    x+=tempoSlider->getWidth()+50;
    
    keyTypes = new ofxDatGuiDropdown("Select key type", types);
    keyTypes->setPosition(x, y);
    keyTypes->onDropdownEvent(this, &ofApp::onDropdownEvent);
    keyTypes->select(0);                                                        //Automatically select 'Major' scale as default when program is launched.
    keyTypes->getChildAt(0)->setBackgroundColor(ofColor(25, 47, 55));
    globalSwarmComponents.push_back(keyTypes);
    x+=keyTypes->getWidth()+50;
    keyTypes->getChildAt(1)->setBackgroundColor(ofColor(25, 47, 55));
    
    key = new ofxDatGuiDropdown("Select key", options);
    key->setPosition(x, y);
    key->onDropdownEvent(this, &ofApp::onDropdownEvent);
    key->select(0);
    for (int i = 0; i < 12; i++) {
        key->getChildAt(i)->setBackgroundColor(ofColor(25, 47, 55));
    }
    globalSwarmComponents.push_back(key);
    x+=key->getWidth();
    
    
    for (int i = 0; i < globalSwarmComponents.size(); i++) {
        globalSwarmComponents[i]->setBackgroundColor(ofColor(25, 47, 55));
    }
    phraseUI = new PhraseUI();
    phraseUI->setupPhraseUI();
    phraseUI->calculatePhraseKey(1, 60);
    
    

    sectionFont.load("Verdana.ttf", 12);
    infoFont.load("Verdana.ttf", 10);
    
    //Fonts for starting screen loaded at different sizes
    titleFont.load("Verdana.ttf", 96);
    smallFont.load("Verdana.ttf", 12);
    
    titleFont.setLetterSpacing(1.2);
    smallFont.setLetterSpacing(1.5);

    sampleRate = 44100;
    bufferSize = 1025;
    
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2,0, this, maxiSettings::sampleRate, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){

    //Updating global swarm user interface components
    for (int i = 0; i < globalSwarmComponents.size(); i++) {
        globalSwarmComponents[i]->update();
    }
   
    
    //Updating individual swarm user interface components
    left->updateInterface();
    right->updateInterface();

    phraseUI->updatePhraseUI();
}


void ofApp::playCurrentPhrase() {
    
    
        
        int phraseHits[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        
        for (int i = 0; i < bufferSize; i++){
            
            currentCount = (int)timer.phasor(tempo);
            
            if (playHeadPhrase == 16) {
                phraseUI->playPhraseBool = false;
                playHeadPhrase = 0;
            }
            
            if (lastCount != currentCount) {
                
                        if (phraseHits[playHeadPhrase% 16] == 1) {
                            
                            
                            phraseUI->currentPhrase[playHeadPhrase%16];
                            
                                swarms[1].midiOut.sendNoteOn(swarms[1].channel, phraseUI->currentPhrase[playHeadPhrase%16], 80);
                            
                                                                                                        
                        }
                
                        swarms[1].midiOut.sendNoteOff(swarms[1].channel, phraseUI->currentPhrase[playHeadPhrase%16]);
                
                            
                
                
                playHeadPhrase++;
                lastCount = 0;
                
                
            }
            
    }
}




void ofApp::sendMIDI() {
    
    //If global 'playSwarms' has been toggled to on, startSwarm becomes true and so begins the PSO
    //algorithmic process and the sending of MIDI messages to Ableton.
    if (startSwarm) {
        
        //Each swarm has an individual 'readyToPlay' boolean that is toggled on whenever the
        //global 'playSwarms' toggle is turned on or it's individual 'playSwarm' toggle
        //is turned on. This readyToPlay boolean ensures that the swarms will play in time with
        //each other, by resetting the individual notePlayheads to 0 and changeRhythmInt to 0.
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
                
                
                //Once the playHead has a modulo value of 0, changeRhythm becomes true
                //to indicate to perform the PSO algorithmic process on the rhythm
                //candidate solutions.
                if (playHead % 16 == 0) {
                    
                    changeRhythm = true;
                    changeRhythmInt++;
                }
                
                //Sending MIDI messages for swarm 1 if boolean 'play' is true.
                if (swarms[1].play == true) {
                    
                    float valR1, valR2;
                    
                    if (swarms[1].playFinalNote == false) {
                        if (swarms[1].bestRhythm.hits[playHead% 16] == 1) {
                            
                            
                            //Send MIDI on messages
                            if (playHead % 16 == 0) {
                                swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%16]], swarms[1].bestParticleSwarmVelocity);
                            } else {
                                swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%16]], swarms[1].bestParticleSwarmVelocity-20);
                                
                            }
                            
                            
                            if (swarms[1].chordPotential > 5) {
                                float r = ofRandom(100);
                                
                                if (r < swarms[1].chordPotential) {
                                    swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%16]+2], swarms[1].bestParticleSwarmVelocity-20);
                                }
                                
                                valR1 = r;
                            }
                            
                            if (swarms[1].chordPotential > 50) {
                                float r = ofRandom(100);
                                
                                if (r < swarms[1].chordPotential) {
                                    swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%16]+4], swarms[1].bestParticleSwarmVelocity-20);
                                }
                                valR2 = r;
                            }
                            
                            //Send MIDI off messages
                            swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%16]]);
                            
                            if (swarms[1].chordPotential > 5) {

                                if (valR1 < swarms[1].chordPotential) {
                                    swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%16]+2]);
                                }
                            }
                            
                            if (swarms[1].chordPotential > 50) {
                                if (valR2 < swarms[2].chordPotential) {
                                    swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%16]+4]);
                                }
                            }
                            
                            lastNotePlayheadLeft = swarms[1].notePlayhead;
                            swarms[1].notePlayhead++;
                            
                            if (lastNotePlayheadLeft != swarms[1].notePlayhead) {
                                calculateChordLeft = true;
                            }
                            noteChangeLeft = true;
                            
                        }
                        
                    //When
                    } else if (swarms[1].readyToPlay == false) {
                        
                        if (swarms[1].bestRhythm.hits[playHead% 16] == 1) {
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
                                swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]], swarms[2].bestParticleSwarmVelocity);
                            } else {
                                swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]], swarms[2].bestParticleSwarmVelocity-20);
                                
                            }
                            
                            
                            if (swarms[2].chordPotential > 5) {
                                float r = ofRandom(100);
                                
                                if (r < swarms[2].chordPotential) {
                                    swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]+2], swarms[2].bestParticleSwarmVelocity-20);
                                }
                                val1R = r;
                            }
                            
                            if (swarms[2].chordPotential > 50) {
                                float r = ofRandom(100);
                                if (r < swarms[2].chordPotential) {
                                    swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]+4], swarms[2].bestParticleSwarmVelocity-20);
                                }
                                val2R = r;
                            }
                            
                            
                            //Send MIDI off messages
                            swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]]);
                            
                            if (swarms[2].chordPotential > 5) {
                                
                                if (val1R < swarms[2].chordPotential) {
                                    swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]+2]);
                                }
                            }
                            
                            if (swarms[2].chordPotential > 50) {
                                
                                if (val2R < swarms[2].chordPotential) {
                                    swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]+4]);
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
                        if (swarms[2].bestRhythm.hits[playHead% 16] == 1) {
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
   
}
//--------------------------------------------------------------
void ofApp::draw(){
    

    
    //if (welcomePage == true) {
        
//       // ofNoFill();
//        ofSetColor(255);
//        //ofDrawRectangle(250, 300, ofGetWidth()-500, ofGetHeight()-600);
//        ofFill();
//        ofSetColor(255);
//        ofDrawRectangle(250, 300, ofGetWidth()-500, ofGetHeight()-600);
//        
//        ofSetColor(0);
//        titleFont.drawString("Swarm Key", 420, ofGetHeight()/2.);
//
//        smallFont.drawString("Press to continue", 400, ofGetHeight()/2.+100);
//        
   // } else {
    
    displayCorrectInfo();
    
    ofNoFill();
    ofSetColor(255);
    ofDrawRectangle(25, 175, 700, 600);
    ofDrawRectangle(25, 800, 1550, 300);
    ofDrawRectangle(25, 25, 1550, 125);
    ofDrawRectangle(775, 175, 800, 600);
    
    
    ofFill();
    ofSetColor(255);
    sectionFont.drawString("INDEPENDENT SWARM CONTROLS", 225, 205);
    sectionFont.drawString("INFORMATION AREA", 50, 830);
    sectionFont.drawString("GLOBAL SWARM CONTROLS", 675, 55);
    sectionFont.drawString("PHRASE CONTROLS", 1100, 205);

    
    ofFill();
    ofSetColor(91, 125, 123, 20);
    
    ofDrawRectangle(25, 175, 700, 600);         //Independent swarm
    ofDrawRectangle(25, 800, 1550, 300);        //Text area box
    ofDrawRectangle(25, 25, 1550, 125);         //Global swarm box
    ofDrawRectangle(775, 175, 800, 600);        //Phrase box
    
    ofNoFill();
    
    
    sendMIDI();
    if (phraseUI->playPhraseBool == true) {
        playCurrentPhrase();
    }
    
    if (phraseUI->phraseChanged == true) {
        
        cout << "REACHED" << endl;
        
        if (left->assignNewPhrase == true) {
            left->resetParticleIntervals();
            swarms[1].inputMotif(phraseUI->swarmNoteIndexes);
            left->currentMotifLabel->setLabel(phraseUI->phrases[phraseUI->selectedPhrase]->selectCell->getLabel() + " selected");

        }
        
        if (right->assignNewPhrase == true) {
            swarms[2].inputMotif(phraseUI->swarmNoteIndexes);
            right->resetParticleIntervals();
                       right->currentMotifLabel->setLabel(phraseUI->phrases[phraseUI->selectedPhrase]->selectCell->getLabel() + " selected");

        }
        
        phraseUI->phraseChanged = false;
    }
    

    
    
    //Drawing the individial swarm user interfaces
    left->drawInterface();
    right->drawInterface();
    
    //Outputting swarm parameter information
    left->displaySwarmParameters();
    right->displaySwarmParameters();
    
    
    
    //PHRASE UI
    phraseUI->displayPhraseList();
    phraseUI->displayPhraseUI();
    phraseUI->displaySelectedPhrase();
    
    
    //Checking if all booleans 'readyToPlay', 'play', and 'playFinalNote' of individual swarms
    //are false. If all are false, this indicates that startSwarm is now false to stop
    //all algorithmic functions.
    if (swarms[1].readyToPlay == false && swarms[2].readyToPlay == false && swarms[1].play == false && swarms[2].play == false && swarms[1].playFinalNote == false && swarms[2].playFinalNote == false) {
        startSwarm = false;
    }
  
    
    if (startSwarm == true) {
        
        //VELOCITY//
        //If swarm one has played a note, run the PSO algorithmic process to update the swarm's velocity.
        if (noteChangeLeft == true) {
            swarms[1].runVelocity();
            noteChangeLeft = false;
            
        //If swarm two has played a note, run the PSO algorithmic process to update the swarm's velocity.
        } if (noteChangeRight == true) {
            swarms[2].runVelocity();
            noteChangeRight = false;
            
        }

        
        //RHYTHM AND NOTE SEQUENCES//
        //Determine whether to change rhythm sequences
        if (changeRhythm == true && changeRhythmInt % 2 == 0) {

            
            if (swarms[2].play == true) {
    
                swarms[2].runRhythm();
                swarms[2].run(&swarms[1], playHead, swarms[2].notePlayhead, swarms[1].notePlayhead);
                
            }
            
            
            if (swarms[1].play == true) {
                
                swarms[1].runRhythm();
                swarms[1].run(&swarms[2], playHead, swarms[1].notePlayhead, swarms[2].notePlayhead);
                
            }

            changeRhythm = false;
        }
    }
    
    
    ofNoFill();
    //Drawing the global swarm user interfaces
    for (int i = 0; i < globalSwarmComponents.size(); i++) {
        globalSwarmComponents[i]->draw();
    }
    
    }

    

    
  //  }



//--------------------------------------------------------------

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e) {
    
    //Set swarm to play.
    //Response to playSwarms toggle.
    if (e.target == playSwarmsToggle && e.checked == true && startSwarm == false) {
        
        startSwarm = true;
        lastCount = 0;
        currentCount = 0;
        playHead = 0;
        changeRhythmInt = 0;
 
        
        swarms[1].readyToPlay = true;
        swarms[1].playFinalNote = false;
        swarms[1].notePlayhead = 0;
        
        swarms[2].readyToPlay = true;
        swarms[2].playFinalNote = false;
        swarms[2].notePlayhead = 0;
    }
    
    //Set swarm to stop playing.
    if (e.target == playSwarmsToggle && e.checked == false && startSwarm == true) {
        swarms[1].playFinalNote = true;
        swarms[2].playFinalNote = true;
        
  

        
    }
    
    
}

//--------------------------------------------------------------
//Handler for UI sliders
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    //Tempo is converted from bpm to hertz for the oscillator to time the swarm's playing.
    //1 hertz = 60 bpm
    if (e.target == tempoSlider) {
        tempo = (e.value/60);
    }
    
}

//--------------------------------------------------------------
//Handler for UI dropdown menus
void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e) {
    
    ////////////////////////
    //Key type dropdown menu
    //If statements select which key type has been selected from the dropdown, Major or Minor.
    //getKeyType is assigned as either 1 (Major), or 2 (Minor) and the swarms available notes are then recalculated to generate notes in the specific key.
    if (e.target->getLabel() == "Major") {
        getKeyType = 1;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);
    }
    
    if (e.target->getLabel() == "Minor") {
       getKeyType = 2;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

    }
    
    //////////////////////////
    //Key tonic dropdown menu
    //If statements select which tonic has been selected, and will assign 'keyNum' as the corresponding MIDI number of the tonic in octave 4.
    //The swarm's available notes are then recalculated to generate notes in the specific key.
    if (e.target->getLabel() == "C") {
        keyNum = 60;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

    }
    
    if (e.target->getLabel() == "C#") {
        keyNum = 61;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

    }
    
    if (e.target->getLabel() == "D") {
        keyNum = 62;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

    }
    
    if (e.target->getLabel() == "D#") {
        keyNum = 63;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

    }
    
    if (e.target->getLabel() == "E") {
        keyNum = 64;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

    }
    
    if (e.target->getLabel() == "F") {
        keyNum = 65;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

    }
    
    if (e.target->getLabel() == "F#") {
        keyNum = 66;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

    }
    
    if (e.target->getLabel() == "G") {
        keyNum = 67;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

    }
    
    
    if (e.target->getLabel() == "G#") {
        keyNum = 68;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

    }
    
    if (e.target->getLabel() == "A") {
        keyNum = 69;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

    }
    
    
    if (e.target->getLabel() == "A#") {
        keyNum = 70;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

    }
    
    
    if (e.target->getLabel() == "B") {
        keyNum = 71;
        swarms[1].calculateKey(keyNum, getKeyType);
        swarms[2].calculateKey(keyNum, getKeyType);
        phraseUI->calculatePhraseKey(getKeyType, keyNum);

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


    
    if (phraseUI->phrases.size() >= 1) {
        

        phraseUI->phrases[phraseUI->selectedPhrase]->checkGridPressed();
        phraseUI->phraseChanged = true;
        
        for (int j = 0; j < 16; j++) {
            phraseUI->swarmNoteIndexes[j] = (phraseUI->phrases[phraseUI->selectedPhrase]->phraseList[j]+17);
        }
    }
    

    
  
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    

    
    
    

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    
    cout << "X: " << ofGetMouseX() << endl;
    cout << "Y: " << ofGetMouseY() << endl;
    
    
    
    if (phraseUI->phrases.size() >= 1) {

        for (int i = 0; i < phraseUI->phrases[phraseUI->selectedPhrase]->phraseCells.size(); i++) {
            phraseUI->phrases[phraseUI->selectedPhrase]->phraseCells[i]->changedOnce = false;
        }
           phraseUI->phrases[phraseUI->selectedPhrase]->checkGridPressed();
      
    }
    
}


void ofApp::displayCorrectInfo() {
    
    ofSetColor(255);
    int x = ofGetMouseX();
    int y = ofGetMouseY();
    
    string info;
    
    //Global swarm controls info
    if (x >= 25 && x <= 25+1550 && y >= 25 && y <= 25+125) {
        info = "GLOBAL SWARM CONTROLS : These controls affect both swarms when altered.";
    }
    
    //Play swarms info
    if (x >= 160 && x <= (160+UIWidth) && y >= 82 && y<= (82+playSwarmsToggle->getHeight())) {
        
        info = "PLAY SWARMS : Use this button to start and stop playing both swarms at the same time. Make a phrase in the Phrase Area for the swarms to use as targets before playing the swarms.";
        
    }
    
    //Tempo info
    if (x >= 510 && x <= (510+UIWidth) && y >= 82 && y<= (82+playSwarmsToggle->getHeight())) {
        
        info = "TEMPO : This slider alters the tempo of the composition.";
        
    }
    
    //Key type info
    if (x >= 830 && x <= (830+UIWidth) && y >= 82 && y<= (82+playSwarmsToggle->getHeight())) {
        
        info = "KEY TYPE : Use this menu to alter the key type you want the swarms to compose in.";
    }
    
    //Key tonic info
    if (x >= 1150 && x <= (1150+UIWidth) && y >= 82 && y<= (82+playSwarmsToggle->getHeight())) {
        
        info = "KEY TONIC : Use this menu to alter the tonic of the key you want the swarms to compose in.";
    }
    
    
    //INDEPENDENT SWARM CONTROLS
    
    if (x >= 25 && x <= (25+700) && y >= 175 && y<= (175+600)) {
        
        info = "INDEPENDENT SWARM CONTROLS : These controls alter the individual swarms, with the left block altering the Left swarm and the right block altering the Right swarm.";
    }
    
    
    //Swarm Labels
    //Left swarm
    if (x >= 50 && x <= 50+300 && y >= 230 && y <= 256) {
        
        info = "LEFT SWARM : User interface panel for the Left swarmÕs composition controls.";
    }
    
    //Right swarm
    if (x >= 400 && x <= 700 && y >= 230 && y <= 256) {
        
        info = "RIGHT SWARM : User interface panel for the Right swarmÕs composition controls.";
    }
    
    //Select Phrase
    if (((x >= 50 && x <= 350) || (x >= 400 && x<= 700)) && (y >= 256 && y<= 282)) {
            info = "SELECT PHRASE FROM LIST : Turn this toggle on, the circle will become highlighted when it is on, and press the phrase you wish to use as the input phrase for this swarm. Then press the toggle to deactivate this mechanism, \nand repeat this process whenever you would like to change the selected input phrase.";
    }
    
    //Current Phrase Label
    if (((x >= 50 && x <= 350) || (x >= 400 && x<= 700)) && (y >= 282 && y<= 308)) {
        info = "CURRENT PHRASE : This is the current phrase that the swarm is using as its target.";
    }
    
    //Phrase Distance
    if (((x >= 50 && x <= 350) || (x >= 400 && x<= 700)) && (y >= 308 && y<= 334)) {
        info = "PHRASE DISTANCE : This slider controls whether you want the swarm to locate the phrase exactly as you have entered it, or if you would like the swarm to search for variations of the selected phrase. A value of 0 means \nthat the swarm will try locate the exact phrase, whereas a value of 100 means that the swarm will find phrases largely dissimilar to the original input phrase. When the PHRASE DISTANCE value is larger than 0, the interval likelihoods that you have controlled will come into play, as the swarm will find variations of the phrase utilising the intervals you have chosen.";
    }
    
    //Search Intensity
    if (((x >= 50 && x <= 350) || (x >= 400 && x<= 700)) && (y >= 334 && y<= 360)) {
        info = "SEARCH INTENSITY : This slider controls the intensity of the swarmÕs search towards the inputted phrase. If the preferred phrase distance is 0, and you would like the swarm to reach the exact inputted phrase quickly, \na higher search intensity means that swarm is more likely to find the exact phrase in a quicker time.";
    }
    
    //Rhythm Speed
    if (((x >= 50 && x <= 350) || (x >= 400 && x<= 700)) && (y >= 380 && y<= 406)) {
      info = "RHYTHM SPEED : This slider controls the ÒspeedÓ of the rhythm. It is not equivalent to the global tempo slider, but instead decides how many notes to divide the bar of 4 beats into. A value of 1 means that the bar \ncontains 1 note, a semibreve or whole note. A value of 16 means that the bar is divided into 16 beats, which equates to 16 semiquavers per bar. Values in between 1 and 16 will create different rhythms composed of beats will values: 4, 2, 1, 1/2, and 1/4.";
    }
    
    //Velocity Level
    if (((x >= 50 && x <= 350) || (x >= 400 && x<= 700)) && (y >= 406 && y<= 432)) {
        info = "VELOCITY LEVEL : This slider alters the velocity, (the strength of the notes pressed), of the swarm. A higher number equals a higher, or stronger, velocity.";
    }
    
    //Octave Level
    if (((x >= 50 && x <= 350) || (x >= 400 && x<= 700)) && (y >= 432 && y<= 458)) {
        info = "OCTAVE LEVEL :  This slider determines what octave you would like the swarm to strive towards. At default this is placed at middle C (octave 4). When you change the octave level you want, the phrase selected \nfor the swarm to use as a target is transposed to octave level you have set.";
    }
    
    //Chord Potential
    if (((x >= 50 && x <= 350) || (x >= 400 && x<= 700)) && (y >= 458 && y<= 484)) {
        info = "CHORD POTENTIAL : This slider determines the likelihood of the swarm playing a chord. A value of 100 means the swarm is 100% likely to play a chord. ";
    }
    
    //Intervals
    
    if (((x >= 50 && x <= 350) || (x >= 400 && x<= 700)) && (y >= 504 && y<= 738)) {
        info = "INTERVALS : These sliders determine what intervals you would like your swarm to target if the PHRASE DISTANCE is more than 0. A higher slider value means that the swarm will show preference to these intervals.";
    }
    
    //PHRASE AREA
    if (x >= 775 && x <= 1575 && y >= 175 && y <= 775) {
        info = "PHRASE CONTROLS : This area allows you to create, edit, delete, and play your own phrases, which you can then set the swarm to use as ÔtargetsÕ in your composition. When you make a phrase that you wish to be \nused by the swarms, select the ÔSELECT PHRASE FROM LISTÕ option in the Independent Swarm Controls area, and click on the phrase you would like to use in the Phrase List.";
    }
    
    //Add phrase
    if (x >= 800 && x <= 1025 && y >= 230 && y <= 256) {
        info = "ADD NEW PHRASE : Click to create a new phrase.";
    }
    
    //Play phrase
    if (x >= 1062 && x <= 1287 && y >= 230 && y <= 256) {
        info = "PLAY SELECTED PHRASE : Click to hear what the currently selected phrase sounds likes.";
    }
    
    //Delete phrase
    if (x >= 1324 && x <= 1549 && y >= 230 && y <= 256) {
        info = "DELETE SELECTED PHRASE : Click to delete the currently selected phrase.";
    }
    
    //Phrase List
    if (phraseUI->phrases.size() > 0) {
        if (x >= 800 && x <= 1025 && y >= 300 && y <= 326) {
            info = "PHRASE LIST : List of currently available phrases.";
        }
        
        
        //Grid area
        if (x >= 1150 && x <= 1550 && y >= 300 && y <= 675) {
            info = "PHRASE EDITOR : Press the grid to create notes for your phrase. The rows of the grid square represent the notes, and itÕs column of the grid represents itÕs time position in the phrase. There can only be one note playing \nat a time. If you do not enter any note in a column, a note will be selected at random to fill this time space in the phrase when played or used by the swarms.";
        }
    }
    
    
    
    infoFont.drawString(info, 50, 870);
    
  
    
   
    
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
