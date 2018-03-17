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
    
    //Algorithm component sliders
    /*
    noteConSlider = new ofxDatGuiSlider(noteConFloat.set("Note Constriction Rate", 0.7984, 0., 1.));
    noteConSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    noteConSlider->setPosition(x, y);
    algorithmComponents.push_back(noteConSlider);
    y+=noteConSlider->getHeight();
    
    noteC1Slider = new ofxDatGuiSlider(noteC1Float.set("Note C1 Value", 1.4, 0., 4.));
    noteC1Slider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    noteC1Slider->setPosition(x, y);
    algorithmComponents.push_back(noteC1Slider);
    y+=noteC1Slider->getHeight();
    
    noteC2Slider = new ofxDatGuiSlider(noteC2Float.set("Note C2 Value", 1.4, 0., 4.));
    noteC2Slider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    noteC2Slider->setPosition(x, y);
    algorithmComponents.push_back(noteC2Slider);
    y+=noteC2Slider->getHeight();
    
    rhythmConSlider = new ofxDatGuiSlider(rhythmConFloat.set("Rhythm Constriction Rate", 0.4, 0., 1.));
    rhythmConSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    rhythmConSlider->setPosition(x, y);
    algorithmComponents.push_back(rhythmConSlider);
    y+=rhythmConSlider->getHeight();
    
    rhythmC1Slider = new ofxDatGuiSlider(rhythmC1Float.set("Rhythm C1 Value", 1.4, 0., 4.));
    rhythmC1Slider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    rhythmC1Slider->setPosition(x, y);
    algorithmComponents.push_back(rhythmC1Slider);
    y+=rhythmC1Slider->getHeight();
    
    rhythmC2Slider = new ofxDatGuiSlider(rhythmC2Float.set("Rhythm C2 Value", 1.4, 0., 4.));
    rhythmC2Slider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    rhythmC2Slider->setPosition(x, y);
    algorithmComponents.push_back(rhythmC2Slider);
    y+=rhythmC2Slider->getHeight();
    */
    
    //GUI setup of toggle and sliders.
    playingToggle = new ofxDatGuiToggle("Playing", false);
    playingToggle->onToggleEvent(this, &SwarmGUI::onToggleEvent);
    playingToggle->setPosition(x, y);
    swarmComponents.push_back(playingToggle);
    y+=playingToggle->getHeight();
    
    
    rhythmSlider = new ofxDatGuiSlider(rhythmInt.set("Rhythm Speed", 4, 1, 16));
    rhythmSlider->setPosition(x, y);
    rhythmSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    swarmComponents.push_back(rhythmSlider);
    y+=rhythmSlider->getHeight();
    
    velocitySlider = new ofxDatGuiSlider(velocityInt.set("Velocity Level", 100, 0, 125));
    velocitySlider->setPosition(x, y);
    velocitySlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    swarmComponents.push_back(velocitySlider);
    y+=velocitySlider->getHeight();
    
    octaveSlider = new ofxDatGuiSlider(octaveInt.set("Octave Level", 4, 1, 8));
    octaveSlider->setPosition(x, y);
    octaveSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    swarmComponents.push_back(octaveSlider);
    y+=octaveSlider->getHeight();
    
    chordSlider = new ofxDatGuiSlider(chordInt.set("Chord potential", 0, 0, 3));
    chordSlider->setPosition(x, y);
    chordSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    swarmComponents.push_back(chordSlider);
    y+=chordSlider->getHeight();
    
    //Interval penalties for specific swarm
    //First
    firstPen = new ofxDatGuiSlider(firstInt.set("Interval of 1", 100, 0, 10000));
    firstPen->setPosition(x, y);
    firstPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(firstPen);
    y+=firstPen->getHeight();
    
    //Second
    secondPen = new ofxDatGuiSlider(secondInt.set("Interval of 2", 10000, 0, 10000));
    secondPen->setPosition(x, y);
    secondPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(secondPen);
    y+=secondPen->getHeight();
    
    thirdPen = new ofxDatGuiSlider(thirdInt.set("Interval of 3", 100, 0, 10000));
    thirdPen->setPosition(x, y);
    thirdPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(thirdPen);
    y+=thirdPen->getHeight();
    
    fourthPen = new ofxDatGuiSlider(fourthInt.set("Interval of 4", 1000, 0, 10000));
    fourthPen->setPosition(x, y);
    fourthPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(fourthPen);
    y+=fourthPen->getHeight();
    
    fifthPen = new ofxDatGuiSlider(fifthInt.set("Interval of 5", 100, 0, 10000));
    fifthPen->setPosition(x, y);
    fifthPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(fifthPen);
    y+=fifthPen->getHeight();
    
    sixthPen = new ofxDatGuiSlider(sixthInt.set("Interval of 6", 1000, 0, 10000));
    sixthPen->setPosition(x, y);
    sixthPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(sixthPen);
    y+=sixthPen->getHeight();
    
    seventhPen = new ofxDatGuiSlider(seventhInt.set("Interval of 7", 10000, 0, 10000));
    seventhPen->setPosition(x, y);
    seventhPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(seventhPen);
    y+=seventhPen->getHeight();
    
    eighthPen = new ofxDatGuiSlider(eighthInt.set("Interval of 8", 100, 0, 10000));
    eighthPen->setPosition(x, y);
    eighthPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(eighthPen);
    y+=eighthPen->getHeight();
    
    elsePen = new ofxDatGuiSlider(elseInt.set("Intervals 9 or above", 10000, 0, 10000));
    elsePen->setPosition(x, y);
    elsePen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(elsePen);
    y+=elsePen->getHeight();

    y+=20;
    
    
    desiredNoteDistSlider = new ofxDatGuiSlider(desiredNoteDistInt.set("Desired note dist", 0, 0, 200));
    desiredNoteDistSlider->setPosition(x, y);
    desiredNoteDistSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    motifComponents.push_back(desiredNoteDistSlider);
    y+=desiredNoteDistSlider->getHeight();
    
    desiredRhythmDistSlider = new ofxDatGuiSlider(desiredRhythmDistInt.set("Desired rhythm", swarm->dimensionalityMotif, 0, 16));
    desiredRhythmDistSlider->setPosition(x, y);
    desiredRhythmDistSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    motifComponents.push_back(desiredRhythmDistSlider);
    y+=desiredRhythmDistSlider->getHeight();
    
    saveNewMotif = new ofxDatGuiButton("Save previous bar as new motif");
    saveNewMotif->setPosition(x, y);
    saveNewMotif->onButtonEvent(this, &SwarmGUI::onButtonEvent);
    motifComponents.push_back(saveNewMotif);
    y+=saveNewMotif->getHeight();
    
    restoreOriginalMotif = new ofxDatGuiButton("Restore original motif");
    restoreOriginalMotif->setPosition(x, y);
    restoreOriginalMotif->onButtonEvent(this, &SwarmGUI::onButtonEvent);
    motifComponents.push_back(restoreOriginalMotif);
    y+=restoreOriginalMotif->getHeight();
    
    
    
    
}

