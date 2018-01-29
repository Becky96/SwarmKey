//
//  Swarm.cpp
//  abletonDemo
//
//  Created by Becky Johnson on 29/01/2018.
//
//

#include "Swarm.hpp"

void Swarm::openVirtualPort(string portName) {
    
    
    midiOut.listPorts();
    ofxMidiOut::listPorts();
    
    
    midiOut.openVirtualPort(portName);
    
}

void Swarm::setup(int _channel) {
    
    channel = _channel;
    
    note = 1;
    velocity = 120;
    
}


void Swarm::exit() {
    
    midiOut.closePort();
}


void Swarm::play() {
    
    note = 60 + channel;
    midiOut.sendNoteOn(channel, note, velocity);
    
}