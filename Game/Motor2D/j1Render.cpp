#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Scene.h"
#include "j1Input.h"
#define VSYNC true

j1Render::j1Render() : j1Module() {
	name.create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
j1Render::~j1Render() {}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config) {
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true) {
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if(renderer == NULL) {
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else {
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;

		if (App->player != nullptr) {
			camera.x = -App->player->position.x;
			camera.y = -App->player->position.y;
		}

	
		//camera.x = App->player->position.x;
		//camera.y = App->player->position.y;
	}

	return ret;
}

// Called before the first frame
bool j1Render::Start() {
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);

	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate() {
	SDL_RenderClear(renderer);

	return true;
}

bool j1Render::Update(float dt) {
	camera.x = -(App->player->position.x + velocity.x) + App->win->width / 2;
	camera.y = -(App->player->position.y + velocity.y) + App->win->height / 1.40;


	/*if (camera.x > 0) {
		camera.x = 0;
	}*/

	if (camera.x > 600) {
		camera.x = 600;
	}
	if (camera.y > 0) {
		camera.y = 0;
	}

	//limites inferiores faltan

	velocity.x = App->player->velocity.x;
	velocity.y = App->player->velocity.y;

	//float speed = 1.7F;
	//if (App->input->GetKey(SDL_SCANCODE_7) == KEY_REPEAT) camera.x += speed;
	//if (App->input->GetKey(SDL_SCANCODE_8) == KEY_REPEAT) camera.x -= speed;
	//if (App->input->GetKey(SDL_SCANCODE_9) == KEY_REPEAT) camera.y += speed;
	//if (App->input->GetKey(SDL_SCANCODE_0) == KEY_REPEAT) camera.y -= speed;

	//if (App->player->velocity.x<0 || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	//	if (camera.x < 0 &&	App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT) camera.x -= -speed;

	//if (App->player->velocity.x > 0 || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	//	if (camera.x < 1000 && App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT) camera.x += +speed;

	//if (App->player->velocity.y < 0 || App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	//	if (camera.y > 0) camera.y -= speed;

	//if (App->player->velocity.y > 0 || App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//	if (camera.y < 0) camera.y += speed;



	return true;
}

bool j1Render::PostUpdate() {
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);

	return true;
}

// Called before quitting
bool j1Render::CleanUp() {
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);

	return true;
}


// TODO 6: Create the load method on the renderer. For now load camera's x and y
bool j1Render::Load(pugi::xml_node& saved)
{
	camera.x = saved.child("camera").attribute("x").as_int();
	camera.y = saved.child("camera").attribute("y").as_int();
	App->LoadRequest = false;

	return true;
}

// TODO 8: Create the save method on the renderer. Fill the camera's data
// using append_child and append_attribute
bool j1Render::Save(pugi::xml_node& data) {
	pugi::xml_node cam = data.append_child("camera");
	cam.append_attribute("x").set_value(camera.x);
	cam.append_attribute("y").set_value(camera.y);
	App->SaveRequest = false;

	return true;
}

iPoint j1Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = App->win->GetScale();

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}

void j1Render::SetBackgroundColor(SDL_Color color) {
	background = color;
}

void j1Render::SetViewPort(const SDL_Rect& rect) {
	SDL_RenderSetViewport(renderer, &rect);
}

void j1Render::ResetViewPort() {
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivot_x, int pivot_y) const {
	bool ret = true;
	uint scale = App->win->GetScale();
	SDL_Rect rect;
	rect.x = (int) (camera.x * speed) + x * scale;
	rect.y = (int) (camera.y * speed) + y * scale;

	if(section != NULL) {
		rect.w = section->w;
		rect.h = section->h;
	}
	else {
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivot_x != INT_MAX && pivot_y != INT_MAX) {
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0) {
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const {
	bool ret = true;
	uint scale = App->win->GetScale();
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_Rect rec(rect);

	if(use_camera) {
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0) {
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const {
	bool ret = true;
	uint scale = App->win->GetScale();
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	int result = -1;

	if(use_camera) result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0) {
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const {
	bool ret = true;
	uint scale = App->win->GetScale();
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	int result = -1;
	SDL_Point points[360];
	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i) {
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0) {
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::IsOnCamera(int x, int y, int w, int h, int player_position)
{
	bool ret = false;

	int camera_width = App->win->width / 2;
	SDL_Rect tile_to_print = { App->map->MapToWorld(x,y).x, App->map->MapToWorld(x,y).y, w, h };

	if (player_position - camera_width <= tile_to_print.x && player_position + camera_width >= tile_to_print.x)
		ret = true;

	else ret = false;

	return ret;
}

bool j1Render::CameraCulling(int x, int y, int w, int h, int camera_position)
{
	bool ret = false;

	iPoint variation;

	if (!App->scene->cullingView)
		variation = { App->map->culling_variation };
	else
		variation = { App->map->culling_view };

	int camera_width = App->win->width;
	int scale = App->win->scale;
	float speed = App->map->parallax_speed;

	SDL_Rect tile_to_print = { App->map->MapToWorld(x,y).x, App->map->MapToWorld(x,y).y, w, h };

	if ((camera_position * speed - variation.x) / scale <= tile_to_print.x && (camera_position * speed - variation.y) / scale + camera_width >= tile_to_print.x)
		ret = true;

	else ret = false;

	return ret;
}
