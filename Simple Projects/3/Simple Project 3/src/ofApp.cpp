/* This is an example of how to integrate maximilain into openFrameworks,
 including using audio received for input and audio requested for output.
 
 
 You can copy and paste this and use it as a starting example.
 
 */



/* Hello!
 
   In this project, when you move the mouse left to right it changes the speed of change
   between the videos.
 
   When you move the mouse up and down, it changes the volume of the highest note in the chord.
   The note will only come in once you have gone above the halfway point on the screen.
 
   The video clips are taken by me so there are no copyright issues :)
 
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
    
    ofEnableAlphaBlending();
    
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4); /* this has to happen at the end of setup - it switches on the DAC */
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
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
    
    
    // Mapping mouseclock to more useable numbers
    mouseclockmapped = ofMap(mouseclock,0,960,10,100);
    // A simple way to make the video change to a random frame
    clockpos += 1;
    if (clockpos > mouseclockmapped){
        clockpos = 0;
        myVideo.setPosition(ofRandom(1));
        myVideo2.setPosition(ofRandom(1));
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // I wanted to create a kaleidescope effect with the videos so they are mirrored around each other.
    
    // I'm using push and pop matrices so that the translations dont affect the other videos
    
    // Top left video
    ofPushMatrix();
    // Draw video normally but a quarter of the window size
    ofSetColor(255, 255, 255, 120);
    myVideo .draw(0, 0, ofGetWidth()/2, ofGetHeight()/2);
    myVideo2.draw(0, 0, ofGetWidth()/2, ofGetHeight()/2);
    
    ofPopMatrix();
    
    // Bottom left video
    ofPushMatrix();
    // Translate video window
    ofTranslate(0, ofGetHeight());
    // Scale it negatively to mirror it in y axis
    ofScale(1, -1);
    // Draw the video
    ofSetColor(255, 255, 255, 120);
    myVideo .draw( 0, 0, ofGetWidth()/2, ofGetHeight()/2);
    myVideo2.draw( 0, 0, ofGetWidth()/2, ofGetHeight()/2);
    ofPopMatrix();
    
    // Top right video
    ofPushMatrix();
    // Translate video window
    ofTranslate(ofGetWidth(),0);
    // Scale it negatively to mirror it in x axis
    ofScale(-1, 1);
    // Draw the video
    ofSetColor(255, 255, 255, 120);
    myVideo .draw( 0, 0, ofGetWidth()/2, ofGetHeight()/2);
    myVideo2.draw( 0, 0, ofGetWidth()/2, ofGetHeight()/2);
    ofPopMatrix();
    
    // Bottom right video
    ofPushMatrix();
    // Translate video window
    ofTranslate(ofGetWidth(),ofGetHeight());
    // Scale it negatively to mirror it in x and y axis
    ofScale(-1, -1);
    // Draw the video
    ofSetColor(255, 255, 255, 120);
    myVideo .draw( 0, 0, ofGetWidth()/2, ofGetHeight()/2);
    myVideo2.draw( 0, 0, ofGetWidth()/2, ofGetHeight()/2);
    ofPopMatrix();
    //myVideo.draw(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth()/2, ofGetHeight()/2);
    
    
    /* Using the colour averages from before to get an overall average.
     
       rVol is for when its dark its closer to 0 and when its light its closer to 255.
     
       gVol is the opposite, so that when it is dark it is closer to 255 and when it is light
       it is closer to 0.
     
       These values effect the volume of the different tones later on in the program.
     */
    
    rVol = (averageColour.r + averageColour.g + averageColour.b) / 3;
    gVol = ofMap(rVol, 0 , 255, 255, 0);
    
    /* This is where the note frequencies are set. When the program returns to the first chord in the list
       there is a glide effect, so sometimes when the chord is triggered theres a nice sort of wonkyness
       to the pitches.
     
       To save repeated comments below, the glide can detect whether the note before it was higher or lower
       and whether it needs to glide down or up to the desired note. This is done using if statements
       saying 'if above then go down until you're nearly equal, and then become equal to the desired note'
       and vice versa.
     
       I decided not to do this for every chord because its nice when some of them just snap from one to
       the next and then occasionally go back to the glidey chord, it brings more interest to the sound
     */
    
    if(gVol > 140){
        
        // Glide for first note in the first chord
        if (sinfreq1 > 130) {
            sinfreq1 -= 3;
        }
        
        if (sinfreq1 < 130) {
            sinfreq1 += 3;
        }
        
        if (sinfreq1 > 120 && sinfreq1 < 140) {
            sinfreq1 = 130;
            
        }
        
        // Glide for second note in the first chord
        if (sinfreq2 > 195) {
            sinfreq2 -= 3;
        }
        
        if (sinfreq2 < 195) {
            sinfreq2 += 3;
        }
        
        if (sinfreq2 > 190 && sinfreq2 < 200) {
            sinfreq2 = 195;
            
        }
        
        // Glide for third note in the first chord
        if (sinfreq3 > 311) {
            sinfreq3 -= 3;
        }
        
        if (sinfreq3 < 311) {
            sinfreq3 += 3;
        }
        
        if (sinfreq3 > 305 && sinfreq3 < 320) {
            sinfreq3 = 311;
            
        }
        
        
        // Glide for fourth note in the first chord
        if (sinfreq4 > 391) {
            sinfreq4 -= 1;
        }
        
        if (sinfreq4 < 391) {
            sinfreq4 += 1;
        }
        
        if (sinfreq4 > 385 && sinfreq4 < 400) {
            sinfreq4 = 391;
            
        }
        
        
    }
    
    if(gVol < 140 && gVol > 100){
        
       
        // Quicker glide for the second bass note
        if (sinfreq1 > 103) {
            sinfreq1 -= 10;
        }
        
        if (sinfreq1 < 103) {
            sinfreq1 += 10;
        }
        
        if (sinfreq1 > 100 && sinfreq1 < 110) {
            sinfreq1 = 103;
            
        }
        sinfreq2 = 155;
        sinfreq3 = 261;
        sinfreq4 = 466;
    }
    
    if(gVol < 100) {
        
        
        // Quicker glide for the third bass note
        if (sinfreq1 > 97) {
            sinfreq1 -= 10;
        }
        
        if (sinfreq1 < 97) {
            sinfreq1 += 10;
        }
        
        if (sinfreq1 > 93 && sinfreq1 < 100) {
            sinfreq1 = 97;
            
        }
        sinfreq2 = 146;
        sinfreq3 = 233;
        sinfreq4 = 311;
    }

   /* Remapping mouseY coordinates so they work upwards instead of downwards.
      This will only activate after the halfway height point on the screen.
    */
    sin4vol = ofMap(sin4mouse, ofGetHeight()/2, 0, 0, 450);
    if (sin4vol <1){
        sin4vol = 0;
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
        
        
       
        /* These are just simple sines. I like how sines sound pure and when you
           have a nice chord it just sounds right. I couldn't quite work out how to
           stop the clicking, but that is something I will be able to figure out soon
         */
        
        sinOut1 = sin1.sinewave(sinfreq1);
        
        sinOut2 = sin2.sinewave(sinfreq2);
        
        sinOut3 = sin3.sinewave(sinfreq3);
        
        sinOut4 = sin4.sinewave(sinfreq4);
        
        sin4volf = sin4vol / 3000;
        
        /* The volumes are set here based on the rVol and gVol values from earlier. rVol and gVol
           are opposites from each other so as some volumes go up, the others will go down at the
           same rate and with the same magnitude
         
           rVol and gVol effect different sounds in different sides so it gives a more interesting
           panning effect to the sound
         */
        
        output[i*nChannels    ] = ((rVol/700) * sinOut1) + ((gVol/700) * sinOut2) + ((rVol/700) * sinOut3) + (sin4volf *sinOut4);
        output[i*nChannels + 1] = ((gVol/700) * sinOut1) + ((rVol/700) * sinOut2) + ((gVol/700) * sinOut3) + (sin4volf * sinOut4);
		
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
    
    //mouse information to change some variables
    
    mouseclock = mouseX;
    
    sin4mouse = mouseY;
    
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
