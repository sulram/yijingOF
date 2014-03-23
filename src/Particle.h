#ifndef PARTICLE
#define PARTICLE

#import "ofMain.h"

class Particle {
    
private:
    
    
public:
	Particle();
    ofVec2f pos;
    ofVec2f vel;
    float x;
    float y;
    int r;
    int g;
    int b;
	void update(float flock, float vel, float acc, float slice, bool yang);
	void draw(float zoom, float zoomf);
    bool yang;
    int count;
};
#endif