//--------------------------------------------------------------

void SwarmGUI::updateInterface() {
    
    for (int i = 0; i < algorithmComponents.size(); i++) {
        //algorithmComponents[i]->update();
    }
    
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
    
    for (int i = 0; i < algorithmComponents.size(); i++) {
        //algorithmComponents[i]->draw();
    }
    
    for (int i = 0; i < swarmComponents.size(); i++) {
        swarmComponents[i]->draw();
    }
    
    for (int i = 0; i < intervalPenalties.size(); i++) {
        intervalPenalties[i]->draw();
    }
    
    for (int i = 0; i < motifComponents.size(); i++) {
        motifComponents[i]->draw();
    }
    
    
}

//--------------------------------------------------------------

void SwarmGUI::onToggleEvent(ofxDatGuiToggleEvent e) {

    //Set swarm to play.
    if (e.target == playingToggle && e.checked == false) {
        
        swarm->readyToPlay = true;
        swarm->notePlayhead = 0;
        cout << swarm->play << endl;
    }
    
    //Set swarm to stop playing.
    if (e.target == playingToggle && e.checked == true) {
        swarm->play = false;
        cout << swarm->play << endl;

    }
    

    
}

//--------------------------------------------------------------

void SwarmGUI::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    ///////////////////////////////
    //Algorithm components
    //Intervals
    if (e.target == noteConSlider) {
        swarm->noteCon = e.value;
        resetParticleIntervalVelocity();
        
    }
    
    if (e.target == noteC1Slider) {
        swarm->noteC1 = e.value;
        resetParticleIntervalVelocity();
    }
    
    if (e.target == noteC2Slider) {
        swarm->noteC2 = e.value;
        resetParticleIntervalVelocity();
    }
    
    //Rhythm
    if (e.target == rhythmConSlider) {
        swarm->rhythmCon = e.value;
        resetParticleRhythmVelocity();
    }
    
    if (e.target == rhythmC1Slider) {
        swarm->rhythmC1 = e.value;
        resetParticleRhythmVelocity();
    }
    
    if (e.target == rhythmC2Slider) {
        swarm->rhythmC2 = e.value;
        resetParticleRhythmVelocity();
    }
    
    
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
        
        if (e.value > swarm->chordPotential) {
            swarm->randomiseParticleChord(e.value);
        }
        
        if (e.value < swarm->chordPotential) {
            swarm->removeParticleChordIndex();
        }
        swarm->prevChordPotential = swarm->chordPotential;
        swarm->chordPotential = e.value;

    }
    
    ///////////////////////////////
    //Interval penalties
    if (e.target == firstPen) {
        
        swarm->firstPen = e.value;
        resetParticleIntervals();
    }
    
    if (e.target == secondPen) {
        
        swarm->secondPen = e.value;
        resetParticleIntervals();
    }
    
    if (e.target == thirdPen) {
        
        swarm->thirdPen = e.value;
        resetParticleIntervals();
    }
    
    if (e.target == fourthPen) {
        
        swarm->fourthPen = e.value;
        resetParticleIntervals();
    }
    
    if (e.target == fifthPen) {
        
        swarm->fifthPen = e.value;
        resetParticleIntervals();
    }
    
    if (e.target == sixthPen) {
        
        swarm->sixthPen = e.value;
        resetParticleIntervals();
    }
    
    if (e.target == seventhPen) {
        
        swarm->seventhPen = e.value;
        resetParticleIntervals();
    }
    
    if (e.target == eighthPen) {
        
        swarm->eighthPen = e.value;
        resetParticleIntervals();
    }
    
    if (e.target == elsePen) {
        
        swarm->elsePen = e.value;
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
      //  swarm->desiredRhythmDistance = e.value;
        resetParticleRhythms();
    }
    
    
}


