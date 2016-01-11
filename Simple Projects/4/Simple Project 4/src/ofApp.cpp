/* This is an example of how to integrate maximilain into openFrameworks,
 including using audio received for input and audio requested for output.
 
 
 You can copy and paste this and use it as a starting example.
 
 */



/* Hello!
 
   In this project, when you move the mouse up and down and click, a circle will appear and a tone will be played.
   The screen is split into 5 sections vertically, one for each note of the selected chord.
 
    To change the selected chord, press the ‘c’ key. It will cycle through 4 chords to play which all interact nicely
    with each other.

 
 */


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
	/* This is stuff you always need.*/
    sampleRate 	= 44100; /* Sampling Rate */
	bufferSize	= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    
    /* Anything that you would normally find/put in maximilian's setup() method needs to go here. For example, Sample loading.
     
     */
    
   /*
    // Setting envelope parameters
    for (int l=0;l<8;l++) {
        
        myEnvelope[l].setAttack(0.1);
        myEnvelope[l].setDecay(2000);
        myEnvelope[l].setSustain(0.0);
        myEnvelope[l].setRelease(1000);
        
    }*/
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4); /* this has to happen at the end of setup - it switches on the DAC */
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
   
    // The code below tells the pitchChoice which note to pick depending on the circle Y position.
    
    if(circleY[click] < 100){
        pitchChoice = 0;
    }
    
    if(circleY[click] > 100 && circleY[click] < 200){
        pitchChoice = 1;
    }
    
    if(circleY[click] > 200 && circleY[click] < 300){
        pitchChoice = 2;
    }
    
    if(circleY[click] > 300 && circleY[click] < 400){
        pitchChoice = 3;
    }
    
    if(circleY[click] > 400){
        pitchChoice = 4;
    }
    
    pitch [click] = pitchSet1 [setChoiceClick] [pitchChoice];
    
    //cout << pitch[0] << ", "<< pitch[1] << ", "<< pitch[2] << ", "<< pitch[3] << ", " << pitch[4] << ", "<< pitch[5] << ", "<< pitch[6] << ", "<< pitch[7] << endl;
    
    // This is where the pitches are set for the oscillators
    
    sinfreq1 = pitch[0];
    sinfreq2 = pitch[1];
    sinfreq3 = pitch[2];
    sinfreq4 = pitch[3];
    sinfreq5 = pitch[4];
    sinfreq6 = pitch[5];
    sinfreq7 = pitch[6];
    sinfreq8 = pitch[7];
    
    /* The alpha fade and size grow for the circles. Its nice when the circle grows out and fades.
       It's like a bubble popping in slow motion. */
    
    for(int i=0; i<8; i++) {
       
       circleAlpha[i] -= 1.5;
       circleScale[i] += 0.04;
   }
    
    
    
    // myEnvelopeOut[click]=myEnvelope[click].adsr(1.,myEnvelope[click].trigger);
    
    // I would have used maxiEnv here but I couldn't get it to work properly within openFrameworks
    
    /* This is a rudimentary envelope using adding numbers and if statements. Because it isnt entirely smooth,
       there is a slight clicking on the attack of the notes. If I get maxiEnv to work one day maybe I'll be 
       able to fix this. 
     */
    
    for(int j=0; j<8; j++) {
        
        vol[j] -= 0.003;
         if(vol[j]<0.01){vol[j]=0;}
         
         attack[j] += 0.1;
         if(attack[j]>1){attack[j]=1;}
        
        
      /*
        more stuff from maxiEnv
       
        myEnvelope[j].trigger=vol[j];
        vol[j] -= 1;
        if(vol[j]<0.01){vol[j]=0;}
       */

    }
    
    //cout << vol[1] << ", " << attack[1] << endl;

    
}

