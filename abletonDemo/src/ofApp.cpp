#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
    /////MISCELLANEOUS/////
    gui->setAssetPath("");   //Assigning correct path for ofxDatGui addon to assets folder.
    ofBackground(2, 20, 26); //Background of program
    
    
    //MIDI PORT SETUP//
    //Setting up MIDI port for swarms to send to Ableton to trigger MIDI notes.
    for (int i = 1; i < SWARM_NUM+1; i++) {
        
        swarms[i].openVirtualPort("Swarm");
        swarms[i].setup(i);
        
    }
    
    //INDIVIDUAL SWARM INTERFACES//
    left = new SwarmGUI(1, 50, 230, &swarms[1], "Left Swarm");              //Left SwarmGUI controls swarms[1]
    right = new SwarmGUI(2, 400, 230, &swarms[2], "Right Swarm");           //Right SwarmGUI controls swarms[2]
    
    
    //Set up of individual interfaces for swarms.
    left->setupInterface();
    right->setupInterface();
    

    
    //GLOBAL INTERFACE//
    setupUI();
    
    //PHRASE UI//
    phraseUI = new PhraseUI();
    phraseUI->setupPhraseUI();
    phraseUI->calculatePhraseKey(1, 60);            //Calculate phrase input/output based on the current key
    
    
    //FONTS//
    loadFonts();                                    //Loading fonts required for program
    

    
    //AUDIO//
    //Sample and buffer size required for outputting sound and using ofxMaxim oscillator for timing.
    sampleRate = 44100;
    bufferSize = 1025;
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2,0, this, maxiSettings::sampleRate, bufferSize, 4);
}

//--------------------------------------------------------------

