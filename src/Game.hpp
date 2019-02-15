#ifndef DIVCONQ_GAME_H
#define DIVCONQ_GAME_H

#include <vector>
#include "Common.hpp"
#include "World/World.hpp"

class Game
{
  public:
    void init();
    void update();
    void draw(const mat3 &projection);
    void onKey(int key, int scancode, int action);

  private:
    World *world;
};

#endif //DIVCONQ_GAME_H
