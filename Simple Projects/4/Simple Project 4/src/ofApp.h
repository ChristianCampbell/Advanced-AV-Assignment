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
 
    maxiOsc sin1, sin2, sin3, sin4, sin5, sin6, sin7, sin8;
    double sinfreq1, sinfreq2, sinfreq3, sinfreq4, sinOut1, sinOut2, sinOut3, sinOut4;
    double sinfreq5, sinfreq6, sinfreq7, sinfreq8, sinOut5, sinOut6, sinOut7, sinOut8;
    double sinOut1env;
    int clockpos = 0;
    int mouseclock = 0;
    
    int click = 0;
    int setChoice, setChoiceClick, pitchIf, pitchChoice;
    
    //initialising things, including notes for the chords! I used a 2D array because it's easier to call
    double pitch [8], circleX [8], circleY [8],
    
    pitchSet1 [4][5]
    
    {130,195,311,391,587,
        
     103,130,311,466,698,
        
     195,293,349,466,587,
        
     87,130,220,349,523};
    
    
    double circleAlpha [8], circleScale [8], vol[8], attack[8], myEnvelopeOut[8], trig;
    ofPixels pixels;
    ofColor averageColor;

    maxiEnv myEnvelope[8];
    
    
    
    
    int		bufferSize;
    int		sampleRate;
    

};


