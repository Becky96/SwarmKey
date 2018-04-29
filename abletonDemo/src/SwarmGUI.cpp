//
//  SwarmGUI.cpp
//  abletonDemo
//
//  Created by Becky Johnson on 23/02/2018.
//
//

#include "SwarmGUI.hpp"

//--------------------------------------------------------------

void SwarmGUI::setupInterface() {
    
    textY = y;
    
    swarmLabel = new ofxDatGuiLabel(label);
    swarmLabel->setPosition(x, y);
    swarmLabel->setWidth(UIWidth);
    swarmLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    swarmComponents.push_back(swarmLabel);
    y+=swarmLabel->getHeight();
    
    //GUI setup of toggle and sliders.
    selectMotifToggle = new ofxDatGuiToggle("Select phrase from list", false);
    selectMotifToggle->onToggleEvent(this, &SwarmGUI::onToggleEvent);
    selectMotifToggle->setPosition(x, y);
    selectMotifToggle->setWidth(UIWidth);
    selectMotifToggle->setChecked(false);
    motifComponents.push_back(selectMotifToggle);
    y+=selectMotifToggle->getHeight();

    currentMotifLabel = new ofxDatGuiLabel("");
    currentMotifLabel->setPosition(x, y);
    currentMotifLabel->setWidth(UIWidth);
    motifComponents.push_back(currentMotifLabel);
    y+=currentMotifLabel->getHeight();
    
    desiredNoteDistSlider = new ofxDatGuiSlider(desiredNoteDistInt.set("Phrase distance", 0, 0, 100));
    desiredNoteDistSlider->setPosition(x, y);
    desiredNoteDistSlider->setWidth(UIWidth, sliderRatio);
    desiredNoteDistSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    motifComponents.push_back(desiredNoteDistSlider);
    y+=desiredNoteDistSlider->getHeight();
    
    searchIntensitySlider = new ofxDatGuiSlider(searchIntensityInt.set("Search intensity", 1, 1, 100));
    searchIntensitySlider->setPosition(x, y);
    searchIntensitySlider->setWidth(UIWidth, sliderRatio);
    searchIntensitySlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    motifComponents.push_back(searchIntensitySlider);
    y+=searchIntensitySlider->getHeight();
    
    y+=20;
    
    desiredRhythmDistSlider = new ofxDatGuiSlider(desiredRhythmDistInt.set("Rhythm speed", swarm->dimensionalityMotif, 1, 16));
    desiredRhythmDistSlider->setPosition(x, y);
    desiredRhythmDistSlider->setWidth(UIWidth, sliderRatio);
    desiredRhythmDistSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    motifComponents.push_back(desiredRhythmDistSlider);
    y+=desiredRhythmDistSlider->getHeight();
    

    velocitySlider = new ofxDatGuiSlider(velocityInt.set("Velocity Level", maxVelocity/2., 0, maxVelocity));
    velocitySlider->setPosition(x, y);
    velocitySlider->setWidth(UIWidth, sliderRatio);
    velocitySlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    swarmComponents.push_back(velocitySlider);
    y+=velocitySlider->getHeight();
    
    octaveSlider = new ofxDatGuiSlider(octaveInt.set("Octave Level", 4, 1, 8));
    octaveSlider->setPosition(x, y);
    octaveSlider->setWidth(UIWidth, sliderRatio);
    octaveSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    swarmComponents.push_back(octaveSlider);
    y+=octaveSlider->getHeight();
    
    chordSlider = new ofxDatGuiSlider(chordInt.set("Chord potential", 0, 0, 100));
    chordSlider->setPosition(x, y);
    chordSlider->setWidth(UIWidth, sliderRatio);
    chordSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    swarmComponents.push_back(chordSlider);
    y+=chordSlider->getHeight();
    
    y+=20;
    
    //Interval penalties for specific swarm
    //First
    firstPen = new ofxDatGuiSlider(firstInt.set("Interval of 1", 90, 0, 100));
    firstPen->setPosition(x, y);
    firstPen->setWidth(UIWidth, sliderRatio);
    firstPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(firstPen);
    y+=firstPen->getHeight();
    
    //Second
    secondPen = new ofxDatGuiSlider(secondInt.set("Interval of 2", 0, 0, 100));
    secondPen->setPosition(x, y);
    secondPen->setWidth(UIWidth, sliderRatio);
    secondPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(secondPen);
    y+=secondPen->getHeight();
    
    thirdPen = new ofxDatGuiSlider(thirdInt.set("Interval of 3", 100, 0, 100));
    thirdPen->setPosition(x, y);
    thirdPen->setWidth(UIWidth, sliderRatio);
    thirdPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(thirdPen);
    y+=thirdPen->getHeight();
    
    fourthPen = new ofxDatGuiSlider(fourthInt.set("Interval of 4", 50, 0, 100));
    fourthPen->setPosition(x, y);
    fourthPen->setWidth(UIWidth, sliderRatio);
    fourthPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(fourthPen);
    y+=fourthPen->getHeight();
    
    fifthPen = new ofxDatGuiSlider(fifthInt.set("Interval of 5", 100, 0, 100));
    fifthPen->setPosition(x, y);
    fifthPen->setWidth(UIWidth, sliderRatio);
    fifthPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(fifthPen);
    y+=fifthPen->getHeight();
    
    sixthPen = new ofxDatGuiSlider(sixthInt.set("Interval of 6", 50, 0, 100));
    sixthPen->setPosition(x, y);
    sixthPen->setWidth(UIWidth, sliderRatio);
    sixthPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(sixthPen);
    y+=sixthPen->getHeight();
    
    seventhPen = new ofxDatGuiSlider(seventhInt.set("Interval of 7", 0, 0, 100));
    seventhPen->setPosition(x, y);
    seventhPen->setWidth(UIWidth, sliderRatio);
    seventhPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(seventhPen);
    y+=seventhPen->getHeight();
    
    eighthPen = new ofxDatGuiSlider(eighthInt.set("Interval of 8", 100, 0, 100));
    eighthPen->setPosition(x, y);
    eighthPen->setWidth(UIWidth, sliderRatio);
    eighthPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(eighthPen);
    y+=eighthPen->getHeight();
    
    elsePen = new ofxDatGuiSlider(elseInt.set("Intervals above 8", 0, 0, 100));
    elsePen->setPosition(x, y);
    elsePen->setWidth(UIWidth, sliderRatio);
    elsePen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(elsePen);
    y+=elsePen->getHeight();

    
    
    
    
   
    
    
    for (int i = 0; i < swarmComponents.size(); i++) {
         if (i % 2 == 1) {
        swarmComponents[i]->setBackgroundColor(ofColor(25, 47, 55));
         } else {
             swarmComponents[i]->setBackgroundColor(ofColor(35, 57, 65));
         }
    }
    for (int i = 0; i < intervalPenalties.size(); i++) {
        if (i % 2 == 1) {
        intervalPenalties[i]->setBackgroundColor(ofColor(25, 47, 55));
        } else {
            intervalPenalties[i]->setBackgroundColor(ofColor(35, 57, 65));
        }
    }
    
    for (int i = 0; i < motifComponents.size(); i++) {
        if (i % 2 == 0) {
            motifComponents[i]->setBackgroundColor(ofColor(25, 47, 55));
        } else {
            motifComponents[i]->setBackgroundColor(ofColor(35, 57, 65));
        }
        }
    
    swarmLabel->setBackgroundColor(ofColor(48, 68, 74));
}

