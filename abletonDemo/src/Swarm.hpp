//
//  Swarm.hpp
//  abletonDemo
//
//  Created by Becky Johnson on 29/01/2018.
//
//

#ifndef Swarm_hpp
#define Swarm_hpp

#include <stdio.h>
#include "ofxMidi.h"
#include "ofxMaxim.h"

class Swarm {
    
    
public:
    
    void openVirtualPort(string portName);
    
    void setup(int _channel);
    
    void exit();
    
    void play();
    
    
    ofxMidiOut midiOut;
    
    int channel, note, velocity;
    
    int tempo = 120;
    
    
};

#endif /* Swarm_hpp */
