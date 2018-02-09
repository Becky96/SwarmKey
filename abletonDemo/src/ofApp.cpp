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
        swarms[i].play();
        
        cout << swarms[i].particles[0]->indFreqs[0] << endl;
        
    }
    
    
    
    int x = 400;
    int y = 200;
    //PSO GUI
    constrictionRate = new ofxDatGuiSlider(constrictionRateF.set("Constriction Rate", 0.8984, 0., 1.));
    constrictionRate->setPosition(x, y);
    constrictionRate->onSliderEvent(this, &ofApp::onSliderEvent);
    PSOcomponents.push_back(constrictionRate);
    y+=constrictionRate->getHeight();
    
    
    PSOc1 = new ofxDatGuiSlider(c1F.set("Learning factor 1", 1.4, 0., 4.));
    PSOc1->setPosition(x, y);
    PSOc1->onSliderEvent(this, &ofApp::onSliderEvent);
    PSOcomponents.push_back(PSOc1);
    y+=PSOc1->getHeight();
    
    
    PSOc2 = new ofxDatGuiSlider(c2F.set("Learning factor 2", 1.4, 0., 4.));
    PSOc2->setPosition(x, y);
    PSOc2->onSliderEvent(this, &ofApp::onSliderEvent);
    PSOcomponents.push_back(PSOc2);
    y+=PSOc2->getHeight();
    
    PSOdt = new ofxDatGuiSlider(PSOdtF.set("Distubance Threshold", 0.01, 0., 1.));
    PSOdt->setPosition(x, y);
    PSOdt->onSliderEvent(this, &ofApp::onSliderEvent);
    PSOcomponents.push_back(PSOdt);
    y+=PSOdt->getHeight();
    
    PSOtarget = new ofxDatGuiSlider(PSOtI.set("Target MIDI", 24, 0,56));
    PSOtarget->setPosition(x, y);
    PSOtarget->onSliderEvent(this, &ofApp::onSliderEvent);
    PSOcomponents.push_back(PSOtarget);
    y+=PSOtarget->getHeight();
    
    
    tempoSlider = new ofxDatGuiSlider(tempoInt.set("Tempo", 4, 1, 12));
    tempoSlider->setPosition(x, y);
    tempoSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    y+=tempoSlider->getHeight();
    
    
    //First
    firstPen = new ofxDatGuiSlider(firstInt.set("1st", 100, 0, 10000));
    firstPen->setPosition(x, y);
    firstPen->onSliderEvent(this, &ofApp::onSliderEvent);
    intervalPenalties.push_back(firstPen);
    y+=firstPen->getHeight();
    
    //Second
    secondPen = new ofxDatGuiSlider(secondInt.set("2nd", 10000, 0, 10000));
    secondPen->setPosition(x, y);
    secondPen->onSliderEvent(this, &ofApp::onSliderEvent);
    intervalPenalties.push_back(secondPen);
    y+=secondPen->getHeight();
    
    thirdPen = new ofxDatGuiSlider(thirdInt.set("3rd", 100, 0, 10000));
    thirdPen->setPosition(x, y);
    thirdPen->onSliderEvent(this, &ofApp::onSliderEvent);
    intervalPenalties.push_back(thirdPen);
    y+=thirdPen->getHeight();
    
    fourthPen = new ofxDatGuiSlider(fourthInt.set("4th", 1000, 0, 10000));
    fourthPen->setPosition(x, y);
    fourthPen->onSliderEvent(this, &ofApp::onSliderEvent);
    intervalPenalties.push_back(fourthPen);
    y+=fourthPen->getHeight();
    
    fifthPen = new ofxDatGuiSlider(fifthInt.set("5th", 100, 0, 10000));
    fifthPen->setPosition(x, y);
    fifthPen->onSliderEvent(this, &ofApp::onSliderEvent);
    intervalPenalties.push_back(fifthPen);
    y+=fifthPen->getHeight();
    
    sixthPen = new ofxDatGuiSlider(sixthInt.set("6th", 1000, 0, 10000));
    sixthPen->setPosition(x, y);
    sixthPen->onSliderEvent(this, &ofApp::onSliderEvent);
    intervalPenalties.push_back(sixthPen);
    y+=sixthPen->getHeight();
    
    seventhPen = new ofxDatGuiSlider(seventhInt.set("7th", 10000, 0, 10000));
    seventhPen->setPosition(x, y);
    seventhPen->onSliderEvent(this, &ofApp::onSliderEvent);
    intervalPenalties.push_back(seventhPen);
    y+=seventhPen->getHeight();
    
    eighthPen = new ofxDatGuiSlider(eighthInt.set("8th", 100, 0, 10000));
    eighthPen->setPosition(x, y);
    eighthPen->onSliderEvent(this, &ofApp::onSliderEvent);
    intervalPenalties.push_back(eighthPen);
    y+=eighthPen->getHeight();
    
    elsePen = new ofxDatGuiSlider(elseInt.set("other", 10000, 0, 10000));
    elsePen->setPosition(x, y);
    elsePen->onSliderEvent(this, &ofApp::onSliderEvent);
    intervalPenalties.push_back(elsePen);
    y+=elsePen->getHeight();


    
    
    sampleRate = 44100;
    bufferSize = 1025;
    
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2,0, this, maxiSettings::sampleRate, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
    

    
    for (int i = 0; i < PSOcomponents.size(); i++) {
        PSOcomponents[i]->update();
    }
    
    
    tempoSlider->update();
    
    for (int i = 0; i < intervalPenalties.size(); i++) {
        intervalPenalties[i]->update();
    }


}


