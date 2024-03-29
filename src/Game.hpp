#ifndef DIVCONQ_GAME_H
#define DIVCONQ_GAME_H

#include <unordered_set>
#include <unordered_map>

#include "Common.hpp"
#include "World/World.hpp"
#include <cassert>
#include <utility>
#include <deque>
#include <vector>
#include <typeindex>
#include "Ui/Button.hpp"
#include "Ui/Label.hpp"
#include "Ui/UiCallback.h"


struct Settlement
{
  uint16_t gold;
  uint16_t timber;
  uint16_t iron;
  vec2 location;
  uint16_t influence;
};

struct Ship
{
  uint16_t capacity;
  uint16_t sailingDistance;
  uint16_t sailors;
  uint16_t maxSpeed;
  uint16_t price;
  uint16_t firearm;
};

static const Ship proa = {10, 10, 2, 4, 100, 0},
                  catboat = {15, 15, 2, 6, 200, 0},
                  sloop = {30, 30, 4, 10, 400, 1},
                  cutter = {30, 30, 4, 10, 800, 1},
                  yawl = {30, 30, 4, 10, 1600, 1},
                  ketch = {30, 30, 4, 10, 3200, 1},
                  schooner = {30, 30, 4, 10, 6400, 1},
                  topsailSchooner = {30, 30, 4, 10, 12800, 1},
                  brig = {30, 30, 4, 10, 25600, 1},
                  brigantine = {30, 30, 4, 10, 25600, 1},
                  barquentine = {30, 30, 4, 10, 25600, 1},
                  barque = {30, 30, 4, 10, 25600, 1},
                  fullyRiggedShip = {30, 30, 4, 10, 25600, 1};

// type forwarding since we have circular includes
class Button;

class Game
{
public:
  void init(vec2 screen);
  void update();
  void draw(const mat3 &projection, int pixelScale);
  void onKey(int key, int scancode, int action);
  bool registerButton(Sprite &btn, vec2 location, UiCallback::OnClickFunc onclick);
  // bool removeButton(Sprite *btn);
  void onClick(int button, int action, double xpos, double ypos);
  void onMouseMove(double xpos, double ypos);
  void onScroll(double xoffset, double yoffset, double xpos, double ypos);
  World *world;
  enum State
  {
    MAINMENU,
    PAUSE,
    RUNNING,
    WIN
  };
  Game::State state;
private:
    std::map<std::type_index, std::vector<UiElement *>> unitUis;

    TextRenderer tr;

    std::deque<std::pair<std::string, long>> logLines;
    std::vector<UiElement *> activeUiElements;
    std::vector<UiElement *> staticUiElements;
    std::vector<UiElement *> pauseScreenUiElements;
    std::unordered_set<Sprite *> selectedSprites; // TODO: these should be gameobjects maybe


    vec2 screen;

    double prevMouseXpos, prevMouseYpos;

  void drawUI(const mat3 &projection, int pixelScale);
public:
  void printLn(std::string line) {logLines.emplace_front(std::make_pair(line, currentTimeMs()));}
};

#endif //DIVCONQ_GAME_H
