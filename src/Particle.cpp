#include "Particle.h"

Particle::Particle()
{
	pos.set(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()));
    vel.set(0,0);
    x = 0;
    y = 0;
    count = 0;
    
    r = 0;
    g = 0;
    b = 0;
}

void Particle::update(float flock, float vel, float acc, float slice, bool yang)
{
    float posx = ofNoise(pos.x/flock, pos.y/flock, slice) * ofGetWidth();
    float posy = ofNoise(pos.x/flock, pos.y/flock, 2000-slice) * ofGetHeight();
    
    ofVec2f tmp;
    ofVec2f tmpacc;
    
    tmp.set(posx, posy);    
    tmpacc = tmp - pos;
    
    tmpacc.normalize();
    
    tmpacc *= acc;
    
    this->vel += tmpacc;
    this->vel.limit(vel);
    
    this->yang = yang;
    
    pos += this->vel;
    
    if(count>0){
        count--;
    }
}

void Particle::draw(float zoom, float zoomf)
{
    x = pos.x * zoom - ofGetWidth() * (zoomf * zoom -zoomf);
    y = pos.y * zoom - ofGetHeight() * (zoomf * zoom -zoomf);
    
	//ofCircle(x, y, 1-(abs(vel.x + vel.y))*.9);
    //ofCircle(x, y, 1);
    ofRect(x, y, 1, 1);
    if(this->yang){
        ofSetColor(r, g, b, 30); 
        ofCircle(x, y, 3);
    }
}