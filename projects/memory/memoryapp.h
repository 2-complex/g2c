
#ifndef _MEMORYAPP_
#define _MEMORYAPP_

#include "app.h"
#include "opengl.h"

#include "bank.h"
#include "sprites.h"

#include <vector>

using namespace std;
using namespace g2c;


class Card {
public:
	Card(Sprite* cardSprite);
	virtual ~Card();
	
	int color;
	int refCount;
	Actor actor;
};


class ZipAnimation : public Animation {
public:
	ZipAnimation(double start, double duration, Actor* actor, const Vec2& target);
	virtual ~ZipAnimation();
	
	Actor* actor;
	Vec2 source;
	Vec2 target;
	
	Vec2 velocity;
	Vec2 direction;
	Vec2 position;
	
	virtual void begin();
	virtual void step(double t);
	virtual void end();
};


class FlipAnimation : public Animation {
public:
	FlipAnimation(double start, double duration, Card* card, int startFrame, int endFrame);
	virtual ~FlipAnimation();
	
	Card* card;
	int startFrame;
	int endFrame;
	
	virtual void begin();
	virtual void step(double t);
	virtual void end();
};


class BlinkAnimation : public Animation {
public:
	BlinkAnimation(double start, double duration, Card* card);
	virtual ~BlinkAnimation();
	
	Card* card;
	
	virtual void begin();
	virtual void step(double t);
	virtual void end();
};


class ExplosionAnimation : public Animation {
public:
	ExplosionAnimation(double instart, double induration,
		Layer* layer, Sprite* sprite, int frame, Vec2 position, Vec2 direction);
	virtual ~ExplosionAnimation();
	
	virtual void begin();
	virtual void step(double t);
	virtual void end();
private:
	
	Actor* actor;
	Vec2 position;
	Vec2 direction;
	Layer* layer;
};



class MemoryApp : public App {
public:
	MemoryApp();
	virtual ~MemoryApp();
	
	Bank* bank;
	
	double height;
	double width;
	
	virtual void init();
	virtual void reshape(int w, int h);
	virtual void draw() const;
	virtual void step(double t);
	
	virtual bool mouseDown(const Vec2& c);
	virtual void mouseDragged(const Vec2& c);
	virtual void mouseUp(const Vec2& c);

private:
	double lastTime;
	World world;
	Animator animator;
	
	void touch(const Vec2& c);
	
	set<Card*> open;
	set<FlipAnimation*> flips;
	
	Sprite* cardSprite;
	Sprite* blinkSprite;
	Sprite* explosionSprite;
	
	Layer* gameLayer;
	Layer* explosionLayer;
	
	vector<Card*> cards;
};

#endif

