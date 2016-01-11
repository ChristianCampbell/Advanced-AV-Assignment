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
    
    int		bufferSize;
    int		sampleRate;
    
    /* stick your maximilian stuff below */
    
    
    
    // Setting amp to 600 makes the shape large enough to leave the sides of the screen
    float amp = 600;
    
    /* These two variables change as you move the mouse so it doesn't matter too much what
       they are initialised as
     */
    float ampMouse = 1;
    float phaseMouse = 0.1;
    
    // Setting the phase to 10 starts the program in a more interesting position
    float phase = 10;
    
    // The amount of lines
    int elements =500;
    int i,j;
    
};


