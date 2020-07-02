#pragma once

#include <vector>
#include <iostream>
using namespace std;

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
using namespace sf;

class Mark : public Drawable {
    VertexArray _points;
    Color _markColor;

   public:

    Mark() {};
    Mark(Color c);
    void addPoint(Vector2f pos);
    void draw(RenderTarget& target, RenderStates states) const override;

    VertexArray& points() { return _points; }
    Color color() { return _markColor; }

    friend Packet& operator>>(Packet& p, Mark& m);
    friend Packet& operator<<(Packet& p, Mark& m);
};

void sendMark(Mark m);
void sendClear();
Mark recvMark();

void recvThread();
