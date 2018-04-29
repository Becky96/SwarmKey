//
//  PhraseUI.cpp
//  abletonDemo
//



#include "PhraseUI.hpp"


//Set up phrase UI
void PhraseUI::setupPhraseUI() {

    addPhrase = new ofxDatGuiButton("Add new phrase");
    addPhrase->setPosition(x, y);
    addPhrase->setWidth(buttonWidth, buttonWidth);
    addPhrase->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    addPhrase->onButtonEvent(this, &PhraseUI::onButtonEvent);
    phraseUIComponents.push_back(addPhrase);
    x+=addPhrase->getWidth()+37;
    
    playPhrase = new ofxDatGuiButton("Play selected phrase");
    playPhrase->setPosition(x, y);
    playPhrase->setWidth(buttonWidth, buttonWidth*20);
    playPhrase->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    playPhrase->onButtonEvent(this, &PhraseUI::onButtonEvent);
    phraseUIComponents.push_back(playPhrase);
    x+=playPhrase->getWidth()+37;
    
    deletePhrase = new ofxDatGuiButton("Delete selected phrase");
    deletePhrase->setPosition(x, y);
    deletePhrase->setWidth(buttonWidth, buttonWidth);
    deletePhrase->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    deletePhrase->onButtonEvent(this, &PhraseUI::onButtonEvent);
    phraseUIComponents.push_back(deletePhrase);
    x+=deletePhrase->getWidth();
    
    phraseListLabel = new ofxDatGuiLabel("Phrase List");
    phraseListLabel->setPosition(listX, listY);
    phraseListLabel->setWidth(buttonWidth, buttonWidth);
    phraseListLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    phraseUIComponents.push_back(phraseListLabel);
    listY+=phraseListLabel->getHeight();
    
    for (int i = 0; i < phraseUIComponents.size()-1; i++) {
        phraseUIComponents[i]->setBackgroundColor(ofColor(25, 47, 55));
    }
    
    phraseListLabel->setBackgroundColor(ofColor(48, 68, 74));
}


//Update phrase UI
void PhraseUI::updatePhraseUI() {
    
    for (int i = 0; i < phraseUIComponents.size(); i++) {
        phraseUIComponents[i]->update();
    }
    
    for (int i = 0; i < phrases.size(); i++) {
        phrases[i]->updatePhraseButton();
    }
}


//Display phrase UI (add phrase)
void PhraseUI::displayPhraseUI() {

    for (int i = 0; i < phraseUIComponents.size()-1; i++) {
        phraseUIComponents[i]->draw();
    }
    
    if (phrases.size() >= 1) {
        phraseUIComponents[3]->draw();
    }
    
    
    ofFill();
    ofSetColor(255);
    
    if (phrases.size() >= 1) {
    for (int i = currentKeyNotes.size()-1; i > -1; i--) {
        ofDrawBitmapString((currentKeyNotes[i]), textX, textY + (14*25) - (i * 25));
    }
    }
    
}

//Display list of all phrases (all currently made phrases)
void PhraseUI::displayPhraseList() {
    
    for (int i = 0; i < phrases.size(); i++) {
        phrases[i]->displayPhraseButton();
    }
    
}