void ofApp::loadFonts() {
    
    //Fonts for main page
    sectionFont.load("Verdana.ttf", 12);            //Font for section titles
    infoFont.load("Verdana.ttf", 10);               //Font for information area text
    
    //Fonts for starting screen loaded at different sizes
    titleFont.load("Verdana.ttf", 96);
    smallFont.load("Verdana.ttf", 12);
    
    //Setting letter spacing
    titleFont.setLetterSpacing(1.2);
    smallFont.setLetterSpacing(1.5);
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
//--------------------------------------------------------------

void ofApp::setupUI() {
    
    //GLOBAL USER INTERFACE//
    //Global UI consists of controls that will affect both swarms simultaneously
    //(Playing, tempo, key type and tonic
    //Initial x and y starting coordinates of global UI
    int x = 160;
    int y = 82;
    
    
    //Toggle to start/stop playing swarms
    playSwarmsToggle = new ofxDatGuiToggle("Play Swarms", false);
    playSwarmsToggle->onToggleEvent(this, &ofApp::onToggleEvent);
    playSwarmsToggle->setChecked(false);                            //Set toggle to false when loaded as default
    globalSwarmComponents.push_back(playSwarmsToggle);
    
    //Swarm global controls (controlling both swarms)
    tempoSlider = new ofxDatGuiSlider(tempoInt.set("Tempo", 240, 1, 480));
    tempoSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    globalSwarmComponents.push_back(tempoSlider);
    
    keyTypes = new ofxDatGuiDropdown("Select key type", types);
    keyTypes->onDropdownEvent(this, &ofApp::onDropdownEvent);
    keyTypes->select(0);                                     //Automatically select 'Major' scale as default when program is launched.
    //Set colour of dropdown options to the same as other UI options
    keyTypes->getChildAt(0)->setBackgroundColor(ofColor(25, 47, 55));
    keyTypes->getChildAt(1)->setBackgroundColor(ofColor(25, 47, 55));
    globalSwarmComponents.push_back(keyTypes);              //Add to globalSwarmComponents vector

    
    key = new ofxDatGuiDropdown("Select key", options);
    key->onDropdownEvent(this, &ofApp::onDropdownEvent);
    key->select(0);
    for (int i = 0; i < 12; i++) {
        key->getChildAt(i)->setBackgroundColor(ofColor(25, 47, 55));
    }
    globalSwarmComponents.push_back(key);                   //Add to globalSwarmComponents vector

    
    
    //Set all UI colours to same colour and set position, incrememting 'x' by height of UI width to create horizontal layout
    for (int i = 0; i < globalSwarmComponents.size(); i++) {
        globalSwarmComponents[i]->setBackgroundColor(ofColor(25, 47, 55));
        globalSwarmComponents[i]->setPosition(x, y);
        x+=globalSwarmComponents[i]->getWidth()+50;
    }
    
    playSwarmsToggle->setBackgroundColors(ofColor(255), ofColor(180, 235, 255), ofColor(180, 235, 255));
    playSwarmsToggle->setLabelColor(ofColor(25, 47, 55));
    playSwarmsToggle->setLabelAlignment(ofxDatGuiAlignment::CENTER);

}

//--------------------------------------------------------------
/*Function playCurrentPhrase() is called when phraseUI object's 'Play Selected Phrase'. The MIDI notes of
 the currently selected grid are sent using Swarm 1's port and channel to Ableton to play the phrase so 
 that the user is able to understand exactly what their phrase sounds like. 
 The rhythm of the phrase is played simply as 16 1/4 notes. */
void ofApp::playCurrentPhrase() {
    
    
        int phraseHits[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};        //Rhythm for playing selected phrase is 16 1/4 notes.
        
        for (int i = 0; i < bufferSize; i++){
            
            /*The currentCount and lastCount variables are used to time the output of MIDI messages to Ableton.
            The timer oscillator uses a a phasor, acting as a continuous linear ramp producing a floating point number between 0.0 and 1.0. The frequency alters the speed at which the phasor ramps from 0 to 1, and so the faster the tempo/frequency of the phasor, the faster the oscillator counts through from 0 to 1 and thus increase the speed of the sending of MIDI messages to Ableton. 
             As currentCount and lastCount are both integers, there values are only concerned with when the phasor reaches 0 and 1. Whenever lastCount does not equal currentCount, this equates to a time step having occurred and a MIDI message should be sent to Ableton if the current rhythm hit specifies so.*/
            currentCount = (int)timer.phasor(tempo);
            
            
            //When playHeadPhrase equals 16, the phrase has now played all of its notes and the process should stop.
            if (playHeadPhrase == 16) {
                phraseUI->playPhraseBool = false;           //Stop playing by setting boolean to false and so 'if statement' is no longer true.
                playHeadPhrase = 0;                         //Reset playHeadPhrase for the next time the 'Play Selected Phrase' button is true and this process is repeated.
            }
            
            
            /*When lastCount is not equal to currentCount, this signals a change in time step, and so if
             the current phraseHit is equal to 1 (signalling a hit in the Rhythm) then output the current note to be played determined by the playHeadPhraseVariable*/
            if (lastCount != currentCount) {
                
                        if (phraseHits[playHeadPhrase% 16] == 1) {
                            
                                //Output MIDI message to Ableton to play current note in Phrase sequence, at velocity 80
                                swarms[1].midiOut.sendNoteOn(swarms[1].channel, phraseUI->currentPhrase[playHeadPhrase%16], 80);
                            
                                                                                                        
                        }
                
                        //Output MIDI message to Ableton to turn current note in Phrase sequence off.
                        swarms[1].midiOut.sendNoteOff(swarms[1].channel, phraseUI->currentPhrase[playHeadPhrase%16]);
                
                            
                
                
                playHeadPhrase++;               //Increment playHeadPhrase by 1 so that the next MIDI note in the sequence will be played.
                lastCount = 0;                  //Reset lastCount to 0 for the next iteration.
                
                
            }
            
    }
}


//--------------------------------------------------------------

void ofApp::sendMIDI() {
    
    //If global 'playSwarms' has been toggled to on, startSwarm becomes true and so begins the PSO
    //algorithmic process and the sending of MIDI messages of the real-time swarm composition to Ableton is started.
    if (startSwarm) {
        
        //Each swarm has an individual 'readyToPlay' boolean that is toggled on whenever the
        //global 'playSwarms' toggle is turned on or it's individual 'playSwarm' toggle
        //is turned on. This readyToPlay boolean ensures that the swarms will play in time with
        //each other as the if statements will only execute when the changeRhythmInt has is a multiple of 4.
        //When the if statements are executed, readyToPlay becomes false and play becomes true and so the MIDI messages will begin sending within this function.
        if (swarms[1].readyToPlay == true && changeRhythmInt % 4 == 0) {
            swarms[1].play = true;
            swarms[1].readyToPlay = false;
            swarms[1].notePlayhead = 0;                 //Reset note playhead to 0
            
            
        }
        
        if (swarms[2].readyToPlay == true && changeRhythmInt % 4 == 0) {
            swarms[2].play = true;
            swarms[2].readyToPlay = false;
            swarms[2].notePlayhead = 0;                 //Reset note playhead to 0
        }
        
        
        
        for (int i = 0; i < bufferSize; i++){
            
            
            /*The oscillator timer, currentCount and lastCount work in the same manner as described in the previous function, 'playCurrentPhrase()'. The frequency of the phasor oscillator will determine it's speed to linearly ramp from 0 to 1, and as a result the speed of the MIDI message output to Ableton.*/
            currentCount = (int)timer.phasor(tempo);
            
            if (lastCount != currentCount) {
                
                
                //Once the playHead has a modulo value of 0, changeRhythm becomes true
                //to indicate to perform the PSO algorithmic process on the rhythm
                //candidate solutions.
                if (rhythmPlayHead % 16 == 0) {
                    
                    changeRhythm = true;
                    changeRhythmInt++;
                }
                
                //FUNCTIONALITY FOR SENDING SWARM 1 MIDI MESSAGES.
                //This process will only execute if playSwarmsToggle has been activated.
                if (swarms[1].play == true) {
                    
                    float valR1, valR2;
                    
                    
                    //playFinalNote is an individual boolean that each Swarm object holds, which will determine whether the user has deactivated the global playSwarmToggle. If it is false, it means that the toggle is currently active and the functionality will continue sending MIDI messages of the best-ranked note sequences within the best-ranked rhythm's timing.
                    if (swarms[1].playFinalNote == false) {
                        
                        
                        //If there is a current hit (represented as 1), in the best rhythm sequence, this meeans that a MIDI message will be sent to play the current note of the best-ranked note sequence.
                        if (swarms[1].bestRhythm.hits[rhythmPlayHead% 16] == 1) {
                            
                            //If rhythmPlayHead % 16 == 0, this indicates it is the first note of the bar/sequence. To give a 'human' element to the playing, this first note is played with a higher velocity than the following notes, to simulate the emphasis given in physical piano playing where the first note is stressed in order to establish a recurring rhythmic beat throughout the composition.
                            if (rhythmPlayHead % 16 == 0) {
                                swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%16]], swarms[1].bestParticleSwarmVelocity);
                                
                            //This else block establishes any other notes that are not the first beat of the bar as a slightly lower velocity.
                            } else {
                                swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%16]], swarms[1].bestParticleSwarmVelocity-20);
                                
                            }
                            
                            
                            //CHORD POTENTIAL FUNCTIONALITY
                            //The independent chordPotential value for the swarm is used as a 'likelihood' as to whether or not to play one or two simultaneous notes as the original current note sequence note is played. The larger the number, the higher the chance that 1 or 2 notes are played concurrently to create a chord.
                            
                            //If chordPotential is more than 5, this will establish to play one more note additionally at the same time as the original current note in the sequence.
                            if (swarms[1].chordPotential > 5) {
                                
                                //r is generated as the number to test the swarm's chordPotential against.
                                float r = ofRandom(100);
                                
                                //If r is smaller than the swarm's chordPotential, then a note with an interval of 3 against the current note to be played is directed to be played at the same time also.
                                if (r < swarms[1].chordPotential) {
                                    swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%16]+2], swarms[1].bestParticleSwarmVelocity-20);
                                }
                                
                                //The value of r is recorded in valR1 in order to determine whether a chord has been played and if so, to ensure that a MIDI message is sent to turn the additional note(s) off.
                                valR1 = r;
                            }
                            
                            
                            //If the chordPotential value for the swarm is higher than 50, this will establish whether or not to play a 2nd additional note at the same time as the current note in the sequence.
                            if (swarms[1].chordPotential > 50) {
                                
                                //r is used again as the number to test the swarm's chordPotential again.
                                float r = ofRandom(100);
                                
                                //If r is smaller, then a second message will be sent additionallty to play a note with an interval of 5 against the current note sequence's note.
                                if (r < swarms[1].chordPotential) {
                                    swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%16]+4], swarms[1].bestParticleSwarmVelocity-20);
                                }
                                
                                //The value of r is recorded in valR2 in order to ensure that a MIDI message will be sent to turn this additional note off.
                                valR2 = r;
                            }
                            
                            //Sending MIDI off message for the current note played in the note sequence.
                            swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[swarms[1].notePlayhead%16]]);
                            
                            
                            //This will measure whether there was an additional 1 or 2 notes played earlier and if so, to send a MIDI message to turn these corresponding notes off.
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
                            
                            
                            //
                            lastNotePlayheadLeft = swarms[1].notePlayhead;
                            swarms[1].notePlayhead++;
                            
                            if (lastNotePlayheadLeft != swarms[1].notePlayhead) {
                                calculateChordLeft = true;
                            }
                            noteChangeLeft = true;
                            
                        }
                        
                    //This determines whether the global 'playSwarmsToggle' has been deactivated, and if so, a final note will be played by the swarm as the tonic of the current key.
                    } else if (swarms[1].readyToPlay == false) {
                        
                        //Play the final note when the swarm has a registered hit to play in its rhythm sequence.
                        if (swarms[1].bestRhythm.hits[rhythmPlayHead% 16] == 1) {
                            swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].tonic, swarms[1].bestParticleSwarmVelocity);
                            
                            //Setting all swarm individual booleans to false to signal stopping any MIDI messages sending or algorithmic processes continuing.
                            swarms[1].playFinalNote = false;
                            swarms[1].play = false;
                            
                            //Sending MIDI message to turn off the final note sent of the tonic note.
                            swarms[1].midiOut.sendNoteOff(swarms[1].channel, swarms[1].tonic);
                            
                        }
                        
                        
                    }
                    
                    
                }
                
                
                //The functionality below is identical to the previous functionality for Swarm 1 except for Swarm 2. To avoid repetition of comments and space, please refer to the identical code block above for more in-depth commenting.
                if (swarms[2].play == true) {
                    
                    float val1R, val2R;         //Values for recording whether one or two additional notes have been played.
                    
                    //If it has not been signalled the playSwarmToggle has been deactivated, then continue with the sending MIDI message process of Swarm 2.
                    if (swarms[2].playFinalNote == false) {
                        
                        
                        //If their is a current hit in the rhythm sequece.
                        if (swarms[2].bestRhythm.hits[rhythmPlayHead% 16] == 1) {
                            
                            
                            //Send MIDI on messages
                            if (rhythmPlayHead % 16 == 0) {
                                swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]], swarms[2].bestParticleSwarmVelocity);
                            } else {
                                swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]], swarms[2].bestParticleSwarmVelocity-20);
                                
                            }
                            
                            
                            //Check whether or not to play an additional note.
                            if (swarms[2].chordPotential > 5) {
                                float r = ofRandom(100);
                                
                                if (r < swarms[2].chordPotential) {
                                    swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]+2], swarms[2].bestParticleSwarmVelocity-20);
                                }
                                val1R = r;      //r is saved into val1R to determine whether to turn off additional note through MIDI message.
                            }
                            
                            
                            //Check whether or not to play a second additional note.
                            if (swarms[2].chordPotential > 50) {
                                float r = ofRandom(100);
                                if (r < swarms[2].chordPotential) {
                                    swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]+4], swarms[2].bestParticleSwarmVelocity-20);
                                }
                                val2R = r;      //r is saved into val2R to determine whether to turn off additional note through MIDI message.
                            }
                            
                            
                            //Turning off MIDI note of current note in note sequence
                            swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]]);
                            
                            
                            //Turning off additional note if it has been played.
                            if (swarms[2].chordPotential > 5) {
                                
                                if (val1R < swarms[2].chordPotential) {
                                    swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[swarms[2].notePlayhead%16]+2]);
                                }
                            }
                            
                            
                            //Turning off additional second note if it has been played.
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
                        
                    
                        //If playSwarmsToggle has been deactivated, Swarm 2 will output the tonic of the current key to signal the end of the playing.
                    } else if (swarms[2].readyToPlay == false) {
                        
                        //Output MIDI message when current rhythm sequence signals a hit.
                        if (swarms[2].bestRhythm.hits[rhythmPlayHead% 16] == 1) {
                            swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].tonic, swarms[2].bestParticleSwarmVelocity);
                            
                            //Setting all booleans to false to finish MIDI output and algorithmic processes.
                            swarms[2].playFinalNote = false;
                            swarms[2].play = false;
                            
                            //Send MIDI off message of final tonic note.
                            swarms[2].midiOut.sendNoteOff(swarms[2].channel, swarms[2].tonic);
                        }
                    }
                }
                
                
                //RhythmPlayHead is incremented to continue through the current best-ranked rhythm sequence.
                rhythmPlayHead++;
                
                //lastCount is set to 0 in order to compare against currentCount in the next iteration.
                lastCount = 0;
                
                
            }
            
        }
    }
   
}
//--------------------------------------------------------------
/*This function will display the introduction screen. It is run by default when the program is loaded, and when the screen is pressed, this function will now longer be run throughout the remainder of the program's running duration*/
void ofApp::displayIntroScreen() {
    
    
    //Display box area of introduction screen (outline)
    ofNoFill();
    ofSetColor(255);
    ofDrawRectangle(75, 75, ofGetWidth()- (75*2), ofGetHeight() - (75*2));
    
    //Display box area of introduction screen (coloured)
    ofFill();
    ofSetColor(91, 125, 123, 20);
    ofDrawRectangle(75, 75, ofGetWidth()- (75*2), ofGetHeight() - (75*2));
    
    
    //Text information for introduction screen
    ofSetColor(255);
    titleFont.drawString("Swarm Key", 350, ofGetHeight()/2-100);
    smallFont.drawString("Welcome to Swarm Key, an interactive generative music program. \nCreate phrases in the Phrase Control area, and then select these \nphrases to be used as targets for the Swarms in the Independent Swarm Control area.", 300, ofGetHeight()/2+100);
    smallFont.drawString("Press the screen to continue.", 300, ofGetHeight()/2+200);

}

