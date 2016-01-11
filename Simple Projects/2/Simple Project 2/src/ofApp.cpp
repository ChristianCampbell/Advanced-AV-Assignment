/* This is an example of how to integrate maximilain into openFrameworks,
 including using audio received for input and audio requested for output.
 
 
 You can copy and paste this and use it as a starting example.
 
 */

/* Hello!
 
   This is a visual piece of code with no audio. There is also some mouse interaction. 
 
   If you move the mouse left of the centre of the screen the shape will get smaller, 
   and if you move it to the right the shape will get larger.
 
   If you move the mouse up from the centre of the screen the shape will spin more in
   one direction, and if you move it down it will spin more in the other direction
 
 */

#include "ofApp.h"
//#include "time.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
	/* This is stuff you always need.*/
    sampleRate 	= 44100; /* Sampling Rate */
	bufferSize	= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    
    /* Anything that you would normally find/put in maximilian's setup() method needs to go here. For example, Sample loading.
     
     */
    
    ofEnableAlphaBlending();
    ofBackground(230);
    
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4); /* this has to happen at the end of setup - it switches on the DAC */
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    /* The phase and amplitude are set to be quite slow because I feel it makes a much more
       effective image if the shapes are morphing slowly */
    
    phase+=(phaseMouse/500000);
    
    amp+= (ampMouse/800);
    
    /* This part controls the movement of the spacing of the lines from each other, the
       trigonometric functions are called later in the for loops */
    
    float spacing = (phase/elements)*TWO_PI;
    
    float spacing2 = (amp/elements)*TWO_PI;
    
    ofNoFill();
    
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
    
    /* Below are two sets of lines which overlap each other. if there is only one set active
       an interesting image is still produced, but the overlap makes it much more effective */
  
    for (i = 10; i < elements;i++) {
        // Set line to black with a slight opacity so overlapping parts are darker, this adds more depth
        ofSetColor(0, 0, 0, 50);
        // Set a thin line
        ofSetLineWidth(1);
        
        /* This is where the trigonometric funtions happen, truth be told they are a result of trial and
           error of what looked the best to me. each line is affected differently (the 'j' is the variable
           for each different line in the for loop) */
        
        ofLine(cos(spacing*i-1)*amp,sin(cos(spacing*i)*amp),cos(sin(spacing*i)*amp), sin(spacing*i)*amp);
        
        ofRotate(1);
        
    }
    
   for (j = 10; j < elements;j++) {
        // Set line to black with a slight opacity so overlapping parts are darker, this adds more depth
        ofSetColor(0, 0, 0, 25);
        //set a thin line
        ofSetLineWidth(1);
       
        /* This is where the trigonometric funtions happen, truth be told they are a result of trial and
           error of what looked the best to me. each line is affected differently (the 'j' is the variable
           for each different line in the for loop) */
       
        ofLine(cos(spacing*j-1)*amp,sin(cos(spacing*j)*amp),cos(sin(spacing*j)*amp), sin(spacing*j)*amp);
        
        ofRotate(1);
        
    }
   
    
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
	
	for (int i = 0; i < bufferSize; i++) {
        
        /* Stick your maximilian 'play()' code in here ! Declare your objects in ofApp.h
		 
		 For information on how maximilian works, take a look at the example code at
		 
		 http://www.maximilian.strangeloop.co.uk
		 
		 under 'Tutorials'.
		 
		 */
        
        output[i*nChannels    ] = 0; /* You may end up with lots of outputs. add them here */
        output[i*nChannels + 1] = 0;
        
        
        /* You may end up with lots of outputs. add them here */
        
		
	}
	
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	// samples are "interleaved"
	for(int i = 0; i < bufferSize; i++){
        /* you can also grab the data out of the arrays*/
        
    }
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
    // Getting mouse position and centering values to manipulate movement
    
    ampMouse = mouseX - ofGetWidth()/2;
    phaseMouse = mouseY - ofGetHeight()/2;
    
    // Testing whether the information is sent out
    
    //cout << ampMouse << ", "<< phaseMouse << endl;
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
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
