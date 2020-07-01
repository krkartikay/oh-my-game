#pragma once

#include <vector>
#include <iostream>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

class Mark {
    VertexArray points;
    Color markColor;

   public:
    Mark(Color c);
    void addPoint(Vector2f pos);
    void draw(RenderWindow &w);
};