//--------------------------------------------------------------
/*This function displays the lines that divide the program screen into areas of "Global Swarm Controls", "Independent Swarm Controls", "Phrase Area", and "Information Area".*/

void ofApp::displayAreaSegments() {
    
    displayInfoAreaText();                  //Display the information area text based upon the current position of the mouse.
    
    
    
    //Outline of rectangle areas.
    ofNoFill();
    ofSetColor(255);
    ofDrawRectangle(25, 175, 700, 600);
    ofDrawRectangle(25, 800, 1550, 300);
    ofDrawRectangle(25, 25, 1550, 125);
    ofDrawRectangle(775, 175, 800, 600);
    
    
    //Shape fill of rectangle areas.
    ofFill();
    ofSetColor(91, 125, 123, 20);
    ofDrawRectangle(25, 175, 700, 600);         //Independent swarm
    ofDrawRectangle(25, 800, 1550, 300);        //Text area box
    ofDrawRectangle(25, 25, 1550, 125);         //Global swarm box
    ofDrawRectangle(775, 175, 800, 600);        //Phrase box
    
    
    //Titles of function areas.
    ofFill();
    ofSetColor(255);
    sectionFont.drawString("INDEPENDENT SWARM CONTROLS", 225, 205);
    sectionFont.drawString("INFORMATION AREA", 50, 830);
    sectionFont.drawString("GLOBAL SWARM CONTROLS", 675, 55);
    sectionFont.drawString("PHRASE CONTROLS", 1100, 205);
    

    //Reset to ofNoFill for slider/UI appearences.
    ofNoFill();
    
    
    
}

