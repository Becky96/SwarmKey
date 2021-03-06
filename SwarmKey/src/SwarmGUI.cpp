//  SWARM KEY
//  SwarmGUI.cpp

#include "SwarmGUI.hpp"

//--------------------------------------------------------------
/*Function primarily sets up the UI components for the individual swarms.*/
void SwarmGUI::setupInterface() {
    
    //Y position of the UI components.
    textY = y;
    
    //Swarm label states whether this UI is for the Left or Right swarm.
    swarmLabel = new ofxDatGuiLabel(label);
    swarmLabel->setPosition(x, y);
    swarmLabel->setWidth(UIWidth);
    swarmLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    swarmComponents.push_back(swarmLabel);
    y+=swarmLabel->getHeight();
    
    //selectMotifToggle allows user to specify a phrase from the Phrase List to act as the target phrase.
    selectMotifToggle = new ofxDatGuiToggle("Select phrase from list", false);
    selectMotifToggle->onToggleEvent(this, &SwarmGUI::onToggleEvent);
    selectMotifToggle->setPosition(x, y);
    selectMotifToggle->setWidth(UIWidth);
    selectMotifToggle->setChecked(false);
    motifComponents.push_back(selectMotifToggle);
    y+=selectMotifToggle->getHeight();

    //Displays the currently selected phrase.
    currentMotifLabel = new ofxDatGuiLabel("");
    currentMotifLabel->setPosition(x, y);
    currentMotifLabel->setWidth(UIWidth);
    motifComponents.push_back(currentMotifLabel);
    y+=currentMotifLabel->getHeight();
    
    //Defines the distance the user would like from the currently selected target phrase.
    desiredNoteDistSlider = new ofxDatGuiSlider(desiredNoteDistInt.set("Phrase distance", 0, 0, 100));
    desiredNoteDistSlider->setPosition(x, y);
    desiredNoteDistSlider->setWidth(UIWidth, sliderRatio);
    desiredNoteDistSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    motifComponents.push_back(desiredNoteDistSlider);
    y+=desiredNoteDistSlider->getHeight();
    
    //Search intensity defines how quickly the user would like the swarm to reach the target phrase with the specified distance. A higher value means the swarm as a higher chance at finding the phrase at a quicker rate.
    searchIntensitySlider = new ofxDatGuiSlider(searchIntensityInt.set("Search intensity", 1, 1, 100));
    searchIntensitySlider->setPosition(x, y);
    searchIntensitySlider->setWidth(UIWidth, sliderRatio);
    searchIntensitySlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    motifComponents.push_back(searchIntensitySlider);
    y+=searchIntensitySlider->getHeight();
    
    y+=20;
    
    //Defines the desired "speed" of the rhythm.
    desiredRhythmDistSlider = new ofxDatGuiSlider(desiredRhythmDistInt.set("Rhythm speed", 4, 1, 16));
    desiredRhythmDistSlider->setPosition(x, y);
    desiredRhythmDistSlider->setWidth(UIWidth, sliderRatio);
    desiredRhythmDistSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    motifComponents.push_back(desiredRhythmDistSlider);
    y+=desiredRhythmDistSlider->getHeight();
    
    //Defines the desired velocity.
    velocitySlider = new ofxDatGuiSlider(velocityInt.set("Velocity Level", maxVelocity/2., 0, maxVelocity));
    velocitySlider->setPosition(x, y);
    velocitySlider->setWidth(UIWidth, sliderRatio);
    velocitySlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    swarmComponents.push_back(velocitySlider);
    y+=velocitySlider->getHeight();
    
    //Defines the desired octave level.
    octaveSlider = new ofxDatGuiSlider(octaveInt.set("Octave Level", 4, 2, 6));
    octaveSlider->setPosition(x, y);
    octaveSlider->setWidth(UIWidth, sliderRatio);
    octaveSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    swarmComponents.push_back(octaveSlider);
    y+=octaveSlider->getHeight();
    
    //Defines the probability of the swarm generating chords.
    chordSlider = new ofxDatGuiSlider(chordInt.set("Chord potential", 0, 0, 100));
    chordSlider->setPosition(x, y);
    chordSlider->setWidth(UIWidth, sliderRatio);
    chordSlider->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    swarmComponents.push_back(chordSlider);
    y+=chordSlider->getHeight();
    
    y+=20;
    
    //Interval penalties for individual swarm
    firstPen = new ofxDatGuiSlider(firstInt.set("Interval of 1", 90, 0, 100));
    firstPen->setPosition(x, y);
    firstPen->setWidth(UIWidth, sliderRatio);
    firstPen->onSliderEvent(this, &SwarmGUI::onSliderEvent);
    intervalPenalties.push_back(firstPen);
    y+=firstPen->getHeight();
    
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

    //Setting background colours for UI components. Modulo % 2 is used to alternate each UI component as a different colour,
    for (int i = 0; i < swarmComponents.size(); i++) {
         if (i % 2 == 1) {
        swarmComponents[i]->setBackgroundColor(ofColor(25, 47, 55));
         } else {
             swarmComponents[i]->setBackgroundColor(ofColor(35, 57, 65));
         }
    }
    
    //Setting background colours for UI components. Modulo % 2 is used to alternate each UI component as a different colour,
    for (int i = 0; i < intervalPenalties.size(); i++) {
        if (i % 2 == 1) {
        intervalPenalties[i]->setBackgroundColor(ofColor(25, 47, 55));
        } else {
            intervalPenalties[i]->setBackgroundColor(ofColor(35, 57, 65));
        }
    }
    
    //Setting background colours for UI components. Modulo % 2 is used to alternate each UI component as a different colour,
    for (int i = 0; i < motifComponents.size(); i++) {
        if (i % 2 == 0) {
            motifComponents[i]->setBackgroundColor(ofColor(25, 47, 55));
        } else {
            motifComponents[i]->setBackgroundColor(ofColor(35, 57, 65));
        }
    }
    
    //Below UI components have different background colours for emphasis.
    swarmLabel->setBackgroundColor(ofColor(48, 68, 74));
    selectMotifToggle->setBackgroundColor(ofColor(91, 125, 123));
    currentMotifLabel->setBackgroundColor(ofColor(91, 125, 123));
}

