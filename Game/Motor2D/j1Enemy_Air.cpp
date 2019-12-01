#include "j1App.h"
#include "j1Player.h"
#include "j1Enemy_Air.h"
#include "j1Collisions.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Map.h"
#include <cmath>


j1Enemy_Air::j1Enemy_Air(ENTITY_TYPE type, float x, float y) : j1Entity(ENTITY_TYPE::ENEMY_AIR, x, y)
{
}

j1Enemy_Air::~j1Enemy_Air() {};

bool j1Enemy_Air::Start() {
	img = App->tex->Load("textures/enemy_ghost_angry.png");
	imgM = App->tex->Load("textures/enemy_ghost_angry_mirror.png");

	collider = App->collisions->AddCollider({ position.x, position.y, 25, /*TODO: INITIALIZE IPoint for these*/ 23 }, COLLIDER_ENEMY, App->entity_manager);

	speed_following = { 0.5f, 0.5f };

	return true;
}
bool j1Enemy_Air::CleanUp() {
	App->tex->UnLoad(img);
	App->tex->UnLoad(imgM);

	collider->to_delete = true;

	return true;
}
bool j1Enemy_Air::PreUpdate() {

	BROFILER_CATEGORY("EnemyAir_PreUpdate", Profiler::Color::GreenYellow)

	return true;
}

bool j1Enemy_Air::Update(float dt) {

	BROFILER_CATEGORY("EnemyAir_Update", Profiler::Color::ForestGreen)

	if (position.x > App->player->position.x) flip = true;
	else flip = false;

	if (isAlive) Draw({0, 0, 25 /*20*/, 23 /*20*/}, position.x, position.y, flip);	
	
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	//if (App->player->col != nullptr) {
	//	if (abs(App->player->position.x - position.x) <= attack_radar_distance || abs(App->player->position.y - position.y) <= attack_radar_distance && App->player->col->type == COLLIDER_PLAYER)
	//	{
	//		iPoint origin = { App->map->WorldToMap((int)position.x + rePathing.x, (int)position.y + rePathing.y) };
	//		iPoint destination;

	//		if (position.x < App->player->position.x)
	//			destination = { App->map->WorldToMap((int)(App->player->position.x + App->player->col->rect.x), (int)(App->player->position.y + App->player->col->rect.y / 2)) };
	//		else
	//			destination = { App->map->WorldToMap((int)(App->player->position.x), (int)(App->player->position.y + App->player->col->rect.y)) };

	//		path = App->pathfinding->CreatePath(origin, destination);
	//		Move(*path, dt);
	//		has_path = true;

	//	}

	//	else if (has_path)
	//	{
	//		path->Clear();
	//		has_path = false;
	//	}
	//}

	return true;
}

bool j1Enemy_Air::PostUpdate() {

	BROFILER_CATEGORY("EnemyAir_PostUpdate", Profiler::Color::LawnGreen)

	return true;
}

void j1Enemy_Air::Move(p2DynArray<iPoint>& path, float dt)
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

void j1Enemy_Air::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_ENEMY) {

		if (c2->type == COLLIDER_PLAYER_SHOT)
		{
			isAlive = false;
			c2->to_delete = true;
			c1->to_delete = true;
		}
	}

	if (c1->type == COLLIDER_ENEMY) {

		if (c2->type == COLLIDER_PLAYER && !App->player->godMode) {

			if (App->scene->current_scene == 0) {
				App->player->position.x = App->player->originalPosition_1.x;
				App->player->position.y = App->player->originalPosition_1.y;
			}
			else if (App->scene->current_scene == 1) {
				App->player->position.x = App->player->originalPosition_1.x;
				App->player->position.y = App->player->originalPosition_1.y;
			}
			else if (App->scene->current_scene == 2) {
				App->player->position.x = App->player->originalPosition_2.x;
				App->player->position.y = App->player->originalPosition_2.y;
			}
		}
	}

}