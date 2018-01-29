#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    gui->setAssetPath("");

    

    int x = 900;
    int y = 20;
    
    semibrevePen = new ofxDatGuiSlider(semibrevePenF.set("Semibreve Penalty", 1000, 0, 1000));
    semibrevePen->setPosition(x, y);
    semibrevePen->onSliderEvent(this, &ofApp::onSliderEvent);
    penalties.push_back(semibrevePen);
    y+=semibrevePen->getHeight();
    
    
    minimPen = new ofxDatGuiSlider(minimPenF.set("Minim Penalty", 100, 0, 1000));
    minimPen->setPosition(x, y);
    minimPen->onSliderEvent(this, &ofApp::onSliderEvent);
    penalties.push_back(minimPen);
    y+=minimPen->getHeight();
    
    crotchetPen = new ofxDatGuiSlider(crotchetPenF.set("Crotchet Penalty", 100, 0, 1000));
    crotchetPen->setPosition(x, y);
    crotchetPen->onSliderEvent(this, &ofApp::onSliderEvent);
    penalties.push_back(crotchetPen);
    y+=crotchetPen->getHeight();
    
    quaverPen = new ofxDatGuiSlider(quaverPenF.set("Quaver Penalty", 100, 0, 1000));
    quaverPen->setPosition(x, y);
    quaverPen->onSliderEvent(this, &ofApp::onSliderEvent);
    penalties.push_back(quaverPen);
    y+=quaverPen->getHeight();
    
    semiquaverPen = new ofxDatGuiSlider(semiquaverPenF.set("Semiquaver Penalty", 100, 0, 1000));
    semiquaverPen->setPosition(x, y);
    semiquaverPen->onSliderEvent(this, &ofApp::onSliderEvent);
    penalties.push_back(semiquaverPen);
    y+=semiquaverPen->getHeight();
    
    
    dimension = new ofxDatGuiSlider(dimensionI.set("Chosen Dimension", 1, 1, 16));
    dimension->setPosition(x, y);
    dimension->onSliderEvent(this, &ofApp::onSliderEvent);
    penalties.push_back(dimension);
    y+=dimension->getHeight();
    
    constrictionRate = new ofxDatGuiSlider(constrictionRateF.set("Constriction Rate", 0.8984, 0., 1.));
    constrictionRate->setPosition(x, y);
    constrictionRate->onSliderEvent(this, &ofApp::onSliderEvent);
    PSOcomponents.push_back(constrictionRate);
    y+=constrictionRate->getHeight();

    
    PSOc1 = new ofxDatGuiSlider(c1F.set("Learning factor 1", 0.2, 0., 1.));
    PSOc1->setPosition(x, y);
    PSOc1->onSliderEvent(this, &ofApp::onSliderEvent);
    PSOcomponents.push_back(PSOc1);
    y+=PSOc1->getHeight();

    
    PSOc2 = new ofxDatGuiSlider(c2F.set("Learning factor 2", 0.2, 0., 1.));
    PSOc2->setPosition(x, y);
    PSOc2->onSliderEvent(this, &ofApp::onSliderEvent);
    PSOcomponents.push_back(PSOc2);
    
    
    tempoSlider = new ofxDatGuiSlider(tempoInt.set("Tempo", 4, 1, 8));
    tempoSlider->setPosition(1000, 20);
    tempoSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    
    
    
    sampleRate = 44100;
    bufferSize = 512;
    
    semibreve.setAttack(1000);
    semibreve.setDecay(1);
    semibreve.setSustain(1);
    semibreve.setRelease(1000);
    
    
    minim.setAttack(1000);
    minim.setDecay(1);
    minim.setSustain(1);
    minim.setRelease(1000);
    
    crotchet.setAttack(1000);
    crotchet.setDecay(1);
    crotchet.setSustain(1);
    crotchet.setRelease(1000);
    
    quaver.setAttack(1000);
    quaver.setDecay(1);
    quaver.setSustain(1);
    quaver.setRelease(1000);
    
    semiquaver.setAttack(1000);
    semiquaver.setDecay(1);
    semiquaver.setSustain(1);
    semiquaver.setRelease(1000);
    
    
    for (int i = 0; i < numSwarms; i++) {
     
        PSO * pso = new PSO();
        psoSwarms.push_back(pso);
        pso->setup();
        
    }
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2,0,this, sampleRate, bufferSize, 4);
    
    ofBackground(0);

    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    for (int i = 0; i < penalties.size(); i++) {
        penalties[i]->update();
    }

    
    for (int i = 0; i < PSOcomponents.size(); i++) {
        PSOcomponents[i]->update();
    }
    
    tempoSlider->update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    for (int i = 0; i < penalties.size(); i++) {
        penalties[i]->draw();
    }
    
    for (int i = 0; i < PSOcomponents.size(); i++) {
        PSOcomponents[i]->draw();
    }
    
    
    if (changeRhythm == true) {
    psoSwarms[0]->run();
        changeRhythm = false;
    }
    

    tempoSlider->draw();
    
    psoSwarms[0]->display();

    string sequence = "Sequence: " + ofToString(psoSwarms[0]->best->rhythm);
   
    ofDrawBitmapStringHighlight(sequence, 20, 200);
    
    
    if (ofGetFrameNum() > 60) {
    string sequence1 = "Sequence: " + ofToString(psoSwarms[0]->particles[0]->rhythm);
    string sequence2 = "Sequence: " + ofToString(psoSwarms[0]->particles[1]->rhythm);
    string sequence3 = "Sequence: " + ofToString(psoSwarms[0]->particles[2]->rhythm);

    ofDrawBitmapStringHighlight(sequence1, 20, 250);
    ofDrawBitmapStringHighlight(sequence2, 20, 300);
    ofDrawBitmapStringHighlight(sequence3, 20, 350);
    }
    

}