//--------------------------------------------------------------
/*Updates UI components.*/
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
/*Displays UI components.*/
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
/*Function responds to UI toggle components.*/
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
/*Function responds to UI slider components.*/
void SwarmGUI::onSliderEvent(ofxDatGuiSliderEvent e) {
    

    if (e.target == rhythmSlider) {
        swarm->chosenDimension = e.value;
        resetParticleRhythms();
    }
    
    //If slider is 'velocitySlider', set new desiredVelocity and reset particle velocitys.
    if (e.target == velocitySlider) {
        swarm->desiredVelocity = e.value;
        resetParticleVelocity();
    }
    
    //If slider is 'octaveSlider', set new desired octave and calculate the distance from the desired octave and the octave of the target phrase. Reset all particle note sequence velocities to provoke particles to find new solutions.
    if (e.target == octaveSlider) {
        swarm->chosenOctave = e.value;
        swarm->distMotifOctave = swarm->chosenOctave - swarm->noteMotifOctaves[0];
        resetParticleIntervals();
    }
    
    //Chord potential is used in ofApp to determine the likelihood of generating chords.
    if (e.target == chordSlider) {
        swarm->chordPotential = e.value;
    }
    
    //Interval likelihood sliders
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
    //DesiredNoteSlider defines the distance the user would like from the target phrase. Reset all particle note sequence velocities to provoke particles to discover new solutions.
    if (e.target == desiredNoteDistSlider) {
        swarm->desiredNoteDistance = int(e.value/2);
        resetParticleIntervals();
    }
    
    //If slider is 'desiredRhythmDistSlider', set the new best rhythm dimensionality as e.value to propel particles towards this value.
    //Reset particle's rhythms and velocitys.
    if (e.target == desiredRhythmDistSlider) {
        swarm->targetDimensionality = e.value;
        resetParticleRhythms();
    }
    
    //NumOfIterations is defined by this slider, and controls how many times to perform the PSO algorithmic process before returning the best particle note sequences, rhythm sequence, and velocity for MIDI output.
    if (e.target == searchIntensitySlider) {
        swarm->numOfIterations = int(e.value)*2;
    }
    
}


//--------------------------------------------------------------
/*Resets algorithm components for note sequences.*/
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
/*Resets algorithm components for rhythm sequences.*/
void SwarmGUI::resetParticleRhythms() {
    
    swarm->bestRhythm.bestRhythm.clear();
    swarm->bestRhythm.hits.clear();

    swarm->bestFitnessRhythm = 9999999;
    
    for (int i = 0; i < swarm->particles.size(); i++) {
        swarm->particles[i]->dimensionalityVel = ofRandom(-vel, vel);
        swarm->particles[i]->bestFitnessRhythm = 99999999;
        swarm->particles[i]->fitnessRhythm = 99999999;
        swarm->particles[i]->bestDimensionality = swarm->particles[i]->dimensionality;
    }
    
}

//--------------------------------------------------------------
/*Resets algorithm components for note sequence velocitys.*/
void SwarmGUI::resetParticleIntervalVelocity() {
    
    for (int i = 0; i < swarm->particles.size(); i++) {
        for (int j = 0; j < 16; j++) {
            swarm->particles[i]->indFreqsVel[j] = ofRandom(-vel, vel);
        }
    }
    
}

//--------------------------------------------------------------
/*Resets algorithm components for rhythm sequence velocitys.*/
void SwarmGUI::resetParticleRhythmVelocity() {
    
    for (int i = 0; i < swarm->particles.size(); i++) {
        swarm->particles[i]->dimensionalityVel = ofRandom(-vel, vel);
    }
    
}

//--------------------------------------------------------------
/*Resets algorithm components for velocity solutions.*/
void SwarmGUI::resetParticleVelocity() {
    
    for (int i = 0; i < swarm->particles.size(); i++) {
        swarm->particles[i]->velocityVel = ofRandom(-2, 2);
        swarm->particles[i]->bestParticleVelocity = swarm->particles[i]->velocity;
        swarm->particles[i]->bestParticleVelocityFitness = 9999999;
    }
    swarm->bestVelocityFitness = 999999;
    
}

//--------------------------------------------------------------

