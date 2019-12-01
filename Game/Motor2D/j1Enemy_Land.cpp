#include "j1App.h"
#include "j1Player.h"
#include "j1Enemy_Land.h"
#include "j1Collisions.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Render.h"
#include <cmath>


j1Enemy_Land::j1Enemy_Land(ENTITY_TYPE type, float x, float y) : j1Entity(ENTITY_TYPE::ENEMY_AIR, x, y)
{
	//idle
	run.PushBack({ 0, 0, 32, 32 });
	run.PushBack({ 32, 0, 32, 32 });
	run.PushBack({ 64, 0, 32, 32 });
	run.PushBack({ 96, 0, 32, 32 });
	run.PushBack({ 128, 0, 32, 32 });
	run.PushBack({ 160, 0, 32, 32 });
	run.PushBack({ 192, 0, 32, 32 });
	run.PushBack({ 224, 0, 32, 32 });
	run.PushBack({ 256, 0, 32, 32 });
	run.PushBack({ 288, 0, 32, 32 });
	run.PushBack({ 320, 0, 32, 32 });
	run.PushBack({ 352, 0, 32, 32 });
	run.PushBack({ 384, 0, 32, 32 });
	run.PushBack({ 416, 0, 32, 32 });
	run.PushBack({ 448, 0, 32, 32 });
	run.PushBack({ 480, 0, 32, 32 });

	runM.PushBack({ 0, 0, 32, 32 });
	runM.PushBack({ 32, 0, 32, 32 });
	runM.PushBack({ 64, 0, 32, 32 });
	runM.PushBack({ 96, 0, 32, 32 });
	runM.PushBack({ 128, 0, 32, 32 });
	runM.PushBack({ 160, 0, 32, 32 });
	runM.PushBack({ 192, 0, 32, 32 });
	runM.PushBack({ 224, 0, 32, 32 });
	runM.PushBack({ 256, 0, 32, 32 });
	runM.PushBack({ 288, 0, 32, 32 });
	runM.PushBack({ 320, 0, 32, 32 });
	runM.PushBack({ 352, 0, 32, 32 });
	runM.PushBack({ 384, 0, 32, 32 });
	runM.PushBack({ 416, 0, 32, 32 });
	runM.PushBack({ 448, 0, 32, 32 });
	runM.PushBack({ 480, 0, 32, 32 });


}

j1Enemy_Land::~j1Enemy_Land() {};

bool j1Enemy_Land::Start() {
	img = App->tex->Load("textures/enemy_masked_guy.png");
	imgM = App->tex->Load("textures/enemy_masked_guy_mirror.png");

	collider = App->collisions->AddCollider({ position.x, position.y, 25, /*TODO: INITIALIZE IPoint for these*/ 23 }, COLLIDER_ENEMY, App->entity_manager);

	speed_following = { 0.5f, 0.5f };

	return true;
}
bool j1Enemy_Land::CleanUp() {
	App->tex->UnLoad(img);
	App->tex->UnLoad(imgM);

	collider->to_delete = true;

	return true;
}
bool j1Enemy_Land::PreUpdate() {

	BROFILER_CATEGORY("EnemyLand_PreUpdate", Profiler::Color::GreenYellow)

		return true;
}

bool j1Enemy_Land::Update(float dt) {

	BROFILER_CATEGORY("EnemyLand_Update", Profiler::Color::ForestGreen)

	if (position.x > App->player->position.x) flip = true;
	else flip = false;

	if (isAlive) Draw({ 0, 0, 25 /*20*/, 23 /*20*/ }, position.x, position.y, flip);

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (distance > 100) {
		move_direction = -1;
	}
	else if (distance < 0) {
		move_direction = +1;
	}
	else {
		position.x += move_direction;
		if (move_direction > 0) distance--;
		else distance++;
	}

	App->render->Blit(img, position.x, position.y, &(current_animation->GetCurrentFrame()));
	return true;
}

bool j1Enemy_Land::PostUpdate() {

	BROFILER_CATEGORY("EnemyLand_PostUpdate", Profiler::Color::LawnGreen)

		return true;
}

void j1Enemy_Land::Move(p2DynArray<iPoint>& path, float dt)
{
	direction = App->pathfinding->WillMoveTo(path);

	if (direction == MOVE_TO::PATH_8)
	{
		position.y -= speed.y * dt;
	}
	if (direction == MOVE_TO::PATH_2)
	{
		position.y += speed.y * dt;
	}
	if (direction == MOVE_TO::PATH_4)
	{
		position.x -= speed.x * dt;
	}
	if (direction == MOVE_TO::PATH_6)
	{
		position.x += speed.x * dt;
	}


}

void j1Enemy_Land::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_ENEMY) {
		if (c2->type == COLLIDER_PLAYER_SHOT) {
			isAlive = false;
			c2->to_delete = true;
			c1->to_delete = true;
		}
	}
}

void IdleMovement() {}