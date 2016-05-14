//
//  visualSystem.cpp
//  JRHMemorialWall
//
//  Created by Jack Kalish on 6/25/14.
//
//

#include "displaySystem.h"


displaySystem::displaySystem(){
    
    width = height = 0;
    numRodsOuter = 36;
    numRodsInner = 9;
}

void displaySystem::init(int w, int h, int numRods, int rodSpacing, int rodMargins){
    test = false;
    //at first you must specify the Ip address of this machine
    artnet.setup("192.168.0.1"); //make sure the firewall is deactivated at this point
    artnet.verbose = false;
    width = w;
    height = h;
    
    _numRods = numRods;
    _rodSpacing = rodSpacing;
    _rodMargins = rodMargins;
    numRodsOuter = numRods;
    
    strip.allocate(1, height, GL_RGB);
    stripImage.allocate(1, height, OF_IMAGE_COLOR);
    
    //clear at start
    clear();
}

//update the display with a new image
void displaySystem::updateDisplay(ofFbo * frame){
    if(!test){
        _frame = frame;
    }
    else{
       
    }
   
    // _frame->readToPixels(testImage.getPixels());
    
    // strip.allocate(1,height,GL_RGB);
    ofSetColor(255);
    
    for(int i=0; i< _numRods; i++){
        
        //crop the LED strip
        strip.begin();
        //float x, float y, float w, float h, float sx, float sy
        _frame->getTexture().drawSubsection(0,0,1,height/2,i*_rodSpacing + _rodMargins, 0);
        strip.end();
        
        strip.readToPixels(stripImage.getPixels());
        
        //draw to LEDs
        
        ////IP address
        string ipStart = "192.168.0.";
        int ipEnd = (51+floor(i/16));
        
        //if(ipEnd == 51){
            
            char *ip = new char[12];
            sprintf(ip, "192.168.0.%d", ipEnd);
            
            ////subnet
            int subnet = (int)floor(i/8)%2;
            
            ////universe
            int universe = (i%8)*2;
            //cout<<"send dmx strip #"<<i<< " to: "<<ip<<", "<<subnet<<", "<<universe<<endl;
            
            artnet.sendDmx(ip, subnet, universe, stripImage.getPixels(), (height/2)*3);
            //strip 1 is universes 0 and 1
            
            //cleanup!
            delete[] ip;
        //}
    }
    
}

//draw to screen
void displaySystem::draw(int x, int y){
    //render test mode
    ofSetColor(255);
    _frame->draw(x,y);
    
    for(int i=0; i< numRodsOuter; i++){
        strip.begin();
        _frame->getTexture().drawSubsection(0,0,1,height,i*_rodSpacing+_rodMargins,0);
        strip.end();
        strip.draw(x+_rodSpacing*i+_rodMargins, y+10+height);
    }
    
}


void displaySystem::clear(){
    cout<<"CLEAR LEDS"<<endl;
    //send clear signal to LED strips
    ofColor black;
    black.r = 0;
    black.g = 0;
    black.b = 0;
    stripImage.setColor(black);
    artnet.sendDmx("192.168.0.51", 0, 0, stripImage.getPixels(), 512);
    artnet.sendDmx("192.168.0.51", 0, 2, stripImage.getPixels(), 512);
    for(int i=0; i< _numRods; i++){
        //draw to LEDs
        ////IP address
        string ipStart = "192.168.0.";
        int ipEnd = (51+floor(i/16));
            
            char *ip = new char[12];
            sprintf(ip, "192.168.0.%d", ipEnd);
            
            ////subnet
            int subnet = (int)floor(i/8)%2;
            
            ////universe
            int universe = (i%8)*2;
            cout<<"send dmx strip #"<<i<< " to: "<<ip<<", "<<subnet<<", "<<universe<<endl;
            
            artnet.sendDmx(ip, subnet, universe, stripImage.getPixels(), 512);
            //strip 1 is universes 0 and 1
            
            //cleanup!
            delete[] ip;
    }
}

void displaySystem::loadTestImage(string path){
    testImage.load(path);
    _frame->begin();
    testImage.draw(0, 0, width, height);
    _frame->end();
    test = true;
}