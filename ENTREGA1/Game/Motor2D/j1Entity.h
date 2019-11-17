#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "j1Animation.h"

struct SDL_Texture;
struct Collider;
class Animation;

enum ENTITY_TYPE;

class j1Entity
{
public:

	j1Entity(int x, int y, ENTITY_TYPE type);

	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();

	virtual bool Save();
	virtual bool Load();

	virtual void OnCollision(Collider* c1, Collider* c2) {};

public:

	ENTITY_TYPE type;

	fPoint position;

	Collider* collider = nullptr;
	Animation* animation = nullptr;
	SDL_Texture* sprites = nullptr;
};

#endif 
// __j1ENTITY_H__