//--------------------------------------------------------------
void ofApp::draw(){
   
    //Setting the background
    
    ofBackground(255);
    
    //Drawing the circles!
    
    for(int k=0; k<8; k++) {
        
        ofSetColor(0,0,0, circleAlpha[k]);
        ofSetCircleResolution(100); // To draw more smooth circles
        ofDrawCircle (circleX[k], circleY[k], 10 * circleScale[k], 10 * circleScale[k]);
    }

    ofSetColor(0,0,0);
    ofDrawBitmapString("Press 'c' to cycle through chords!" ,110, 20);
    ofDrawBitmapString("Click to play notes!" ,170, 50);
    
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
	
	for (int i = 0; i < bufferSize; i++) {
        
        /* Stick your maximilian 'play()' code in here ! Declare your objects in ofApp.h
		 
		 For information on how maximilian works, take a look at the example code at
		 
		 http://www.maximilian.strangeloop.co.uk
		 
		 under 'Tutorials'.
		 
		 */
    
        
        /* Sine oscillators sound nice for this kind of thing because of their pure tone.
         
           There are 8 oscillators for 8 note polyphony
           
           Below is all the mixing informtion, it's quite simple, the vol[] and attack[]
           numbers manipulate the level outputted and everything is divided by 8 so it doesn't
           clip.
         */
        
        sinOut1 = sin1.sinewave(sinfreq1);
        
        sinOut2 = sin2.sinewave(sinfreq2);
        
        sinOut3 = sin3.sinewave(sinfreq3);
        
        sinOut4 = sin4.sinewave(sinfreq4);
        
        sinOut5 = sin5.sinewave(sinfreq5);
        
        sinOut6 = sin6.sinewave(sinfreq6);
    
        sinOut7 = sin7.sinewave(sinfreq7);
        
        sinOut8 = sin8.sinewave(sinfreq8);
        
        
        output[i*nChannels    ] = ((sinOut1 / 8)*vol[0]*attack[0]) + ((sinOut2 / 8)*vol[1]*attack[1]) + ((sinOut3 / 8)*vol[2]*attack[2]) + ((sinOut4 / 8)*vol[3]*attack[3]) + ((sinOut5 / 8)*vol[4]*attack[4]) + ((sinOut6 / 8)*vol[5]*attack[5]) + ((sinOut7 / 8)*vol[6]*attack[6]) + ((sinOut8 / 8)*vol[7]*attack[7]);
        
        
        output[i*nChannels + 1] = ((sinOut1 / 8)*vol[0]*attack[0]) + ((sinOut2 / 8)*vol[1]*attack[1]) + ((sinOut3 / 8)*vol[2]*attack[2]) + ((sinOut4 / 8)*vol[3]*attack[3]) + ((sinOut5 / 8)*vol[4]*attack[4]) + ((sinOut6 / 8)*vol[5]*attack[5]) + ((sinOut7 / 8)*vol[6]*attack[6]) + ((sinOut8 / 8)*vol[7]*attack[7]);
        
        
       
        
     /*  
      
      This is some of the code for the envelope I couldn't get to work properly. 
      The envelope would not reset itself after 1 time through the click cycle.
      This means I had to settle for a more clicky sounding home made approach which is still OK.
      
      for(int j=0; j<8; j++) {
            
        myEnvelopeOut[j]=myEnvelope[j].adsr(1.,myEnvelope[j].trigger);
            
        }

       
        sinOut1 = sin1.sinewave(sinfreq1)*myEnvelopeOut[0];
        
        sinOut2 = sin2.sinewave(sinfreq2)*myEnvelopeOut[1];
    
        sinOut3 = sin3.sinewave(sinfreq3)*myEnvelopeOut[2];
        
        sinOut4 = sin4.sinewave(sinfreq4)*myEnvelopeOut[3];
        
        sinOut5 = sin5.sinewave(sinfreq5)*myEnvelopeOut[4];
        
        sinOut6 = sin6.sinewave(sinfreq6)*myEnvelopeOut[5];
        
        sinOut7 = sin7.sinewave(sinfreq7)*myEnvelopeOut[6];
        
        sinOut8 = sin8.sinewave(sinfreq8)*myEnvelopeOut[7];

       
        
        output[i*nChannels    ] = (sinOut1 / 8) + (sinOut2 / 8) + (sinOut3 / 8) + (sinOut4 / 8) + (sinOut5 / 8) + (sinOut6 / 8) + (sinOut7 / 8) + (sinOut8 / 8);
        
        
        output[i*nChannels + 1] = (sinOut1 / 8) + (sinOut2 / 8) + (sinOut3 / 8) + (sinOut4 / 8) + (sinOut5 / 8) + (sinOut6 / 8) + (sinOut7 / 8) + (sinOut8 / 8);
        
        for (int i=0; i<8; i++) {
            myEnvelope[i].trigger=0;
        } */

		
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
  
    
    // Pressing the 'c' key cycles through chord patterns.
    if(key == 'c') {
        
        setChoice +=1;
        if(setChoice > 3){
            setChoice = 0;
        }
    }
    
    //cout << setChoice << endl;
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    click += 1;
    
    // Resets clicks to 0
    if(click > 7){
        click = 0;
    }
    
    // Information for drawing circles
    circleX[click] = mouseX;
    circleY[click] = mouseY;
    
    circleAlpha[click] = 255;
    circleScale[click] = 1;
    
    vol[click] = 1;
    attack[click] = 0;
    
    /* Only changes the set of chords played when the mouse is pressed.
    If this didn't happen, the last note pressed would change while it was playing */
    setChoiceClick = setChoice;
    
   
    
    
   // cout << click << ", "<< circleX [click] << ", " << circleY [click] << endl ;
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
