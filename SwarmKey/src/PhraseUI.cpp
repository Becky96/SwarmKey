//  SWARM KEY
//  PhraseUI.cpp

#include "PhraseUI.hpp"

//--------------------------------------------------------------
/*PhraseUI setup.*/
void PhraseUI::setupPhraseUI() {

    //Add phrase button - will create a new phrase
    addPhrase = new ofxDatGuiButton("Add new phrase");
    addPhrase->setPosition(x, y);
    addPhrase->setWidth(buttonWidth, buttonWidth);
    addPhrase->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    addPhrase->onButtonEvent(this, &PhraseUI::onButtonEvent);
    phraseUIComponents.push_back(addPhrase);
    x+=addPhrase->getWidth()+37;
    
    x=1150;
    
    //Play phrase - will allow the user to hear the phrase before using it in the swarms
    playPhrase = new ofxDatGuiButton("Play selected phrase");
    playPhrase->setPosition(x, 700);
    playPhrase->setWidth(buttonWidth-50, buttonWidth-50);
    playPhrase->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    playPhrase->onButtonEvent(this, &PhraseUI::onButtonEvent);
    phraseUIComponents.push_back(playPhrase);
    x+=playPhrase->getWidth()+37;
    
    //Delete phrase - will delete the currently selected phrase
    deletePhrase = new ofxDatGuiButton("Delete selected phrase");
    deletePhrase->setPosition(x, 700);
    deletePhrase->setWidth(buttonWidth-50, buttonWidth-50);
    deletePhrase->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    deletePhrase->onButtonEvent(this, &PhraseUI::onButtonEvent);
    phraseUIComponents.push_back(deletePhrase);
    x+=deletePhrase->getWidth();
    
    //Phrase List Label is used to show the area of created phrases
    phraseListLabel = new ofxDatGuiLabel("Phrase List");
    phraseListLabel->setPosition(listX, listY);
    phraseListLabel->setWidth(buttonWidth, buttonWidth);
    phraseListLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    phraseUIComponents.push_back(phraseListLabel);
    listY+=phraseListLabel->getHeight();
    
    //Setting all UI components background colours
    for (int i = 0; i < phraseUIComponents.size()-1; i++) {
        phraseUIComponents[i]->setBackgroundColor(ofColor(25, 47, 55));
    }
    
    //PhraseListLabel has a different background colour for emphasis
    phraseListLabel->setBackgroundColor(ofColor(91, 125, 123));
}

//--------------------------------------------------------------
/*Updating all PhraseUI components*/
void PhraseUI::updatePhraseUI() {
    
    for (int i = 0; i < phraseUIComponents.size(); i++) {
        phraseUIComponents[i]->update();
    }
    
    for (int i = 0; i < phrases.size(); i++) {
        phrases[i]->updatePhraseButton();
    }
}

//--------------------------------------------------------------
/*Function is responsible for playing all UI components.*/
void PhraseUI::displayPhraseUI() {

    //Displays addPhrase button
    phraseUIComponents[0]->draw();
    
    //If phrase size is larger than 1, all other Phrase UI components are drawn.
    if (phrases.size() >= 1) {
        for (int i = 1; i < phraseUIComponents.size(); i++) {
            phraseUIComponents[i]->draw();
        }
    }
    
    
    ofFill();
    ofSetColor(255);
    
    //If phrase is larger than 1, the keys which represent the Phrase's grid rows are displayed.
    if (phrases.size() >= 1) {
        for (int i = currentKeyNotes.size()-1; i > -1; i--) {
            ofDrawBitmapString((currentKeyNotes[i]), textX, textY + (14*25) - (i * 25));
        }
    }
    
}

//--------------------------------------------------------------
/*Displays list of all phrases (all currently made phrases)*/
void PhraseUI::displayPhraseList() {
    
    for (int i = 0; i < phrases.size(); i++) {
        phrases[i]->displayPhraseButton();
    }
    
}

