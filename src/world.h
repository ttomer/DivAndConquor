//
// Created by Dmitry on 2019-01-25.
//

#ifndef DIVCONQ_WORLD_H
#define DIVCONQ_WORLD_H

#include <vector>
#include <string>

#include "drawable.h"
#include "Camera.h"
#include "GameObject.h"
#include "PlayerBoat.h"

class Camera;

class World : Drawable
{
public:
  World(int w, int h, Camera *camera);

  ~World();

  void update() override;

  void draw(DrawContext *dc) override;

  Camera *camera;

  void onClick(float x, float y);

  PlayerBoat* playerBoat;

private:
  std::vector<std::vector<int>> tilemap;
  std::vector<std::string> savedFiles;

  std::vector<GameObject *> gameObjects;
};

#endif //DIVCONQ_WORLD_H