//--------------------------------------------------------------

void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    
    //playSound determined by Play button.

   // if (play == true) {
        for (unsigned i = 0; i < bufferSize; i++) {
            
            //Phasor controls bpm - 8 hertz = 480 bpm
            currentCount = (int)timer.phasor(tempo);
            
            //Check if count has changed
            if (lastCount!=currentCount) {
                
                if (playHead % 16 == 0) {
                    changeRhythm = true;
                }
                //Determining which envelope to use (for duration)
                if (psoSwarms[0]->best->hits[playHead%16] == 1) {
                    
                    float duration= psoSwarms[0]->best->rhythm[playHeadR%psoSwarms[0]->best->rhythm.size()];
                    
                    if (duration == 4) {
                       // cout << "env 4" << endl;
                        chosen = semibreve;
                    } else if (duration == 2) {
//                        cout << "env 2" << endl;

                        chosen = minim;
                    } else if (duration == 1) {
                        //cout << "env 1" << endl;

                        chosen = crotchet;
                    } else if (duration == 0.5) {
                       // cout << "env 0.5" << endl;

                        chosen = quaver;
                    } else if (duration == 0.25) {
//                        cout << "env 0.25" << endl;

                        chosen = semiquaver;
                    }
                    
                    playHeadR++;
                    chosen.trigger = psoSwarms[0]->best->hits[playHead%16];

                }
                
                
           
                playHead++;
                lastCount = 0;
            }
            
            
            
            double out = chosen.adsr(1., chosen.trigger)*osc.sinewave(440);
            
            output[i*nChannels] = out;
            output[i*nChannels+1] = output[i*nChannels];
            
            chosen.trigger = 0;
            
            
        }
        
   // }
    
}
//--------------------------------------------------------------
//Handler for sliders
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    
    for (int i = 0; i < psoSwarms[0]->particles.size(); i++) {
        psoSwarms[0]->particles[i]->dimensionalityVel = ofRandom(-2, 2);
    }
    
    
    //PSO sliders
    //Changing number of PSO swarms
    if (e.target == semibrevePen) {
        psoSwarms[0]->bestFitness = 2000000;
        psoSwarms[0]->best->bestRhythm.clear();
        psoSwarms[0]->semibrevePen = e.value;
        
    }
    
    if (e.target == minimPen) {
        
        psoSwarms[0]->bestFitness = 2000000;

        psoSwarms[0]->best->bestRhythm.clear();

        psoSwarms[0]->minimPen = e.value;
        
    }
    
    if (e.target == crotchetPen) {
        
        psoSwarms[0]->bestFitness = 2000000;

        psoSwarms[0]->best->bestRhythm.clear();

        psoSwarms[0]->crotchetPen = e.value;
        
    }
    
    if (e.target == quaverPen) {
        
        psoSwarms[0]->bestFitness = 2000000;

        psoSwarms[0]->best->bestRhythm.clear();

        psoSwarms[0]->quaverPen = e.value;
        
    }
    
    if (e.target == semiquaverPen) {
        
        psoSwarms[0]->bestFitness = 2000000;

        psoSwarms[0]->best->bestRhythm.clear();

        psoSwarms[0]->semiquaverPen = e.value;
        
    }
    
    if (e.target == dimension) {
        psoSwarms[0]->bestFitness = 2000000;
        
        psoSwarms[0]->best->bestRhythm.clear();
        
        psoSwarms[0]->chosenDimension = e.value;
    }
    
    if (e.target == constrictionRate) {
        psoSwarms[0]->con = e.value;
    }
    
    if (e.target == PSOc1) {
        psoSwarms[0]->c1 = e.value;
    }
    
    if (e.target == PSOc2) {
        psoSwarms[0]->c2 = e.value;
    }
    
    if (e.target == tempoSlider) {
        tempo = e.value;
    }

}

    

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    //play = true;
    psoSwarms[0]->run();
    playHead = 0;
    playHeadR = 0;
    cout << "Best: " << endl;
    cout << psoSwarms[0]->best->dimensionality << endl;


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

   // changeRhythm();
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
