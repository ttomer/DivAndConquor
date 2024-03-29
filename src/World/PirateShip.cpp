#include "GameObject.hpp"

#define SHIP_VELOCITY 1

PirateShip::PirateShip(World* world, vec2 loc) : GameObject(world, loc) {
    w = 100;
    h = 100;
    ticks = std::rand() % 120;
    if (!sprite.init(w, h, textures_path("pirateship.png"), {0.5f, 1.f})) {
        printf("ERROR initializing sprite\n");
    }
    canShoot = true;
    rotation = 0;
    scale = {1.f, 1.f};
    landUnit = false;
    playerControlled = false;
    pathfinder = new Pathfinder(world, landUnit, true);
    health = maxHealth = 300;
    world->pirateStrength++;
    printf("enemyNum %d\n", world->pirateStrength);
    pathfindingDelay = 120;
}

void PirateShip::update() {
    vec2 position = getPosition();
    GameObject::update();

    if (world->state == flee){
        GameObject* o = world->getClosestObject(position, true, false);
        if (o && abs(o->position.x - position.x) < 800 && abs(o->position.y - position.y) < 800) {
            vec2 targetPos = o->getPosition();
            TilePos start = Tilemap::getTilePos(position.x, position.y);
            TilePos goal = Tilemap::getTilePos(world->w - 100, world->h - 100);
            pathfinder->init(start.x, start.y, goal.x - 400, goal.y - 400);
            pathfinder->replan();
            path = pathfinder->getPath();
        }
    }


    if (ticks % pathfindingDelay == 0) {
        GameObject* o = world->getClosestObject(position, true, false);
        if (o) {
            double temp;
            double min = abs(position.x - o->getPosition().x);
            double max = abs(position.y - o->getPosition().y);
            if (min > max) {
                temp = min;
                min = max;
                max = temp;
            }
            double dist = ((M_SQRT2 - 1.0) * min + max);
            pathfindingDelay = static_cast<int>(dist * 0.048);
            if (pathfindingDelay < 120) pathfindingDelay = 120;

            if (world->state != flee) {
                vec2 targetPos = o->getPosition();
                TilePos start = Tilemap::getTilePos(position.x, position.y);
                TilePos goal = Tilemap::getTilePos(targetPos.x, targetPos.y);
                pathfinder->init(start.x, start.y, goal.x, goal.y);
                pathfinder->replan();
                path = pathfinder->getPath();
            }
            world->fireOnClosestObject(this, true, true);
        }
    }


    if (!path.path.empty()) {
        auto next = std::next(path.path.begin());
        if (next != path.path.end()) {
            float destX = next->x * TILE_SIZE;
            float destY = next->y * TILE_SIZE;
            if (abs(position.x - destX) <= SHIP_VELOCITY && abs(position.y - destY) <= SHIP_VELOCITY) {
                pathfinder->updateStart(next->x, next->y);
                pathfinder->replan();
                path = pathfinder->getPath();
            } else {
                travel({destX, destY});
            }
        }
    }

    TilePos tilePos = Tilemap::getTilePos(position.x, position.y);
    if (world->tilemap.map[tilePos.x][tilePos.y].type != 0) {
        vec2 pos = {(float) tilePos.x * TILE_SIZE, (float) tilePos.y * TILE_SIZE};
        float difX = position.x - pos.x;
        float difY = position.y - pos.y;
        addForce({difX * 0.5f, difY * 0.5f});
    }

    if (health <= 0) {
        if(world->lock == this){
            world->lock = nullptr;
        }
        world->pirateStrength--;
        destroy();
    }

    sprite.tint = {1.f, health / maxHealth, health / maxHealth};
}

void PirateShip::travel(vec2 destination) {
    vec2 position = getPosition();
    vec2 dir = {destination.x - position.x, destination.y - position.y};
    float length = sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length > SHIP_VELOCITY) {
        dir = {dir.x / length, dir.y / length};
        this->rotation = atan2(dir.y, dir.x);
        if (abs(rotation) > M_PI / 2) {
            this->sprite.state = 1;
            rotation = atan2(-dir.y, -dir.x);
        } else {
            this->sprite.state = 0;
        }

        addForce({dir.x * SHIP_VELOCITY, dir.y * SHIP_VELOCITY});
    }
}

void PirateShip::collide(GameObject* obj) {
    if(obj->playerControlled){
        vec2 pos = obj->getPosition();
        vec2 position = getPosition();
        float difX = position.x - pos.x;
        float difY = position.y - pos.y;
        float mul = 0.05f;

        if (obj->playerControlled) {
            obj->health -= 7;
            mul = 0.2f;
        }

        addForce({difX * mul, difY * mul});
    }

}