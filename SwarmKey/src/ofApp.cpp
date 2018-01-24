#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    changeRhythm();


    sampleRate = 44100;
    bufferSize = 512;
    
    semibreve.setAttack(500);
    semibreve.setDecay(1);
    semibreve.setSustain(4);
    semibreve.setRelease(1000);
    
    
    minim.setAttack(500);
    minim.setDecay(1);
    minim.setSustain(2);
    minim.setRelease(1000);
    
    crotchet.setAttack(500);
    crotchet.setDecay(1);
    crotchet.setSustain(1);
    crotchet.setRelease(1000);
    
    quaver.setAttack(500);
    quaver.setDecay(1);
    quaver.setSustain(0.5);
    quaver.setRelease(1000);
    
    semiquaver.setAttack(500);
    semiquaver.setDecay(1);
    semiquaver.setSustain(0.25);
    semiquaver.setRelease(1000);
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2,0,this, sampleRate, bufferSize, 4);

    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}


void ofApp::changeRhythm() {
    
    
    rhythm.clear();
    hits.clear();
    int r = int(ofRandom(0, 5));
    
    
    rhythm.push_back(validDurations[r]);
    cout << r << endl;
    cout << rhythm[0] << endl;
    
    float sum = rhythm[0];
    
    while (sum != 4) {
    
        cout << "Current sum: " << sum << endl;
        
        if (sum >= 3.75) {
            
            r = 4;
            rhythm.push_back(validDurations[r]);
            cout << "3.75 picked" << endl;
            
            
        } else if (sum > 3 && sum <= 3.5) {
            
            r = int(ofRandom(3, 5));
            rhythm.push_back(validDurations[r]);
            cout << "3.5 picked" << endl;

            
        } else if (sum > 2 && sum <= 3) {
            
            r = int(ofRandom(2, 5));
            rhythm.push_back(validDurations[r]);
            cout << "3 picked" << endl;

            
            
        } else if (sum > 0 && sum <= 2) {
            
            r = int(ofRandom(1, 5));
            rhythm.push_back(validDurations[r]);
            cout << "2 picked" << endl;

            
            
        }
        
        cout << "Random: " << r << endl;
        cout << "Chosen duration: " << validDurations[r] << endl;
        cout << " " << endl;
        
        sum = 0;
        for (int i = 0; i < rhythm.size(); i++) {
            
            sum+=rhythm[i];
            
        }
        
        
    }
    
    for (int i = 0; i < rhythm.size(); i++) {
        
        cout << rhythm[i] << ", " ;
        
        if (rhythm[i] == 0.25) {
            hits.push_back(1);
        } else if (rhythm[i] == 0.5) {
            hits.push_back(1);
            hits.push_back(0);
        } else if (rhythm[i] == 1) {
            hits.push_back(1);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
        } else if (rhythm[i] == 2) {
            hits.push_back(1);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            
            
        } else if (rhythm[i] == 4) {
            
            hits.push_back(1);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            hits.push_back(0);
            
        }
    }

    
}
//--------------------------------------------------------------

void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    
    //playSound determined by Play button.

        for (unsigned i = 0; i < bufferSize; i++) {
            
            //Phasor controls bpm - 8 hertz = 480 bpm
            currentCount = (int)timer.phasor(8);
            
            //Check if count has changed
            if (lastCount!=currentCount) {
                
                
                //Determining which envelope to use (for duration)
                if (hits[playHead%hits.size()] == 1) {
                    
                    int duration= playHeadR%rhythm.size();
                    
                    if (duration == 4) {
                        chosen = semibreve;
                    } else if (duration == 2) {
                        chosen = minim;
                    } else if (duration == 1) {
                        chosen = crotchet;
                    } else if (duration == 0.5) {
                        chosen = quaver;
                    } else if (duration == 0.25) {
                        chosen = semiquaver;
                    }
                    
                    
                }
                chosen.trigger = hits[playHead%hits.size()];
                
                
                //env.setSustain(rhythm[playHeadR%rhythm.size()]);
                
                if (chosen.trigger == 1) {
                    
                    playHeadR++;
                }
                playHead++;
                lastCount = 0;
            }
            
            
            
            double out = chosen.adsr(1., chosen.trigger)*osc.sinewave(440);
            
            output[i*nChannels] = out;
            output[i*nChannels+1] = output[i*nChannels];
            
            chosen.trigger = 0;
            
            
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

    changeRhythm();
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
