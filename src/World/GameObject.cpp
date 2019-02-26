#include "GameObject.hpp"

GameObject::GameObject(World* world) : world(world) {
    w = 75;
    h = 50;
    if (!sprite.init(w, h, textures_path("turtle.png")))
    {
        printf("ERROR initializing sprite\n");
    }
    position = {200, 200};
    rotation = 0;
    scale = {1.f, 1.f};
}

void GameObject::draw(const mat3 &projection)
{
    sprite.draw(projection, position, rotation, scale);

}

bounds GameObject::getBounds() {
    return {position.x - w / 2, position.x + w / 2, position.y - h / 2, position.y + h / 2};
}

void GameObject::move(vec2 pos) {
    position = pos;
}

void GameObject::update() {
    world->setExplored(position, 7 * TILE_SIZE);
}

void GameObject::setSelected() {
    sprite.selected = !sprite.selected;
}

bool GameObject::onTerrain(vec2 pos, int type){

    int rT = world->tilemap.getTile(pos.x + w/2, pos.y - h/2).type;
    int lT = world->tilemap.getTile(pos.x - w/2, pos.y - h/2).type;
    int rB = world->tilemap.getTile(pos.x + w/2, pos.y + h/2).type;
    int lB = world->tilemap.getTile(pos.x - w/2, pos.y + h/2).type;
    bool res = false;
    if (rT == type && (pos.x + w/2) < world->w - 1)
        if (rB == type && (pos.y + h/2) < world->h - 1)
            if (lB == type && (pos.x - w/2) >= 0)
                if (lT == type && (pos.y - h/2) >= 0)
                    res = true;

    return res;
}