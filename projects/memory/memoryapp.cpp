
#include "memoryapp.h"
#include "transforms.h"


ZipAnimation::ZipAnimation(double start, double duration, Actor* actor, const Vec2& target) :
	Animation(start, duration), actor(actor), target(target)
{
	source = Vec2(actor->x, actor->y);
	direction = target - source;
	position = source;
}

ZipAnimation::~ZipAnimation()
{
	
}

void ZipAnimation::begin()
{
	actor->Vec2::operator=(source);
}

void ZipAnimation::step(double t)
{
	position = source + t*t*direction;
	actor->Vec2::operator=(position);
}

void ZipAnimation::end()
{
	actor->Vec2::operator=(target);
}


FlipAnimation::FlipAnimation(double start, double duration, Card* card, int startFrame, int endFrame) :
	Animation(start, duration), card(card), startFrame(startFrame), endFrame(endFrame)
{
	card->refCount++;
}

FlipAnimation::~FlipAnimation()
{
	card->refCount--;
}

void FlipAnimation::begin()
{
}

void FlipAnimation::step(double t)
{
	int k = t * 8;
	
	if( k < 4 )
	{
		card->actor.frame = startFrame + 4*k;
		card->actor.visible = true;
	}
	else if(k == 4)
	{
		card->actor.visible = false;
	}
	else
	{
		card->actor.frame = endFrame - 4*(k + 1);
		card->actor.visible = true;
	}
}

void FlipAnimation::end()
{
	card->actor.frame = endFrame;
	card->actor.visible = true;
}



BlinkAnimation::BlinkAnimation(double start, double duration, Card* card) :
	Animation(start, duration), card(card)
{
}

BlinkAnimation::~BlinkAnimation()
{
	card->refCount--;
}

void BlinkAnimation::begin()
{
	card->actor.visible = true;
}

void BlinkAnimation::step(double t)
{
	double m = (t * duration);
	int k = 15*m;
	
	card->actor.visible = true;
	
	if( k % 2 )
		card->actor.frame = card->color;
	else
		card->actor.frame = card->color + 4;
}

void BlinkAnimation::end()
{
}


ExplosionAnimation::ExplosionAnimation( double instart, double induration,
	Layer* layer, Sprite* sprite, int frame, Vec2 position, Vec2 direction ) :
	Animation(instart, induration),
	layer(layer),
	position(position),
	direction(direction)
{
	actor = new Actor(sprite);
	actor->frame = frame;
}

ExplosionAnimation::~ExplosionAnimation()
{
	layer->remove( actor );
	delete actor;
}

void ExplosionAnimation::begin()
{
	layer->add( actor );
	
	Vec2 p = position;
	
	actor->set(p.x, p.y);
	actor->k = 1.2;
	actor->color = Color(1,1,1,1.0);
}

void ExplosionAnimation::step(double t)
{
	Vec2 p = position + t * direction;
	actor->set(p.x, p.y);
	actor->k = 1.2*(1.0 - t);
	actor->color = Color(2,2,2,1.0 - t);
}

void ExplosionAnimation::end()
{
	Vec2 p = position + direction;
	actor->set(p.x, p.y);
}




Card::Card(Sprite* cardSprite) : color(1), refCount(1)
{
	actor.sprite = cardSprite;
}

Card::~Card()
{
	
}


MemoryApp::MemoryApp() :
	bank(NULL),
	cardSprite(NULL),
	explosionSprite(NULL),
	blinkSprite(NULL),
	gameLayer(NULL),
	explosionLayer(NULL),
	width(0.0),
	height(0.0)

{
}

MemoryApp::~MemoryApp()
{
	if( Sprite::renderer )
	{
		delete Sprite::renderer;
		Sprite::renderer = NULL;
	}
}

void MemoryApp::reshape(int w, int h)
{
	width = w;
	height = h;
	
	Sprite::renderer->projection = orthographic(-0.5*w, 0.5*w, -0.5*h, 0.5*h, -1, 1);
}

