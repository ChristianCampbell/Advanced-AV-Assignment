#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"

class ofApp : public ofBaseApp{
	
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    void audioOut(float * output, int bufferSize, int nChannels);
    void audioIn(float * input, int bufferSize, int nChannels);
	
    
    /* stick you maximilian declarations below
     
     For information on how maximilian works, take a look at the example code at
     
     http://www.maximilian.strangeloop.co.uk
     
     under 'Tutorials'.
     
     
     */
    
    
    //declaring variables
    
    ofVideoPlayer myVideo;
    ofVideoPlayer myVideo2;
    
    maxiOsc sin1, sin2, sin3, sin4;
    double sinfreq1, sinfreq2, sinfreq3, sinfreq4, sinOut1, sinOut2, sinOut3, sinOut4;
    double rVol, gVol, bVol;
    double sin4vol, sin4volf, sin4mouse;
    int clockpos = 0;
    int mouseclock = 0;
    int mouseclockmapped = 0;
 
    ofPixels pixels;
    ofColor averageColour;
    
    maxiClock myClock;
    
    int		bufferSize;
    int		sampleRate;
    
    
};


