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
    
    maxiOsc sin1, sin2, sin3, sin4, sin5, sin6, sin7, sin8, sin9, sin10, fmSin;
    double sinfreq1, sinfreq2, sinfreq3, sinfreq4, sinOut1, sinOut2, sinOut3, sinOut4;
    double sinfreq5, sinfreq6, sinfreq7, sinfreq8, sinOut5, sinOut6, sinOut7, sinOut8, fmSinOut;
    double sinfreq9, sinfreq10, sinOut9, sinOut10;
    double sinOut1env;
    double pitchMultiply, pitchMultiplys;
    double colourMod;
    
    int click = 0;
    int setChoice,setChoiceMod, setChoiceClick, pitchIf, pitchChoice, keyChoice=0;
    
    //initialising things, including notes for the chords! I used a 2D array because it's easier to call
    double pitch [10], circleX [10], circleY [10],
    
    pitchSet1 [8] [5]
    
    {130,195,311,391,587,
        
     87,130,311,466,698,
        
     130,293,349,466,587,
        
     87 ,130,220,349,523,
    
        97 ,116,155,233,466,
        
        65,155,233,311,587,
        
        97 ,146,293,466,698,
        
        65 ,97,195,311,523};
    
    double circleAlpha [10], circleScale [10], vol[10], attack[10], myEnvelopeOut[10], trig;
    
    double rVol, gVol, bVol, rMod, gMod, bMod;
    int clockpos = 0;
    int mouseclock = 0;
    int mouseclockmapped = 0;
    
    ofPixels pixels;
    ofColor averageColour;
    
    //maxiEnv myEnvelope[8];
    
    int		bufferSize;
    int		sampleRate;
    

};