void ofApp::audioRequested(float * output, int bufferSize, int nChannels){
    
    
    for (int i = 0; i < bufferSize; i++){
        
        currentCount = (int)timer.phasor(tempo);
        
        if (lastCount != currentCount) {
            

            swarms[1].midiOut.sendNoteOn(swarms[1].channel, swarms[1].availableNotes[swarms[1].best.indFreqs[playHead%4]], swarms[1].velocity);
            //swarms[2].midiOut.sendNoteOn(swarms[2].channel, swarms[2].availableNotes[swarms[2].best.indFreqs[playHead%4]], swarms[2].velocity);

            if (playHead % 4 == 3) {
                changeNotes = true;
            }
            
            playHead++;
            lastCount = 0;
        }
        
    }
    
    
    
}   // end audioRequested

//--------------------------------------------------------------
void ofApp::draw(){
    
    
     tempoSlider->draw();
    
    for (int i = 0; i < PSOcomponents.size(); i++) {
        PSOcomponents[i]->draw();
    }
    
    for (int i = 0; i < intervalPenalties.size(); i++) {
        intervalPenalties[i]->draw();
    }


    
    if (changeNotes == true) {

        swarms[1].run();
        //swarms[2].run();

        changeNotes = false;
    }

    swarms[1].display();
    
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

}