//--------------------------------------------------------------

void SwarmGUI::onButtonEvent(ofxDatGuiButtonEvent e) {
    
    if (e.target == saveNewMotif) {
        
        for (int i = 0; i < 4; i++) {
            swarm->noteMotif[i] = swarm->best.indFreqs[i];
        }
    }
    
    if (e.target == restoreOriginalMotif) {
        
        for (int i = 0; i < 4; i++) {
            swarm->noteMotif[i] = swarm->originalMotif[i];
        }
    }
}
//--------------------------------------------------------------

void SwarmGUI::resetParticleIntervals() {
    
    swarm->bestFitness = 99999999;
    swarm->best.fitness = 99999999;
    
    
    for (int i = 0; i < swarm->particles.size(); i++) {
        for (int j = 0; j < 4; j++) {
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
        for (int j = 0; j < 4; j++) {
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
        swarm->particles[i]->velocityVel = ofRandom(-0.75, 0.75);
        swarm->particles[i]->bestParticleVelocity = swarm->particles[i]->velocity;
        swarm->particles[i]->bestParticleVelocityFitness = 9999;
    }
    
    swarm->bestVelocityFitness = 99999;
}

//--------------------------------------------------------------

void SwarmGUI::displaySwarmParameters() {
    string sequence = "Note sequence: " + ofToString(swarm->best.indFreqs[0]) + ", " + ofToString(swarm->best.indFreqs[1]) + ", " + ofToString(swarm->best.indFreqs[2]) + ", " + ofToString(swarm->best.indFreqs[3]);
    ofDrawBitmapStringHighlight(sequence, x, y);
    
    sequence = "MIDI equivalent: " + ofToString(swarm->availableNotes[swarm->best.indFreqs[0]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[1]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[2]]) + ", " + ofToString(swarm->availableNotes[swarm->best.indFreqs[3]]);
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
    
    string originalMotif = "Original: " + ofToString(swarm->originalMotif[0]) + ", " + ofToString(swarm->originalMotif[1]) + ", " + ofToString(swarm->originalMotif[2]) + ", " + ofToString(swarm->originalMotif[3]);
    string currentMotif = "Current: " + ofToString(swarm->noteMotif[0]) + ", " + ofToString(swarm->noteMotif[1]) + ", " + ofToString(swarm->noteMotif[2]) + ", " + ofToString(swarm->noteMotif[3]);
    ofDrawBitmapStringHighlight(originalMotif, x, y+175);
    ofDrawBitmapStringHighlight(currentMotif, x, y+200);

    

    
}