//Display editing mode for currently selected phrase (display the grid of the currently selected phrase)
void PhraseUI::displaySelectedPhrase() {
 
    
    //Function checks if any of the individual Phrase object's buttons have been pressed,
    //if so, it will set all other boolean displayGrids to false so that it is the only one being displayed.
    for (int i = 0; i < phrases.size(); i++) {
        if (phrases[i]->buttonPressed == true) {
            
            phraseChanged = true;
            
            for (int j = 0; j < 16; j++) {
                swarmNoteIndexes[j] = (phrases[i]->phraseList[j]+17);
            }
            
            for (int j = 0; j < phrases.size(); j++) {
                if (j != i) {
                    phrases[j]->displayGrid = false;
                }
            }
            
            phrases[i]->buttonPressed = false;
        }
    }
    
    
    //Displays the currently selected grid.
    for (int i = 0; i < phrases.size(); i++) {
        
        if (phrases[i]->displayGrid == true) {
            selectedPhrase = i;
            phrases[i]->displayPhraseGrid();
            
            for (int j = 0; j < 16; j++) {

           // ofDrawBitmapString(ofToString(currentMidiNotes[phrases[selectedPhrase]->phraseList[j]]), listX+(j*50), 450);
            currentPhrase[j] = currentMidiNotes[phrases[selectedPhrase]->phraseList[j]];
           //     swarmNoteIndexes[j] = phrases[selectedPhrase]->phraseList[j] + 17;
           //     phraseChanged = true;
                
            }
        } else {
            phrases[i]->selectCell->setStripeColor(0);
        }

        
    }
    
    
    
    
}



void PhraseUI::calculatePhraseKey(int keyType, int tonic) {
    
    currentKeyNotes.clear();
    currentMidiNotes.clear();
    
    int index;
    
    for (int i = 0; i < midiNotes.size(); i++) {
        
        if (midiNotes[i] == tonic) {
            index = i;
        }
        
    }
    
    
    
    
    
    //If major key
    if (keyType == 1) {
        
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

    
    
    
    for (int i = 0; i < currentKeyNotes.size(); i++) {
        cout << currentKeyNotes[i] << ", ";
    }
    
    for (int i = 0; i < currentMidiNotes.size(); i++) {
        cout << currentMidiNotes[i] << ", ";
    }
    

    
}

void PhraseUI::onButtonEvent(ofxDatGuiButtonEvent e) {
    
    
    //If add phrase button is pressed.
    if (e.target == addPhrase) {
        Phrase * p = new Phrase;
        p->setupPhrase(id, listX, listY);
        selectedPhrase = id-1;
        listY+=p->selectCell->getHeight();
        phrases.push_back(p);
        
        //Loop through all created phrases and only display the grid of the new phrase.
        for (int i = 0; i < phrases.size(); i++) {
            
            //Setting newly created phrase displayGrid boolean to true
            if (i == selectedPhrase) {
                cout << "selected" << endl;
                phrases[i]->displayGrid = true;
                phrases[i]->selectCell->setStripeColor(255);
                //Setting all other displayGrid booleans to false apart from the currently selected.
            } else {
                cout << "not selected" << endl;
                phrases[i]->displayGrid = false;
                phrases[i]->selectCell->setStripeColor(0);
            }
        }
        
        phraseChanged = true;
        
        id++;

    }
    
    
    //If play phrase button is pressed.
    if (e.target == playPhrase) {
        playPhraseBool = true;
    }
    
    //If delete phrase button is pressed.
    if (e.target == deletePhrase) {
        
        //Delete button functionality is only used if there is more than 1 phrase currently active.
        if (phrases.size() >= 1) {
            cout << "phrase list size: " << phrases.size() << endl;
            cout << "selected: " << selectedPhrase << endl;
            
            
            //listY-=phrases[selectedPhrase]->selectCell->getHeight();
            
            
            

            //Set phrase's to be deleted 'displayGrid' to false so previous functions do not try to display it.
            phrases[selectedPhrase]->displayGrid = false;
            
            
           // listY-= (phrases[selectedPhrase]->selectCell->getHeight()*(selectedPhrase+1));
            
            
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
            
            
            listY = 300;
            listY+=phraseListLabel->getHeight();

            id = 1;
            
            for (int i = 0; i < phrases.size(); i++) {
                
                phrases[i]->selectCell->setPosition(listX, listY);
                listY+=phrases[i]->selectCell->getHeight();
                phrases[i]->id = id;
                phrases[i]->selectCell->setLabel("phrase " + ofToString(id));
               
                id++;
            }
            
            if (phrases.size() >= 1) {
            selectedPhrase = phrases[phrases.size()-1]->id-1;
            }

        }
        
    }
}