//--------------------------------------------------------------
/*This function monitors whether or not a phrase has been deleted. If it has, if a swarm has currently used this deleted phrase as it's selected phrase, it needs to be deselected by the swarm. 
 If a phrase has been deleted, all other phrases will change their id and position to reorganise the structure of the phrase list. The swarms will then have to reselect the phrase previously selected and display it by the phrase's new id. */

void ofApp::checkPhraseDeleted() {
    
    //If a phrase is deleted, the phrases are relisted with new positions and different ids
    //so the swarms must change their id representation of the phrase also.
    if (phraseUI->resetPhraseIds == true) {
        
        //Individual booleans check whether or not the swarm had selected a phrase which has been deleted from the list. The for loop below loops through all phrases that have not been deleted, and if any phrase has a 'prevId' (previous id) that matches the swarm's currently selected phrase id, the swarm will then adjust it's selected phrase id to the phrase's new id. When this occurs, phraseIdChanged will become true for the specific swarm. After the for loop, if this individual boolean is still false, it indicates that the phrase which the Swarm had selected as its phrase has been deleted, and so it's label will be replaced with the instructions to select a new phrase.
        left->phraseIdChanged = false;
        right->phraseIdChanged = false;
        
        
        //Checking through all phrases that have not been deleted.
        for (int i = 0; i < phraseUI->phrases.size(); i++) {
            
            
            //If the currently checked phrase's previous id matches the swarm's selected phrase's id, replace the swarm's selected phrase's id with the phrase's new id. This process occurs as an if statement for both Swarm 1 and Swarm 2.
            if (phraseUI->phrases[i]->prevId == swarms[1].phraseId) {
                swarms[1].phraseId = phraseUI->phrases[i]->id;
                left->currentMotifLabel->setLabel(phraseUI->phrases[i]->selectCell->getLabel() + " selected");  //Replacing label with the phrase's new id.
                left->phraseIdChanged = true;       //PhraseIdChanged now becomes true to indicate that the phrsae that was deleted was not the phrase used by the swarm.
            }
            
            //Swarm 2 checking against phrase ids in the exact way as detailed above.
            if (phraseUI->phrases[i]->prevId == swarms[2].phraseId) {
                swarms[2].phraseId = phraseUI->phrases[i]->id;
                right->currentMotifLabel->setLabel(phraseUI->phrases[i]->selectCell->getLabel() + " selected"); //Replacing label with the phrase's new id.
                right->phraseIdChanged = true;      //PhraseIdChanged now becomes true to indicate that the phrsae that was deleted was not the phrase used by the swarm.
            }

        }
        
        
        //If boolean remains false, indicates it has been deleted for the specific swarm. Label is relabeled as instruction to select new phrase.
        //Left swarm phrase deleted check.
        if (left->phraseIdChanged == false) {
            left->currentMotifLabel->setLabel("Select new phrase");
        }
        
        //Right swarm phrase deleted check.
        if (right->phraseIdChanged == false) {
            right->currentMotifLabel->setLabel("Select new phrase");
        }
        
        
        //resetPhraseIds is set to false to indicate that the process of checking if any of the Swarm's selected phrase has been deleted, or if it needs to reevaluate the selected phrase's current id, has finished.
        phraseUI->resetPhraseIds = false;
    }

}

