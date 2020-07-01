#include "mark.h"

Mark::Mark(Color c) : markColor(c) {
    points.setPrimitiveType(LineStrip);
}

void Mark::addPoint(Vector2f pos) {
    points.append(Vertex(pos, markColor));
}

void Mark::draw(RenderTarget& target, RenderStates states) const {
    target.draw(points, states);
}