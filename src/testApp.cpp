#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    /* SETUP */
    
    /*
    ofSetWindowShape(800, 600);
    fbo.allocate(800, 600);
    fbo.begin();
    ofSetColor(0);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    fbo.end();
    
    // Initial Allocation
    fluid.allocate(800, 600, 0.5);
    // Seting the gravity set up & injecting the background image
    fluid.setGravity(ofVec2f(0.0,-1));
    fluid.setTexture(fbo.getTextureReference());
    
    fluid.addConstantForce(ofVec2f(0,0), ofVec2f(100,10), ofFloatColor(0), 100.f);
    fluid.addConstantForce(ofVec2f(800,600), ofVec2f(-100,-10), ofFloatColor(0), 100.f);
    fluid.addConstantForce(ofVec2f(800,0), ofVec2f(-100,10), ofFloatColor(0), 100.f);
    fluid.addConstantForce(ofVec2f(0,600), ofVec2f(100,-10), ofFloatColor(0), 100.f);
    */
    
    receiver.setup( 8000 );
    
    width = 800;
    height = 600;
    
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofBackground(0,0,0);
    
    fbo.allocate(width, height);  
    fbo.begin();
    ofSetColor(0);
    ofRect(0, 0, width, height);
    fbo.end();  
    
    pixels = new unsigned char[width*height*4];  
    grabbed.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
    
    glPixelStorei(GL_PACK_ALIGNMENT, 1);  
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);  
    grabbed.setFromPixels(pixels, width, height, OF_IMAGE_COLOR_ALPHA);
    
    kontrol.setup();
	kontrol.show();
    
    /* VARS */
    
    guivisible = true;
    shader = false;
    yinyang = false;
    
    trail = 50;
    contrast = 50;
    
    num_particles = 50000;
    num_particles_display = 200;
    
    slice_1 = 0.0;
    slice_2 = 0.0;
    slice_3 = 0.0;
    
    vel_yang = 1.0;
    acc_yang = 1.0;
    flock_yang = 1.0;
    
    vel_yin = 1.0;
    acc_yin = 1.0;
    flock_yin = 1.0;
    
    zoom = 1.0;
    
    steps = 0;
    
    /* FIRST THINGS FIRST */
    
    for(int i = 0; i < num_particles; i++){
        particles.push_back( Particle() );
    }
    
    sequence1.loadSequence("taichi", "png", 1, 240, 4);
	sequence1.preloadAllFrames();	//this way there is no stutter when loading frames
	sequence1.setFrameRate(30);
    
    img1.loadImage("5imagens.jpg");
    img2.loadImage("8tri.jpg");
    img3.loadImage("64hexa.jpg");
    img4.loadImage("7.jpg");
    img5.loadImage("0.jpg");
    
    
    /* GUI */
    
    gui = new ofxUICanvas(0,0,320,600);
    
    gui->addWidgetDown(new ofxUILabel("YIJING", OFX_UI_FONT_LARGE)); 
    gui->addWidgetDown(new ofxUIToggle(40, 40, true, "YIN&YANG"));
    gui->addWidgetDown(new ofxUISlider(304,16,1.0,num_particles,1000.0,"PARTICLES"));
    gui->addWidgetDown(new ofxUISlider(304,16,1,500.0,1.0,"VEL_YANG"));
    gui->addWidgetDown(new ofxUISlider(304,16,1,1000,1.0,"ACC_YANG"));
    gui->addWidgetDown(new ofxUISlider(304,16,0.001,200.0,1.0,"FLOCK_YANG"));
    gui->addWidgetDown(new ofxUISlider(304,16,1,500.0,1.0,"VEL_YIN"));
    gui->addWidgetDown(new ofxUISlider(304,16,1,1000,1.0,"ACC_YIN"));
    gui->addWidgetDown(new ofxUISlider(304,16,0.001,200.0,1.0,"FLOCK_YIN"));
    gui->addWidgetDown(new ofxUISlider(304,16,1.0,10.0,5.0,"ZOOM"));
    gui->addWidgetDown(new ofxUISlider(304,16,1.0,255.0,50.0,"TRAIL"));
    gui->addWidgetDown(new ofxUISlider(304,16,1.0,255.0,50.0,"CONTRAST"));
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->loadSettings("GUI/guiSettings.xml");
}