//--------------------------------------------------------------
//Handler for sliders
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    /*
    for (int i = 0; i < swarms[1].particles.size(); i++) {
        for (int j = 0; j < 4; j++) {
            swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
            swarms[1].particles[i]->bestIndFreqs[j] = swarms[1].particles[i]->indFreqs[j];
            swarms[1].particles[i]->bestFit = 100000;
        }
    }
    */
    if (e.target == constrictionRate) {
        swarms[1].con = e.value;
        
        
        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
            }
        }
    }
    
    if (e.target == PSOc1) {
        swarms[1].c1 = e.value;
        
        
        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
            }
        }
    }
    
    if (e.target == PSOc2) {
        swarms[1].c2 = e.value;
        
        
        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
            }
        }
    }
    
    
    //Slider for disturbance threshold for DFO swarms
    if (e.target == PSOdt) {
        
        swarms[1].dt = e.value;
        
    }

    if (e.target == PSOtarget) {
        swarms[1].bestFitness = 2000000;
    
        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
                swarms[1].particles[i]->bestIndFreqs[j] = swarms[1].particles[i]->indFreqs[j];
                swarms[1].particles[i]->bestFit = 100000;
            }
        }
        
        swarms[1].target = e.value;
    }
    
    
    if (e.target == tempoSlider) {
        tempo = e.value;
    }
    
    
    if (e.target == firstPen) {
        swarms[1].firstPen = e.value;
        swarms[2].firstPen = e.value;
        swarms[1].bestFitness = 9999999999;
        
        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
                swarms[1].particles[i]->bestIndFreqs[j] = swarms[1].particles[i]->indFreqs[j];
                swarms[1].particles[i]->bestFit = 100000;
            }
        }


    }
    
    
    if (e.target == secondPen) {
        swarms[1].secondPen = e.value;
        swarms[2].secondPen = e.value;
        swarms[1].bestFitness = 9999999999;
        
        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
                swarms[1].particles[i]->bestIndFreqs[j] = swarms[1].particles[i]->indFreqs[j];
                swarms[1].particles[i]->bestFit = 100000;
            }
        }


    }

    
    
    if (e.target == thirdPen) {
        swarms[1].thirdPen = e.value;
        swarms[2].thirdPen = e.value;
        swarms[1].bestFitness = 9999999999;

        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
                swarms[1].particles[i]->bestIndFreqs[j] = swarms[1].particles[i]->indFreqs[j];
                swarms[1].particles[i]->bestFit = 100000;
            }
        }

    }

    
    
    if (e.target == fourthPen) {
        swarms[1].fourthPen = e.value;
        swarms[2].fourthPen = e.value;
        swarms[1].bestFitness = 9999999999;

        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
                swarms[1].particles[i]->bestIndFreqs[j] = swarms[1].particles[i]->indFreqs[j];
                swarms[1].particles[i]->bestFit = 100000;
            }
        }

    }

    
    
    if (e.target == fifthPen) {
        swarms[1].fifthPen = e.value;
        swarms[2].fifthPen = e.value;
        swarms[1].bestFitness = 9999999999;
        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
                swarms[1].particles[i]->bestIndFreqs[j] = swarms[1].particles[i]->indFreqs[j];
                swarms[1].particles[i]->bestFit = 100000;
            }
        }


    }

    
    
    if (e.target == sixthPen) {
        swarms[1].sixthPen = e.value;
        swarms[2].sixthPen = e.value;
        swarms[1].bestFitness = 9999999999;

        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
                swarms[1].particles[i]->bestIndFreqs[j] = swarms[1].particles[i]->indFreqs[j];
                swarms[1].particles[i]->bestFit = 100000;
            }
        }

    }

    
    
    if (e.target == seventhPen) {
        swarms[1].seventhPen = e.value;
        swarms[2].seventhPen = e.value;
        swarms[1].bestFitness = 9999999999;
        
        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
                swarms[1].particles[i]->bestIndFreqs[j] = swarms[1].particles[i]->indFreqs[j];
                swarms[1].particles[i]->bestFit = 100000;
            }
        }

    }

    
    
    if (e.target == eighthPen) {
        swarms[1].eighthPen = e.value;
        swarms[2].eighthPen = e.value;
        swarms[1].bestFitness = 9999999999;
        
        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
                swarms[1].particles[i]->bestIndFreqs[j] = swarms[1].particles[i]->indFreqs[j];
                swarms[1].particles[i]->bestFit = 100000;
            }
        }


    }

    
    if (e.target == elsePen) {
        swarms[1].elsePen = e.value;
        swarms[2].elsePen = e.value;
        swarms[1].bestFitness = 9999999999;

        for (int i = 0; i < swarms[1].particles.size(); i++) {
            for (int j = 0; j < 4; j++) {
                swarms[1].particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
                swarms[1].particles[i]->bestIndFreqs[j] = swarms[1].particles[i]->indFreqs[j];
                swarms[1].particles[i]->bestFit = 100000;
            }
        }

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