//--------------------------------------------------------------

void SwarmGUI::updateInterface() {

    for (int i = 0; i < swarmComponents.size(); i++) {
        swarmComponents[i]->update();
    }
    
    for (int i = 0; i < intervalPenalties.size(); i++) {
        intervalPenalties[i]->update();
    }
    
    for (int i = 0; i < motifComponents.size(); i++) {
        motifComponents[i]->update();
    }
    
    
}

//--------------------------------------------------------------

void SwarmGUI::drawInterface() {
    
    
    for (int i = 0; i < motifComponents.size(); i++) {
        motifComponents[i]->draw();
    }
    
    for (int i = 0; i < swarmComponents.size(); i++) {
        swarmComponents[i]->draw();
    }
    
    for (int i = 0; i < intervalPenalties.size(); i++) {
        intervalPenalties[i]->draw();
    }
    

    
    
}

//--------------------------------------------------------------

void SwarmGUI::onToggleEvent(ofxDatGuiToggleEvent e) {


    
    //If selectMotifToggle is set to true, when clicking on the phrase list it will
    //assign the phrase to the swarm's current motif.
    if (e.target == selectMotifToggle && e.checked == true) {
        assignNewPhrase = true;
    }
    
    //Setting the selectMotifToggle to false will not set any phrase clicked on the list to the new motif.
    if (e.target == selectMotifToggle && e.checked == false) {
        assignNewPhrase = false;
    }

    
}