//--------------------------------------------------------------
void testApp::update(){
    
    vel_yang = ofMap(kontrol.channel[0].slider,0.0,1.0,0.01,50.0);
    acc_yang = ofMap(kontrol.channel[1].slider,0.0,1.0,0.01,50.0);
    flock_yang = ofMap(kontrol.channel[2].slider,0.0,1.0,0.001,200.0);
    
    vel_yin = ofMap(kontrol.channel[3].slider,0.0,1.0,0.01,50.0);
    acc_yin = ofMap(kontrol.channel[4].slider,0.0,1.0,0.01,50.0);
    flock_yin = ofMap(kontrol.channel[5].slider,0.0,1.0,0.001,200.0);
    
    zoom = ofMap(kontrol.channel[6].slider,0.0,1.0,1,10.0);
    
    
    while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
        
		// check for mouse moved message
		if ( m.getAddress() == "/a" )
		{
           /* cout << vel_yin << endl;
            
			// both the arguments are int32's
			vel_yin =   m.getArgAsFloat( 0 );
            vel_yin = 1 + (500 - 1) * (vel_yin - 500) / (700 - 500);
            vel_yin = vel_yin > 500 ? 500 : vel_yin;
            vel_yin = vel_yin < 1 ? 1 : vel_yin;
            */
            cout << vel_yin << endl;
		}
        
    }
    
    
    int i = 0;
    float slice;
    
    for( vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p ){
        
        if(i < num_particles_display){
            
            if(i < num_particles_display / 3.0){
                slice = slice_1;
            }else if(i > 2 * num_particles_display / 3.0){
                slice = slice_2;
            }else{
                slice = slice_3;
            }
            
            if(isYang( p->x, p->y )){
                if(yinyang){
                    p->update( flock_yin, vel_yin, acc_yin, slice, true);
                }else{
                    p->update( flock_yang, vel_yang, acc_yang, slice, true);
                }
               
                
                if(ofRandom(1, 1000) < 10)
                    p->count = 20;
            }
            else{
                if(p->count == 0 && ofRandom(1, 16000) < 10)
                    p->count = 50;
                
                p->update( flock_yin, vel_yin, acc_yin, slice, false);
            }
            
            slice_1 += 0.000005;
            slice_2 += 0.000003;
            slice_3 += 0.000001;
            
        }else{
            break;
        }
        
        i++;
    }
    
      
    fbo.begin();
    ofSetColor(255,255,255,255);
    switch (steps) {
        case 0:
            sequence1.getFrameForTime(ofGetElapsedTimef())->draw(0,0);
            break;
        case 1:
            img1.draw(0,0,800,600);
            break;
        case 2:
            img2.draw(0,0,800,600);
            break;
        case 3:
            img3.draw(0,0,800,600);
            break;
        case 4:
            img4.draw(0,0,800,600);
            break;
        case 5:
            img5.draw(0,0,800,600);
            break;
            
        default:
            break;
    }
    
    glPixelStorei(GL_PACK_ALIGNMENT, 1);  
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);  
    grabbed.setFromPixels(pixels, width, height, OF_IMAGE_COLOR_ALPHA);
    
    fbo.end();
    
    /*
    fluid.begin();
    
    for(i=0; i < 40; i++){
        if(i<20){
        fluid.addTemporalForce(particles[i].pos, ofVec2f(0,0), ofFloatColor(255));
        }else{
             fluid.addTemporalForce(particles[i].pos, ofVec2f(0,0), ofFloatColor(0));
        }
        
    }
    
    fluid.end();
    fluid.update();
    */
    if(slice_1 > 2000){
        slice_1 = 0;
    }
    if(slice_2 > 2000){
        slice_2 = 0;
    }
    if(slice_3 > 2000){
        slice_3 = 0;
    }
    
    /* window title */
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    if(shader && !guivisible){
        ofSetColor(0, 0, 0, trail);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    if(guivisible){
        kontrol.draw();
        //cout << ofToString(kontrol.channel[0].slider) << endl;
    }
    
    int i = 0;
    
    for( vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p ){
        
        if(i < num_particles_display){
            
            int escala = 10000;
            
            if(p->yang || p->count > 0) {
                /*ofSetColor( (int)(slice_1 * escala) % 255,(int)(slice_2 * escala) % 255, (int)(slice_3 * escala) % 255);
                p->r = (int)(slice_1 * escala) % 255;
                p->g = (int)(slice_2 * escala) % 255;
                p->b = (int)(slice_3 * escala) % 255;*/
                ofSetColor(255);
                p->r = 255;
                p->g = 255;
                p->b = 255;
            }
            else {
                ofSetColor(contrast);
            }
            p->draw( zoom, 0.5 );
            
        }else{
            break;
        }
        
        i++;
    }
    
}

