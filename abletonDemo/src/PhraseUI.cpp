//
//  PhraseUI.cpp
//  abletonDemo
//



#include "PhraseUI.hpp"


//Set up phrase UI
void PhraseUI::setupPhraseUI() {

    addPhrase = new ofxDatGuiButton("Add new phrase");
    addPhrase->setPosition(x, y);
    addPhrase->onButtonEvent(this, &PhraseUI::onButtonEvent);
    phraseUIComponents.push_back(addPhrase);
    y+=addPhrase->getHeight();
    
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
    for (int i = 0; i < phraseUIComponents.size(); i++) {
        phraseUIComponents[i]->draw();
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
        }
    }
    
}


void PhraseUI::onButtonEvent(ofxDatGuiButtonEvent e) {
    
    if (e.target == addPhrase) {
        Phrase * p = new Phrase;
        p->setupPhrase(id, listX, listY);
        selectedPhrase = id-1;
        listY+=p->selectCell->getHeight();
        phrases.push_back(p);
        
        //Loop through all created phrases and only display the grid of the new phrase.
        for (int i = 0; i < phrases.size(); i++) {
            
            //Setting newly created phrase displayGrid boolean to true
            if (i == phrases[i]->id-1) {
                phrases[i]->displayGrid = true;
                
                //Setting all other displayGrid booleans to false apart from the currently selected.
            } else {
                phrases[i]->displayGrid = false;
            }
        }
        
        id++;

    }
}