//--------------------------------------------------------------

void SwarmGUI::onSliderEvent(ofxDatGuiSliderEvent e) {
    

    
    ///////////////////////////////
    if (e.target == rhythmSlider) {
        
        swarm->bestRhythm.bestDimensionality = e.value;
        swarm->chosenDimension = e.value;
        resetParticleRhythms();
        
    }
    
    if (e.target == velocitySlider) {
        
        swarm->desiredVelocity = e.value;
        resetParticleVelocity();
        
    }
    
    if (e.target == octaveSlider) {
        
        
        swarm->chosenOctave = e.value;
        swarm->distMotifOctave = swarm->chosenOctave - swarm->noteMotifOctaves[0];

        resetParticleIntervals();

    }
    
    if (e.target == chordSlider) {
        
        swarm->chordPotential = e.value;

    }
    
    ///////////////////////////////
    //Interval penalties
    if (e.target == firstPen) {
        
        swarm->firstPen = ofMap(e.value, 0, 100, 100, 0);
        resetParticleIntervals();
    }
    
    if (e.target == secondPen) {
        
        swarm->secondPen = ofMap(e.value, 0, 100, 100, 0);
        resetParticleIntervals();
    }
    
    if (e.target == thirdPen) {
        
        swarm->thirdPen = ofMap(e.value, 0, 100, 100, 0);
        resetParticleIntervals();
    }
    
    if (e.target == fourthPen) {
        
        swarm->fourthPen = ofMap(e.value, 0, 100, 100, 0);
        resetParticleIntervals();
    }
    
    if (e.target == fifthPen) {
        
        swarm->fifthPen = ofMap(e.value, 0, 100, 100, 0);
        resetParticleIntervals();
    }
    
    if (e.target == sixthPen) {
        
        swarm->sixthPen = ofMap(e.value, 0, 100, 100, 0);
        resetParticleIntervals();
    }
    
    if (e.target == seventhPen) {
        
        swarm->seventhPen = ofMap(e.value, 0, 100, 100, 0);
        resetParticleIntervals();
    }
    
    if (e.target == eighthPen) {
        
        swarm->eighthPen = ofMap(e.value, 0, 100, 100, 0);
        resetParticleIntervals();
    }
    
    if (e.target == elsePen) {
        
        swarm->elsePen = ofMap(e.value, 0, 100, 100, 0);
        resetParticleIntervals();
    }
    
    
    //Motif sliders
    if (e.target == desiredNoteDistSlider) {
        swarm->desiredNoteDistance = e.value;
        swarm->targetDimensionality = e.value;
        resetParticleIntervals();
    }
    
    if (e.target == desiredRhythmDistSlider) {
        swarm->targetDimensionality = e.value;
        resetParticleRhythms();
    }
    
    if (e.target == searchIntensitySlider) {
        swarm->numOfIterations = int(e.value/2.);
    }
}


//--------------------------------------------------------------

void SwarmGUI::resetParticleIntervals() {
    
    swarm->bestFitness = 99999999;
    swarm->best.fitness = 99999999;
    
    
    for (int i = 0; i < swarm->particles.size(); i++) {
        for (int j = 0; j < 16; j++) {
            swarm->particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
            swarm->particles[i]->bestIndFreqs[j] = swarm->particles[i]->indFreqs[j];
            swarm->particles[i]->bestFit = 99999999;
        }
    }

    
    
}

//--------------------------------------------------------------

void SwarmGUI::resetParticleRhythms() {
    
    swarm->bestRhythm.bestRhythm.clear();
    swarm->bestFitnessRhythm = 9999999;
    
    for (int i = 0; i < swarm->particles.size(); i++) {
        swarm->particles[i]->dimensionalityVel = ofRandom(-vel, vel);
        swarm->particles[i]->bestFitnessRhythm = 99999999;
        swarm->particles[i]->fitnessRhythm = 99999999;
        swarm->particles[i]->bestDimensionality = swarm->particles[i]->dimensionality;
    }
    
    
}

//--------------------------------------------------------------

void SwarmGUI::resetParticleIntervalVelocity() {
    
    for (int i = 0; i < swarm->particles.size(); i++) {
        for (int j = 0; j < 16; j++) {
            swarm->particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
        }
    }
}