void MemoryApp::init()
{
	Sprite::renderer = new RendererGL1;
	Sprite::renderer->init();
	
	glDisable(GL_DEPTH_TEST);
	
	world.bank = bank;
	bank->initSerializableWithPath(&world, "memory.world");
	
	cardSprite = world.getSprite("card");
	blinkSprite = world.getSprite("blink");
	explosionSprite = world.getSprite("explosion");
	
	gameLayer = dynamic_cast<Layer*>(world.getNode("gameLayer"));
	explosionLayer = dynamic_cast<Layer*>(world.getNode("explosionLayer"));
	
	int counter = 0;
	
	for( int x = -3; x < 4; x++ )
	for( int y = -3; y < 4; y++ )
	{
		Card* newcard = new Card(cardSprite);
		
		newcard->color = 1 + randInt(3);
		
		newcard->actor.k = 50.0 / 70.0;
		newcard->actor.x = x*50;
		newcard->actor.y = y*50 + 500;
		
		animator.add(new ZipAnimation(
			counter * 0.005, 0.6, &(newcard->actor), Vec2(x*50, y*50)));
		
		cards.push_back(newcard);
		
		gameLayer->add(&(newcard->actor));
		
		counter++;
	}
}

void MemoryApp::step(double t)
{
	if( t < lastTime + 1.0 / 60.0 )
		return;
	
	animator.step(t);
	
	vector<Card*> newCards;
	
	for( vector<Card*>::iterator itr = cards.begin(); itr != cards.end(); itr++ )
	{
		Card* card = *itr;
		if( card->refCount > 0 )
			newCards.push_back(card);
		else
		{
			gameLayer->remove(&(card->actor));
			delete card;
		}
	}
	
	cards = newCards;
	
	lastTime = t;
}

void MemoryApp::draw() const
{
	world.draw();
}

void MemoryApp::touch(const Vec2& c)
{
	Vec2 p = c - 0.5*Vec2(width, height);
	
	for( vector<Card*>::iterator itr = cards.begin(); itr != cards.end(); itr++ )
	{
		Card* card = *itr;
		
		Vec2 cp(card->actor.x, card->actor.y);
		
		if( (cp - p).mag() < 25.0 )
		{
			if( card->actor.frame == 0 )
				animator.add(new FlipAnimation(0, 0.25, card, 0, card->color));
			
			open.insert(card);
		}
	}
}

bool MemoryApp::mouseDown(const Vec2& c)
{
	touch(c);
	return true;
}

void MemoryApp::mouseDragged(const Vec2& c)
{
	touch(c);
}

void MemoryApp::mouseUp(const Vec2& c)
{
	int color = 0;
	bool unicolor = true;
	
	for( set<Card*>::iterator itr = open.begin(); itr!=open.end(); itr++ )
	{
		Card* card = *itr;
		
		if( color && color != card->color )
			unicolor = false;
		
		color = card->color;
	}
	
	if( !unicolor )
	{
		for( set<Card*>::iterator itr = open.begin(); itr!=open.end(); itr++ )
		{
			Card* card = *itr;
			animator.add(new FlipAnimation(0, 0.25, card, card->color, 0));
		}
		
		open.clear();
	}
	
	if( unicolor && open.size() >= 3 )
	{
		int count = 0;
		set<Card*> stragglers;
		
		for( vector<Card*>::iterator itr = cards.begin(); itr!=cards.end(); itr++ )
		{
			Card* card = *itr;
			
			if( open.find(card) != open.end() )
			{
				card->actor.sprite = blinkSprite;
				animator.add(new BlinkAnimation(0, 1.0 + count/3.0, card));
				
				int n = 7;
				for( int i=0; i<n; i++ )
				{
					double theta = i * 2.0 * M_PI / n;
					
					animator.add(new ExplosionAnimation(1.0 + count/3.0,
						0.4, explosionLayer,
						explosionSprite, card->color,
						Vec2(card->actor.x, card->actor.y),
						150 * Vec2(cos(theta), sin(theta)) ));
				}
				
				count++;
			}
			else if( card->color == color )
				stragglers.insert(card);
		}
		
		if( stragglers.size() < 3 )
		{
			for( set<Card*>::iterator itr = stragglers.begin(); itr!=stragglers.end(); itr++ )
			{
				Card* card = *itr;
				
				card->actor.sprite = blinkSprite;
				animator.add(new BlinkAnimation(0, 1.0 + count/3.0, card));
				count++;
				
				int n = 7;
				for( int i=0; i<n; i++ )
				{
					double theta = i * 2.0 * M_PI / n;
					
					animator.add(new ExplosionAnimation(1.0 + count/3.0,
						0.4, explosionLayer,
						explosionSprite, card->color,
						Vec2(card->actor.x, card->actor.y),
						150 * Vec2(cos(theta), sin(theta)) ));
				}
			}
		}
		
		open.clear();
	}
}



