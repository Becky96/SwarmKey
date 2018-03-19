//
//  PianoRoll.cpp
//  abletonDemo
//
//  Created by Becky Johnson on 18/03/2018.
//
//

#include "PianoRoll.hpp"


void PianoRoll::setup(int _x, int _y) {
    
    containerX = _x;
    containerY = _y;
    
    piano.load("piano.png");
    
    cout << piano.getHeight() << endl;
    
    
    
 
    
    
    
    //Loading width of grid, (different sequence numbers, same height numbers);
    for (int i = 0; i < 16; i++) {
        
    //Loading height of grid (different midi numbers, same sequence num)
    for (int j = 0; j < 48; j++) {
        

        
    
            
            PianoRollCell  prc;
            prc.setup((containerX+piano.getWidth()+5) + (i * 35), containerY + 2 + (j*14), MIDINum, sequenceNum);
            
            pianoRollCells.push_back(prc);
        
        MIDINum--;
            
            
        }
        
        sequenceNum++;
        
        MIDINum = 83;
    }

}

void PianoRoll::displayRoll() {
    
    ofSetColor(255);
    //ofDrawRectangle(700, 100, 800, 800);

    piano.draw(containerX, containerY);
    
    for (int i = 0; i < pianoRollCells.size(); i++) {
        
        pianoRollCells[i].displayCell();
    }
    
    
    
    /*
    for (int i = 0; i < 17; i++) {
        
        
        if (i % 4 == 0) {
            ofSetColor(0);
            
        } else {
            ofSetColor(100);
        }
        
        ofDrawLine( (containerX+piano.getWidth()+5) + i * 35, containerY+0, (containerX+piano.getWidth()+5) + i * 35, containerY+piano.getHeight());

    }
    
    for (int i = 0; i < 49; i++) {
        
        ofDrawLine(containerX+piano.getWidth()-10,   containerY + 2 + (i*14),        containerX+685, containerY + 2 + (i*14));
        
    }
    */
    
    
    
    
    //for (int i = 0; i < )
}

void PianoRoll::resetCells(Swarm * swarm1, Swarm * swarm2) {
    int playHead = 0;
    cout << "REACHED 1 1" << endl;

    for (int i = 0; i < pianoRollCells.size(); i++) {
        pianoRollCells[i].highlighted = false;
    }
    
    for (int i = 0; i < swarm1->bestRhythm.hits.size(); i++) {
        
        if (swarm1->bestRhythm.hits[i]%16 == 1) {
            cout << "found a hit at sequence num: " << i << endl;
            
            int start = (i*48);
            int end = ((i+1)*48);
            for (int j = start; j < end; j++) {
                
                if (i == pianoRollCells[j].sequenceNum && swarm1->availableNotes[swarm1->best.indFreqs[playHead%4]] == pianoRollCells[j].MIDINum) {
                    
                    pianoRollCells[j].highlighted = true;
                    cout << "playHead: " << playHead << endl;
                    cout << "Found cell for hit number: " << i << "at cell sequence number: " << pianoRollCells[j].sequenceNum << "with MIDI note: " << pianoRollCells[j].MIDINum << endl;
                    
                    cout << "REACHED" << endl;
                } else {
                    pianoRollCells[j].highlighted = false;
                }
            }
            
            playHead++;
        }
    }
    
    
}