//--------------------------------------------------------------
/*This function determines whether a new Phrase has been selected in the Phrase List to be edited/changed/, or whether or not the grid of the currently selected Phrase has been altered (by pressing on it to create notes.
 If this occurs, two conditions are checked for. 
 1. If the 'assignNewPhrase' toggle has been activated for either Swarm 1 or Swarm 2, if a phrase has been selected then it must become the Swarm's new target/selected phrase. The functionality for this is repeated for both Swarm 1 and Swarm 2.
 2. If a grid of a phrase is altered to display/play new notes, then if either Swarm has this phrase as the target/selected Phrase, this phrase needs to be updated so that the note sequence the grid represented is updated as the target note sequence of the Swarm if it is selected.
 */

void ofApp::checkPhraseChanged() {
    
    
    //If a different phrase has been selected on the Phrase List, or if the grid of the currently selected phrase has been altered.
    if (phraseUI->phraseChanged == true) {
        

        //Swarm 1 checking.
        //if left->assignNewPhrase indicates whether or not it's 'assignNewPhrase' toggle has been highlighted, and so when a Phrase is selected from the Phrase List, it becomes the target phrase for the swarm.
        //If phraseUI->phrases[phraseUI->selectedPhrase]->id == swarms[1].phraseId is true, this signals that the grid of the currently selected phrase has been altered and that it is also the target phrase currently for the swarm.
        //If either conditions are true, the particle intervals are reset in order for the swarm to discover possible solutions to the new target phrase. The target phrase sequence is changed to match the newly selected target phrase sequence. The swarm's phraseId is set to match the phrase's id for future checking of these conditions, and it's label is set to match the name of the currently selected phrase id for user purposes.
        //All booleans are reset to false until these conditions are met again.
        //This process is repeated for the 'Swarm 2 checking' block.
        if (left->assignNewPhrase == true || phraseUI->phrases[phraseUI->selectedPhrase]->id == swarms[1].phraseId) {
            left->resetParticleIntervals();                         //Reset particle intervals.
            swarms[1].inputMotif(phraseUI->swarmNoteIndexes);       //Input new target sequence.
            swarms[1].phraseId = phraseUI->phrases[phraseUI->selectedPhrase]->id;       //Match phrases
            left->currentMotifLabel->setLabel(phraseUI->phrases[phraseUI->selectedPhrase]->selectCell->getLabel() + " selected");       //Change label to display current phrase selected.
            
            //Reset phrase checking booleans.
            left->phraseIdChanged = false;
            left->assignNewPhrase = false;
            left->selectMotifToggle->setChecked(false);
            
        }
        
        //Swarm 2 checking.
        if (right->assignNewPhrase == true || phraseUI->phrases[phraseUI->selectedPhrase]->id == swarms[2].phraseId) {
            
            right->resetParticleIntervals();
            swarms[2].inputMotif(phraseUI->swarmNoteIndexes);           //Reset particle intervals
            //Input new target sequences
            swarms[2].phraseId = phraseUI->phrases[phraseUI->selectedPhrase]->id;       //Match phrases
            right->currentMotifLabel->setLabel(phraseUI->phrases[phraseUI->selectedPhrase]->selectCell->getLabel() + " selected");                              //Change label to display current phrase selected.
            
            //Reset phrase checking booleans.
            right->phraseIdChanged = false;
            right->assignNewPhrase = false;
            right->selectMotifToggle->setChecked(false);
        }
        
        
        //phraseChanged is now set to false to indicate this process is now over, and will be run again when either the Swarm 'selectCurrentMotif' toggles are activated or if the selected Phrase's grid is altered.
        phraseUI->phraseChanged = false;
    }

}