//--------------------------------------------------------------

void SwarmGUI::resetParticleRhythmVelocity() {
    
    for (int i = 0; i < swarm->particles.size(); i++) {
        swarm->particles[i]->dimensionalityVel = ofRandom(-vel, vel);
    }
    
}

//--------------------------------------------------------------

void SwarmGUI::resetParticleVelocity() {
    
    for (int i = 0; i < swarm->particles.size(); i++) {
        swarm->particles[i]->velocityVel = ofRandom(-2, 2);
        swarm->particles[i]->bestParticleVelocity = swarm->particles[i]->velocity;
        swarm->particles[i]->bestParticleVelocityFitness = 9999999;
    }
    
    swarm->bestVelocityFitness = 99999;
}

//--------------------------------------------------------------

void SwarmGUI::displaySwarmParameters() {
    
   /*
    string sequence = "Note sequence: " + ofToString(swarm->best.indFreqs[0]) + ", " + ofToString(swarm->best.indFreqs[1]) + ", " + ofToString(swarm->best.indFreqs[2]) + ", " + ofToString(swarm->best.indFreqs[3]) + ofToString(swarm->best.indFreqs[4]) + ", "+ ofToString(swarm->best.indFreqs[5]) + ", " + ofToString(swarm->best.indFreqs[6]) + ", " + ofToString(swarm->best.indFreqs[7]) + ", " + ofToString(swarm->best.indFreqs[8]) + ", " + ofToString(swarm->best.indFreqs[9]) + ", " + ofToString(swarm->best.indFreqs[10]) + ", " + ofToString(swarm->best.indFreqs[11]) + ", " + ofToString(swarm->best.indFreqs[12]) + ", " + ofToString(swarm->best.indFreqs[13]) + ", " + ofToString(swarm->best.indFreqs[14]) + ", " + ofToString(swarm->best.indFreqs[15]) + ", ";
    ofDrawBitmapStringHighlight(sequence, x, y);
    
    sequence = "MIDI equivalent: " + ofToString(swarm->availableNotes[swarm->best.indFreqs[0]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[1]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[2]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[3]]) + ofToString(swarm->availableNotes[swarm->best.indFreqs[4]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[5]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[6]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[7]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[8]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[9]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[10]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[11]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[12]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[13]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[14]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[15]]) + ", ";
    ofDrawBitmapStringHighlight(sequence, x, y+25);
    
    
    
    string bestFit = "Best note fitness currently: " + ofToString(swarm->bestFitness);
    ofDrawBitmapStringHighlight(bestFit, x, y+50);
    
    
    
    string rhythmSequence = "Sequence: " + ofToString(swarm->bestRhythm.rhythm);
    string rhythmBest = "Dimensionality: " + ofToString(swarm->bestRhythm.dimensionality);
    
    ofDrawBitmapStringHighlight(rhythmSequence, x, y+75);
    ofDrawBitmapStringHighlight(rhythmBest, x, y+100);
    
    string velocityText = "Velocity: " + ofToString(swarm->bestParticleSwarmVelocity);
    ofDrawBitmapStringHighlight(velocityText, x, y+150);
    
    string rhythmText = "Rhythm fitness: " + ofToString(swarm->bestFitnessRhythm);
    ofDrawBitmapStringHighlight(rhythmText, x, y+125);
    
    string currentMotif = "Current: " + ofToString(swarm->noteMotif[0]) + ", " + ofToString(swarm->noteMotif[1]) + ", " + ofToString(swarm->noteMotif[2]) + ", " + ofToString(swarm->noteMotif[3]) + ", " + ofToString(swarm->noteMotif[4]) + ", " + ofToString(swarm->noteMotif[5]) + ", " + ofToString(swarm->noteMotif[6]) + ", " + ofToString(swarm->noteMotif[7]) + ", " + ofToString(swarm->noteMotif[8]) + ", " + ofToString(swarm->noteMotif[9]) + ", " + ofToString(swarm->noteMotif[10]) + ", " + ofToString(swarm->noteMotif[11]) + ", " + ofToString(swarm->noteMotif[12]) + ", " + ofToString(swarm->noteMotif[13]) + ", " + ofToString(swarm->noteMotif[14]) + ", " + ofToString(swarm->noteMotif[15]) + ", ";
    ofDrawBitmapStringHighlight(currentMotif, x, y+200);

    

    */
}

