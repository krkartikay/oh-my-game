#include "mark.h"

Mark::Mark(Color c) : markColor(c) {
    points.setPrimitiveType(LineStrip);
}

void Mark::addPoint(Vector2f pos) {
    points.append(Vertex(pos, markColor));
}

void Mark::draw(RenderWindow& w) {
    w.draw(points);
}