//--------------------------------------------------------------
/*Function displays the editing mode for currently selected phrase (display the grid of the currently selected phrase) and is responsible for making sure only one phrase is active at any one time. It then makes sure that all note sequences used by the Swarms are updated to store the note sequence of the current phrase.*/
void PhraseUI::displaySelectedPhrase() {
 
    
    //Function checks if any of the individual Phrase object's buttons have been pressed,
    //if so, it will set all other boolean displayGrids to false so that it is the only one being displayed.
    for (int i = 0; i < phrases.size(); i++) {
        //Determines which phrase is now active
        if (phrases[i]->buttonPressed == true) {
            
            //Selected phrase has now changed
            phraseChanged = true;
            
            //SwarmNoteIndexes now stores the note sequence the phrase's grid holds.
            for (int j = 0; j < 16; j++) {
                swarmNoteIndexes[j] = (phrases[i]->phraseList[j]+17);
            }
            
            //Set all other phrase's to not selected so their grids are not displayed.
            for (int j = 0; j < phrases.size(); j++) {
                if (j != i) {
                    phrases[j]->displayGrid = false;
                }
            }
            
            //Set selected phrase buttonPressed to false so this process is not repeated until the next time a phrase's selectCell button is pressed.
            phrases[i]->buttonPressed = false;
        }
    }
    
    
    //Displays the currently selected grid.
    for (int i = 0; i < phrases.size(); i++) {
        
        if (phrases[i]->displayGrid == true) {
            //Selected phrase now becomes the index of this phrase
            selectedPhrase = i;
            
            //Displaying the selected phrase's grid
            phrases[i]->displayPhraseGrid();
            
            //Stores the MIDI note sequence of the current phrase's sequence
            for (int j = 0; j < 16; j++) {
                currentPhrase[j] = currentMidiNotes[phrases[selectedPhrase]->phraseList[j]];
            }
            
        } else {
            //Highlights the label of the currently selected phrase.
            phrases[i]->selectCell->setStripeColor(0);
        }
    }
    
}

//--------------------------------------------------------------
/*Calculates the correct key for the phrase editor. This is based on the global UI to determine the key type and tonic.*/
void PhraseUI::calculatePhraseKey(int keyType, int tonic) {
    
    //Clear all previous available notes.
    currentKeyNotes.clear();
    currentMidiNotes.clear();
    
    //Index is assigned to the index of the midiNote vector item which matches the inputted tonic.
    //Once the position of the tonic is known in the vector, currentKeyNotes and currentMidiNotes are updated to store the viable notes which represent each grid row of the phrase.
    int index;
    
    for (int i = 0; i < midiNotes.size(); i++) {
        if (midiNotes[i] == tonic) {
            index = i;
        }
    }
    
    //If major key
    if (keyType == 1) {
        
        //Storing all viable key note names
        currentKeyNotes.push_back( notes[index-7] );
        currentKeyNotes.push_back( notes[index-5] );
        currentKeyNotes.push_back( notes[index-3] );
        currentKeyNotes.push_back( notes[index-1] );
        currentKeyNotes.push_back( notes[index] );
        currentKeyNotes.push_back( notes[index+2] );
        currentKeyNotes.push_back( notes[index+4] );
        currentKeyNotes.push_back( notes[index+5] );
        currentKeyNotes.push_back( notes[index+7] );
        currentKeyNotes.push_back( notes[index+9] );
        currentKeyNotes.push_back( notes[index+11] );
        currentKeyNotes.push_back( notes[index+12] );
        currentKeyNotes.push_back( notes[index+14] );
        currentKeyNotes.push_back( notes[index+16] );
        currentKeyNotes.push_back( notes[index+17] );
        
        //Storing all viable midi notes
        currentMidiNotes.push_back( midiNotes[index-7] );
        currentMidiNotes.push_back( midiNotes[index-5] );
        currentMidiNotes.push_back( midiNotes[index-3] );
        currentMidiNotes.push_back( midiNotes[index-1] );
        currentMidiNotes.push_back( midiNotes[index] );
        currentMidiNotes.push_back( midiNotes[index+2] );
        currentMidiNotes.push_back( midiNotes[index+4] );
        currentMidiNotes.push_back( midiNotes[index+5] );
        currentMidiNotes.push_back( midiNotes[index+7] );
        currentMidiNotes.push_back( midiNotes[index+9] );
        currentMidiNotes.push_back( midiNotes[index+11] );
        currentMidiNotes.push_back( midiNotes[index+12] );
        currentMidiNotes.push_back( midiNotes[index+14] );
        currentMidiNotes.push_back( midiNotes[index+16] );
        currentMidiNotes.push_back( midiNotes[index+17] );
        
    }
    
    //If minor key
    if (keyType == 2) {
        
        //Storing all viable key note names
        currentKeyNotes.push_back( notes[index-7] );
        currentKeyNotes.push_back( notes[index-5] );
        currentKeyNotes.push_back( notes[index-4] );
        currentKeyNotes.push_back( notes[index-2] );
        currentKeyNotes.push_back( notes[index] );
        currentKeyNotes.push_back( notes[index+2] );
        currentKeyNotes.push_back( notes[index+3] );
        currentKeyNotes.push_back( notes[index+5] );
        currentKeyNotes.push_back( notes[index+7] );
        currentKeyNotes.push_back( notes[index+8] );
        currentKeyNotes.push_back( notes[index+10] );
        currentKeyNotes.push_back( notes[index+12] );
        currentKeyNotes.push_back( notes[index+14] );
        currentKeyNotes.push_back( notes[index+15] );
        currentKeyNotes.push_back( notes[index+17] );
        
        //Storing all viable midi notes
        currentMidiNotes.push_back( midiNotes[index-7] );
        currentMidiNotes.push_back( midiNotes[index-5] );
        currentMidiNotes.push_back( midiNotes[index-4] );
        currentMidiNotes.push_back( midiNotes[index-2] );
        currentMidiNotes.push_back( midiNotes[index] );
        currentMidiNotes.push_back( midiNotes[index+2] );
        currentMidiNotes.push_back( midiNotes[index+3] );
        currentMidiNotes.push_back( midiNotes[index+5] );
        currentMidiNotes.push_back( midiNotes[index+7] );
        currentMidiNotes.push_back( midiNotes[index+8] );
        currentMidiNotes.push_back( midiNotes[index+10] );
        currentMidiNotes.push_back( midiNotes[index+12] );
        currentMidiNotes.push_back( midiNotes[index+14] );
        currentMidiNotes.push_back( midiNotes[index+15] );
        currentMidiNotes.push_back( midiNotes[index+17] );
    }

}