//--------------------------------------------------------------

void ofApp::draw(){
    

    //If introScreen is true, only display the contents of the introduction screen.
    if (introScreen == true) {
        
        displayIntroScreen();
        
    //When introScreen is false, begin the processes/functionality of the main program.
    } else {
    
        
    sendMIDI();                     //Sending of MIDI messages to Ableton based upon currently best-ranked note sequences, velocity, and rhythm sequence.
        
    displayAreaSegments();          //Display the lines that segment the screen into the different functionality areas.
        
    checkPhraseDeleted();           //Function monitors whether a phrase has been deleted from the current phrase list.
        
    checkPhraseChanged();
    
    //When PhhraseUI button is pressed, send MIDI of grid notes to Ableton to play.
    if (phraseUI->playPhraseBool == true) {
        playCurrentPhrase();
    }
    


    
    
    //Drawing the individial swarm user interfaces
    left->drawInterface();
    right->drawInterface();
    
    
    
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
        if (changeRhythm == true && changeRhythmInt % 1 == 0) {


            
            if (swarms[2].play == true) {
    
                swarms[2].runRhythm();
                swarms[2].run(&swarms[1], rhythmPlayHead, swarms[2].notePlayhead, swarms[1].notePlayhead);
                
            }
            
            
            if (swarms[1].play == true) {
                
                swarms[1].runRhythm();
                swarms[1].run(&swarms[2], rhythmPlayHead, swarms[1].notePlayhead, swarms[2].notePlayhead);
                
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

    

    
}



//--------------------------------------------------------------

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e) {
    
    //Set swarm to play.
    //Response to playSwarms toggle.
    if (e.target == playSwarmsToggle && e.checked == true && startSwarm == false) {
        
        startSwarm = true;
        lastCount = 0;
        currentCount = 0;
        rhythmPlayHead = 0;
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
void ofApp::mouseReleased(int x, int y, int button){
    
    if (introScreen == true) {
        introScreen = false;
    } else {
    
    
    if (phraseUI->phrases.size() >= 1) {

        for (int i = 0; i < phraseUI->phrases[phraseUI->selectedPhrase]->phraseCells.size(); i++) {
            phraseUI->phrases[phraseUI->selectedPhrase]->phraseCells[i]->changedOnce = false;
        }
           phraseUI->phrases[phraseUI->selectedPhrase]->checkGridPressed();
      
    }
    }
    
}


//--------------------------------------------------------------

void ofApp::displayInfoAreaText() {
    
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
        info = "PHRASE DISTANCE : This slider controls whether you want the swarm to locate the phrase exactly as you have entered it, or if you would like the swarm to search for variations of the selected phrase. A value of 0 means \nthat the swarm will try locate the exact phrase, whereas a value of 100 means that the swarm will find phrases largely dissimilar to the original input phrase. When the PHRASE DISTANCE value is larger than 0, the interval \nlikelihoods that you have controlled will come into play, as the swarm will find variations of the phrase utilising the intervals you have chosen.";
    }
    
    //Search Intensity
    if (((x >= 50 && x <= 350) || (x >= 400 && x<= 700)) && (y >= 334 && y<= 360)) {
        info = "SEARCH INTENSITY : This slider controls the intensity of the swarmÕs search towards the inputted phrase. If the preferred phrase distance is 0, and you would like the swarm to reach the exact inputted phrase quickly, \na higher search intensity means that swarm is more likely to find the exact phrase in a quicker time.";
    }
    
    //Rhythm Speed
    if (((x >= 50 && x <= 350) || (x >= 400 && x<= 700)) && (y >= 380 && y<= 406)) {
      info = "RHYTHM SPEED : This slider controls the ÒspeedÓ of the rhythm. It is not equivalent to the global tempo slider, but instead decides how many notes to divide the bar of 4 beats into. A value of 1 means that the bar \ncontains 1 note, a semibreve or whole note. A value of 16 means that the bar is divided into 16 beats, which equates to 16 semiquavers per bar. Values in between 1 and 16 will create different rhythms composed \nof beats will values: 4, 2, 1, 1/2, and 1/4.";
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
    
    
    //Phrase List
    if (phraseUI->phrases.size() > 0) {
        
        //Play phrase
        if (x >= 1150 && x <= 1325 && y >= 700 && y <= 726) {
            info = "PLAY SELECTED PHRASE : Click to hear what the currently selected phrase sounds likes.";
        }
        
        //Delete phrase
        if (x >= 1362 && x <= 1512  && y >= 700 && y <= 726) {
            info = "DELETE SELECTED PHRASE : Click to delete the currently selected phrase.";
        }

        
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