bool testApp::isYang(float x, float y) {
    
    int w = ofGetWidth();
    int h = ofGetHeight();
    
    x = ofClamp(x, 0, w);
    y = ofClamp(y, 0, h);
    
    x = x / w * 800;
    y = y / h * 600;
    /*
    float ww = h / 600 * 800;
    float dif = (w - ww) / 2;
    
    x = (ofClamp(x, dif, dif+ww) - dif) / ww * 800;
    */   
    ofColor cor = grabbed.getColor((int)x, (int)y);

    //cout << ofToString(cor.r + cor.g + cor.b) << endl;

    if (cor.getBrightness() > 70) 
        return true;
    else
        return false;
}


void testApp::exit()
{
	gui->saveSettings("GUI/guiSettings.xml");     
    delete gui;
}

void testApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "PARTICLES")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        num_particles_display = int(slider->getScaledValue());
    }
	else if(e.widget->getName() == "VEL_YANG")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        vel_yang = slider->getScaledValue() / 100;
    }
    else if(e.widget->getName() == "ACC_YANG")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        acc_yang = slider->getScaledValue() / 10000;
    }
    else if(e.widget->getName() == "FLOCK_YANG")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        flock_yang = slider->getScaledValue();
    }
    else if(e.widget->getName() == "VEL_YIN")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        vel_yin = slider->getScaledValue() / 100;
    }
    else if(e.widget->getName() == "ACC_YIN")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        acc_yin = slider->getScaledValue() / 10000;
    }
    else if(e.widget->getName() == "FLOCK_YIN")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        flock_yin = slider->getScaledValue();
    }
    else if(e.widget->getName() == "ZOOM")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        zoom = slider->getScaledValue();
    }
    else if(e.widget->getName() == "TRAIL")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        trail = int(slider->getScaledValue());
    }
    else if(e.widget->getName() == "CONTRAST")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        contrast = int(slider->getScaledValue());
    }
    else if(e.widget->getName() == "YIN&YANG")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        yinyang = toggle->getValue(); //ofSetFullscreen(toggle->getValue());
    }
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if(key == 32){
        
        gui->toggleVisible();
        guivisible = gui->isEnabled();
        
        if(guivisible){
            ofSetBackgroundAuto(true);
        }else{
            if(shader){
                ofSetBackgroundAuto(false);
            }
        }
        
    }else if(key == 116){
        
        shader = !shader;
        if(!guivisible)
            ofSetBackgroundAuto(!shader);
        
    }else if(key == 102){
        ofToggleFullscreen();
    }
    else if(key == 358){
        steps++;
    }
    else if(key == 356){
        steps--;
    }
    
    cout << "key pressed: " << key << endl;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    ofColor c = grabbed.getColor(x,y);
    
    cout << c.getBrightness() << endl;
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
