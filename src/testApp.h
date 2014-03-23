#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxImageSequence.h"
#include "ofxOsc.h"
#include "NanoKontrol2.h"

#include "Particle.h"

#include <vector>

#define PORT 8000

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
        
        vector<Particle> particles;
        int num_particles;
        int num_particles_display;
    
        float slice_1;
        float slice_2;
        float slice_3;
    
        float vel_yang;
        float acc_yang;
        float flock_yang;
        float vel_yin;
        float acc_yin;
        float flock_yin;
    
        float zoom;
    
        int trail;
        int contrast;
    
        ofxUICanvas *gui;
        
        bool shader;
        bool guivisible;
        bool yinyang;
    
        ofImage grabbed;
        ofFbo fbo;
        unsigned char * pixels;
    
        //ofxFluid fluid;
        
        ofxImageSequence sequence1;
        ofImage img1, img2, img3, img4, img5, img6;
    
        NanoKontrol2 kontrol;
    
        ofxOscReceiver	receiver;
    
        int steps;
    
        int width;
        int height;
    
        void exit(); 
        void guiEvent(ofxUIEventArgs &e);
        bool isYang(float x, float y);
    
        // we don't actually use these
        // just checking to see if they
        // all work in the same place :)
        /*
        ofxCvGrayscaleImage cvGray;
        ofxTCPClient client;
        ofxTCPServer server;
        ofxOscSender osc_sender;
        ofxXmlSettings settings;
        ofx3DModelLoader modelLoader;
        ofxAssimpModelLoader betterModelLoader;
        //ofxSynth synth;
        ofxThreadedImageLoader threadedLoader;
        */
};
