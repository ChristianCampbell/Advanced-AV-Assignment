/* This is an example of how to integrate maximilain into openFrameworks,
 including using audio received for input and audio requested for output.
 
 
 You can copy and paste this and use it as a starting example.
 
 */



/* Hello!
 
   This is a user interactive music making application!
 
   The screen is split into 5 horizontal slices which correspond to a certain note of a chord.
 
   When you click a circle will appear, and a tone will be played and slowly fade out, in correlation
   to the circle growing and fading.
 
   If you press the 'c' key, it will switch between the sets of chords it can play and
   the screen will change colour to indicate this happening.
 
   In each of the two sets of chords there are 4 chords. The chord change within the sets is
   controlled by the average rgb value of the video in the background. This value is converted to one 
   number by averaging (brightness) and this value is split into 4 sections. Each chord is in one of
   these sections, so is changed as the background changes, giving an explicit relationship between the
   change of notes and change of visuals.
 
   When you move the mouse left to right it alters the speed of change between the videos in
   the background.
 
   There is also an FM modulator signal which is increased by moving the mouse to the left of the
   window. It only activates once its to the left of the central line of the screen.
 
   The video clips are taken by me so there are no copyright issues :)
 
   P.S This project works best with good speakers or headphones, because some of the frequencies
       produced are quite low (down to ~65Hz).
 
 */


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
	/* This is stuff you always need.*/
    sampleRate 	= 44100; /* Sampling Rate */
	bufferSize	= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    
    /* Anything that you would normally find/put in maximilian's setup() method needs to go here. For example, Sample loading.
     
     */

    // Loading videos, the locations should be OK as they are in the data folder in the project file.
    
    myVideo .loadMovie("Movie_1.mov");
    myVideo2.loadMovie("Movie_2.mov");
    
    // Playing videos
    myVideo .play();
    myVideo2.play();
    
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4); /* this has to happen at the end of setup - it switches on the DAC */
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //ofHideCursor doesn't seem to work properly. Possible bug?
    //ofHideCursor();
    
    //////////////// V I D E O ////////////////
    
    // Muting videos so they dont affect the music
    myVideo.setVolume(0.0f);
    myVideo2.setVolume(0.0f);
    myVideo.update();
    myVideo2.update();
    
    
    // Working out average colour from a section of video
    
    // If the frame is new then do these sums
    if(myVideo.isFrameNew()) {
        
        pixels = myVideo2.getPixels();
        
        // Setting variables, they are local to update so no need to put in the .h file
        int rTotal = 0, gTotal = 0, bTotal = 0;
        
        /* Looping through a 50*50 section of the the video to get colours
         
         It's only 50*50 because I didnt want to create a huge amount of numbers.
         Because the video is quite blurry anyway, the coulours on the screen are all
         quite similar
         */
        
        for(int x = 0; x < 50; x++) {
            for(int y = 0; y < 50; y++) {
                ofColor pixelColor = pixels.getColor(x, y);
                rTotal += pixelColor.r;
                gTotal += pixelColor.g;
                bTotal += pixelColor.b;
            }
        }
        
        // This is the total size of the section taken to average
        int samples = 50 * 50;
        
        // Updating the average color
        averageColour.r = rTotal/samples;
        averageColour.b = gTotal/samples;
        averageColour.g = bTotal/samples;
    }

    
    mouseclockmapped = ofMap(mouseclock,0,960,20,100);
    // A simple way to make the video change to a random frame
    clockpos += 1;
    if (clockpos > mouseclockmapped){
        clockpos = 0;
        myVideo.setPosition(ofRandom(1));
        myVideo2.setPosition(ofRandom(1));
    }
    
    //////////////// A U D I O ////////////////
    
    /* Using the colour averages from before to get an overall average.
     
     rVol is for when its dark its closer to 0 and when its light its closer to 255.
     
     gVol is the opposite, so that when it is dark it is closer to 255 and when it is light
     it is closer to 0.
     
     These values effect the volume of the different tones later on in the program.
     */
    
    rVol = (averageColour.r + averageColour.g + averageColour.b) / 3;
    gVol = ofMap(rVol, 0 , 255, 255, 0);
    
    /* When the average colour changes, a different chord is selected. Only
     the last note clicked will get affected of the notes already playing.
     This is so that the chords don't change around too much and become too distracting,
     but instead you just have the one modulating note which changes the feeling of the
     chord playing.
     */
    
    if(gVol > 140){
        setChoice = 0+setChoiceMod;
    }
    
    if(gVol < 140 && gVol > 100){
        setChoice = 1+setChoiceMod;
    }
    
    if(gVol < 100 && gVol > 40) {
        setChoice = 2+setChoiceMod;
    }
    
    if(gVol < 40) {
        setChoice = 3+setChoiceMod;
    }
    
    
    cout<<gVol<< ", "<<setChoice<<endl;
    
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
    
    // Changes the last note played depending on setChoice
    
    pitch [click] = pitchSet1 [setChoice] [pitchChoice];
    
    //cout << pitch[0] << ", "<< pitch[1] << ", "<< pitch[2] << ", "<< pitch[3] << ", " << pitch[4] << ", "<< pitch[5] << ", "<< pitch[6] << ", "<< pitch[7] << endl;
    
    // This is where the pitches are set for the oscillators
    
    pitchMultiplys = ofMap(pitchMultiply, 0, ofGetWidth()/2,120,0);
    if(pitchMultiplys < 0){pitchMultiplys=0;}
    
    
    //cout<<pitchMultiplys<<endl;
    
    sinfreq1 = pitch[0];
    sinfreq2 = pitch[1];
    sinfreq3 = pitch[2];
    sinfreq4 = pitch[3];
    sinfreq5 = pitch[4];
    sinfreq6 = pitch[5];
    sinfreq7 = pitch[6];
    sinfreq8 = pitch[7];
    sinfreq9 = pitch[8];
    sinfreq10 = pitch[9];
    
    
    /* The alpha fade and size grow for the circles. Its nice when the circle grows out and fades.
       It's like a bubble popping in slow motion. */
    
    for(int i=0; i<10; i++) {
       
       circleAlpha[i] -= 1.5;
       circleScale[i] += 0.04;
   }
    
    
    
    // myEnvelopeOut[click]=myEnvelope[click].adsr(1.,myEnvelope[click].trigger);
    
    // I would have used maxiEnv here but I couldn't get it to work properly within openFrameworks
    
    /* This is a rudimentary envelope using adding numbers and if statements. Because it isnt entirely smooth,
       there is a slight clicking on the attack of the notes. If I get maxiEnv to work one day maybe I'll be 
       able to fix this. */
    
    for(int j=0; j<10; j++) {
        
        vol[j] -= 0.003;
         if(vol[j]<0.01){vol[j]=0;}
         
         attack[j] += 0.1;
         if(attack[j]>1){attack[j]=1;}
        
        
      /*
        more stuff from maxiEnv
       
        myEnvelope[j].trigger=vol[j];
        vol[j] -= 1;
        if(vol[j]<0.01){vol[j]=0;}*/

    }

    //cout << vol[1] << ", " << attack[1] << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //////////////// V I D E O ////////////////
    
    // I wanted to create a kaleidescope effect with the videos so they are mirrored around each other.
    
    // I'm using push and pop matrices so that the translations dont affect the other videos
    
    // Top left video
    ofPushMatrix();
    /* Set the colour of the video so it is affected by rMod, gMod and bMod, it is set in the order
       b, g, r so that there is a more interesting and less uniform hue cast ofer it.*/
    ofSetColor(255-bMod, 255-gMod, 255-rMod, 120);
    // Draw video normally but a quarter of the window size
    myVideo .draw(0, 0, ofGetWidth()/2, ofGetHeight()/2);
    myVideo2.draw(0, 0, ofGetWidth()/2, ofGetHeight()/2);
    
    
    ofPopMatrix();
    
    // Bottom left video
    ofPushMatrix();
    // Translate video window
    ofTranslate(0, ofGetHeight());
    // Scale it negatively to mirror it in y axis
    ofScale(1, -1);
    /* Set the colour of the video so it is affected by rMod, gMod and bMod, it is set in the order
     b, g, r so that there is a more interesting and less uniform hue cast ofer it.*/
    ofSetColor(255-bMod, 255-gMod, 255-rMod, 120);
    // Draw the video
    myVideo .draw( 0, 0, ofGetWidth()/2, ofGetHeight()/2);
    myVideo2.draw( 0, 0, ofGetWidth()/2, ofGetHeight()/2);
  
    ofPopMatrix();
    
    // Top right video
    ofPushMatrix();
    // Translate video window
    ofTranslate(ofGetWidth(),0);
    // Scale it negatively to mirror it in x axis
    ofScale(-1, 1);
    /* Set the colour of the video so it is affected by rMod, gMod and bMod, it is set in the order
     b, g, r so that there is a more interesting and less uniform hue cast ofer it.*/
    ofSetColor(255-bMod, 255-gMod, 255-rMod, 120);
    // Draw the video
    myVideo .draw( 0, 0, ofGetWidth()/2, ofGetHeight()/2);
    myVideo2.draw( 0, 0, ofGetWidth()/2, ofGetHeight()/2);
   
    ofPopMatrix();
    
    // Bottom right video
    ofPushMatrix();
    // Translate video window
    ofTranslate(ofGetWidth(),ofGetHeight());
    // Scale it negatively to mirror it in x and y axis
    ofScale(-1, -1);
    /* Set the colour of the video so it is affected by rMod, gMod and bMod, it is set in the order
     b, g, r so that there is a more interesting and less uniform hue cast ofer it.*/
    ofSetColor(255-bMod, 255-gMod, 255-rMod, 120);
    // Draw the video
    myVideo .draw( 0, 0, ofGetWidth()/2, ofGetHeight()/2);
    myVideo2.draw( 0, 0, ofGetWidth()/2, ofGetHeight()/2);

    ofPopMatrix();
    
    /* This is where various parts of code are affected by the change of key (pressing the 'c'
       key will change the key). The colour of the video changes, and also the notes which are played
       change as a different part of the note array is called. This is done by just adding a constant
       to the funcition which calls the note part of the array (in this case 4).
     
       This sets rMod, gMod and bMod to the average parts of the colour of the 50x50 section of the
       video. colourMod is what changes the intensity of the colour changing effect. It slowly fades in
       to give more of a sense of fluidity.
    */
    
    if(keyChoice==0){
        colourMod -= 0.01;
        if (colourMod <0.05){colourMod=0;}
        setChoiceMod=0;
        rMod = (averageColour.r) * colourMod;
        gMod = (averageColour.g) * colourMod;
        bMod = (averageColour.b) * colourMod;
    }
    
    if(keyChoice==1){
        colourMod += 0.01;
        if (colourMod >0.8){colourMod=0.9;}
        setChoiceMod=4;
        rMod = (averageColour.r) * colourMod;
        gMod = (averageColour.g) * colourMod;
        bMod = (averageColour.b) * colourMod;
    }
    
    
    //////////////// V I S U A L ////////////////
    
    /* Drawing the circles!
       
       The colour of the circles is slightly affected by rMod, bMod and gMod so that they don't
       stand out so starkly from the background, and make the visuals seem as if they are more
       on one level
    */
     
    
    for(int k=0; k<10; k++) {
        
        ofSetColor(20 + (0.1*bMod),20 + (0.1*gMod),20 + (0.1*rMod), circleAlpha[k]);
        ofSetCircleResolution(100); // To draw more smooth circles
        ofDrawCircle (circleX[k], circleY[k], 10 * circleScale[k], 10 * circleScale[k]);
    
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
    
        
        /* Sine oscillators sound nice for this kind of thing because of their pure tone.
           
           Below is all the mixing informtion, it's quite simple, the vol[] and attack[]
           numbers manipulate the level outputted and everything is divided by 8 so it doesn't
           clip.
         
           There are 10 oscillators so there is 10 note polyphony.
         
           There is some light FM modulation based on the cursors X position. The sines
           are modulated by their own frequency so it is more tonal.
        */
        
        sinOut1 = sin1.sinewave(sinfreq1+(fmSin.sinewave(sinfreq1)*pitchMultiplys));
        
        sinOut2 = sin2.sinewave(sinfreq2+(fmSin.sinewave(sinfreq2)*pitchMultiplys));
    
        sinOut3 = sin3.sinewave(sinfreq3+(fmSin.sinewave(sinfreq3)*pitchMultiplys));
        
        sinOut4 = sin4.sinewave(sinfreq4+(fmSin.sinewave(sinfreq4)*pitchMultiplys));
        
        sinOut5 = sin5.sinewave(sinfreq5+(fmSin.sinewave(sinfreq5)*pitchMultiplys));
        
        sinOut6 = sin6.sinewave(sinfreq6+(fmSin.sinewave(sinfreq6)*pitchMultiplys));
    
        sinOut7 = sin7.sinewave(sinfreq7+(fmSin.sinewave(sinfreq7)*pitchMultiplys));
        
        sinOut8 = sin8.sinewave(sinfreq8+(fmSin.sinewave(sinfreq8)*pitchMultiplys));
        
        sinOut9 = sin9.sinewave(sinfreq9+(fmSin.sinewave(sinfreq9)*pitchMultiplys));
        
        sinOut10 = sin10.sinewave(sinfreq10+(fmSin.sinewave(sinfreq10)*pitchMultiplys));
        
        
        output[i*nChannels    ] = ((sinOut1 / 8)*vol[0]*attack[0]) + ((sinOut2 / 8)*vol[1]*attack[1]) + ((sinOut3 / 8)*vol[2]*attack[2]) + ((sinOut4 / 8)*vol[3]*attack[3]) + ((sinOut5 / 8)*vol[4]*attack[4]) + ((sinOut6 / 8)*vol[5]*attack[5]) + ((sinOut7 / 8)*vol[6]*attack[6]) + ((sinOut8 / 8)*vol[7]*attack[7])+ ((sinOut9 / 8)*vol[8]*attack[8]) + ((sinOut10 / 8)*vol[9]*attack[9]);
        
        
        output[i*nChannels + 1] = ((sinOut1 / 8)*vol[0]*attack[0]) + ((sinOut2 / 8)*vol[1]*attack[1]) + ((sinOut3 / 8)*vol[2]*attack[2]) + ((sinOut4 / 8)*vol[3]*attack[3]) + ((sinOut5 / 8)*vol[4]*attack[4]) + ((sinOut6 / 8)*vol[5]*attack[5]) + ((sinOut7 / 8)*vol[6]*attack[6]) + ((sinOut8 / 8)*vol[7]*attack[7])+ ((sinOut9 / 8)*vol[8]*attack[8]) + ((sinOut10 / 8)*vol[9]*attack[9]);
        
        
       
        
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
  
  
    // Pressing the 'c' key cycles through the key choice and changes the background.
    if(key == 'c') {
        
        keyChoice +=1;
        if(keyChoice > 1){
            keyChoice = 0;
        }
    }
   
    //cout << keyChoice << endl;
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
    //sending info to speed change of videos and FM modulation
    
    mouseclock = mouseX;
    
    pitchMultiply = mouseX;
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    click += 1;
    
    // Resets clicks to 0
    if(click > 9){
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