//--------------------------------------------------------------
/*Function responds to when addPhrase, deletePhrase, and playPhrase are pressed.*/
void PhraseUI::onButtonEvent(ofxDatGuiButtonEvent e) {
    
    
    //If add phrase button is pressed, a new phrase is created and positioned in the phrase list with a unique id. This phrase is then set to the selected phrase.
    if (e.target == addPhrase) {
        Phrase * p = new Phrase;
        p->setupPhrase(id, listX, listY);               //Setting phrase with id and position
        selectedPhrase = id-1;                          //Setting selectedPhrase to this phrase
        listY+=p->selectCell->getHeight();              //Adjusting listY for next added phrase
        phrases.push_back(p);                           //Adding phrase to vector
        
        //Loop through all created phrases and only display the grid of the new phrase.
        for (int i = 0; i < phrases.size(); i++) {
            
            //Setting newly created phrase displayGrid boolean to true
            if (i == selectedPhrase) {
                phrases[i]->displayGrid = true;
                phrases[i]->selectCell->setStripeColor(255);
                //Setting all other displayGrid booleans to false apart from the currently selected.
            } else {
                phrases[i]->displayGrid = false;
                phrases[i]->selectCell->setStripeColor(0);
            }
        }
        
        //Phrase changed becomes true to signal change in note sequence.
        phraseChanged = true;
        
        //Incrementing id for next added phrase
        id++;
    }
    
    
    //If play phrase button is pressed, ofApp::draw() responds by sending MIDI messages of the currently selected phrase's note sequence.
    if (e.target == playPhrase) {
        playPhraseBool = true;
    }
    
    //If delete phrase button is pressed, the phrase list is redrawn by adjusting the coordinates of each phrase and reassigning each id to a phrase. This then signals to ofApp to adjust each swarm's target phrase in order to ensure they have still selected the correct phrase if it has not been deleted, and to deselect the phrase if it has been deleted.
    if (e.target == deletePhrase) {
        
        //Delete button functionality is only used if there is more than 1 phrase currently active.
        if (phrases.size() >= 1) {

            //Set phrase's to be deleted 'displayGrid' to false so previous functions do not try to display it.
            phrases[selectedPhrase]->displayGrid = false;
            
            //Delete all phrase cell pointers in the phrase to be deleted.
            for (int i = 0; i < phrases[selectedPhrase]->phraseCells.size(); i++) {
                delete phrases[selectedPhrase]->phraseCells[i];
            }
            
            //Delete phrase's selectCell button
            delete phrases[selectedPhrase]->selectCell;
            
            //Delete phrase
            delete phrases[selectedPhrase];
            
            //Erase phrase from vector list.
            phrases.erase(phrases.begin() + selectedPhrase);
            
            //Reset the y coordinate of the buttons to the top of the list and reposition them
            //without the phrase that has now been deleted.
            listY = 300;
            listY+=phraseListLabel->getHeight();
            
            //Resetting phrase IDs and positions.
            id = 1;
            
            //Resetting the position of each phrase remaining, and assigning it a new id.
            for (int i = 0; i < phrases.size(); i++) {
                phrases[i]->selectCell->setPosition(listX, listY);
                listY+=phrases[i]->selectCell->getHeight();
                phrases[i]->prevId = phrases[i]->id;
                phrases[i]->id = id;
                phrases[i]->selectCell->setLabel("phrase " + ofToString(id));
                id++;           //Incrementing id for the next phrase
            }
            
            //Setting selectedPhrase to the last phrase's id in the list.
            if (phrases.size() >= 1) {
                selectedPhrase = phrases[phrases.size()-1]->id-1;
            }
            
            //ResetPhraseIds signals to ofApp that if the swarms need to check if there selected phrase's id has changed or if it has been deleted.
            resetPhraseIds = true;

        }
    }
}
//--------------------------------------------------------------
