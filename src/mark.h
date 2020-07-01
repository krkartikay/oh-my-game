#pragma once

#include <vector>
#include <iostream>
using namespace std;

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
using namespace sf;

class Mark : public Drawable {
    VertexArray points;
    Color markColor;

   public:
    Mark(Color c);
    void addPoint(Vector2f pos);
    void draw(RenderTarget& target, RenderStates states) const override;
};