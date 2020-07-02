#include "mark.h"

extern UdpSocket sock;
extern vector<Mark> marks;
extern IpAddress otherIp;
extern unsigned short int otherPort;

IpAddress recvIp;
unsigned short int recvPort;

Mark::Mark(Color c) : _markColor(c) {
    _points.setPrimitiveType(LineStrip);
}

void Mark::addPoint(Vector2f pos) {
    _points.append(Vertex(pos, _markColor));
}

void Mark::draw(RenderTarget& target, RenderStates states) const {
    target.draw(_points, states);
}

Packet& operator>>(Packet& p, Mark& m) {
    int color;
    p >> color;
    m._points.setPrimitiveType(LineStrip);
    m._markColor = Color::Black; // the mark other player draws should be black
    int vertexCount;
    p >> vertexCount;
    m._points.resize(vertexCount);
    for (int i = 0; i < vertexCount; i++) {
        p >> m._points[i].position.x;
        p >> m._points[i].position.y;
        cout << m._points[i].position.x << "\t" << m._points[i].position.y
             << endl;
        m._points[i].color = m._markColor;
    }
    return p;
}

Packet& operator<<(Packet& p, Mark& m) {
    p << m._markColor.toInteger();
    int vertexCount = m._points.getVertexCount();
    p << vertexCount;
    for (int i = 0; i < vertexCount; i++) {
        p << m._points[i].position.x;
        p << m._points[i].position.y;
    }
    return p;
}

void sendMark(Mark m) {
    Packet p_code;
    p_code << Int8('m'); // for Mark
    while (sock.send(p_code, otherIp, otherPort) != Socket::Status::Done)
        ;
    Packet p_mark;
    p_mark << m;
    while (sock.send(p_mark, otherIp, otherPort) != Socket::Status::Done)
        ;
}

Mark recvMark() {
    Packet p;
    while (sock.receive(p, recvIp, recvPort) != Socket::Status::Done)
        ;
    Mark m;
    p >> m;
    return m;
}

void recvThread() {
    while (true) {
        Packet p_code;
        while (sock.receive(p_code, recvIp, recvPort) != Socket::Status::Done)
            ;
        Int8 code;
        p_code >> code;
        if (code == 'm') {
            marks.push_back(recvMark());
            cout << "recvd mark " << marks.size() << endl;
        } else if (code == 'c') {
            marks.clear();
            cout << "recvd clr signal" << endl;
        } else {
            cout << "unknown signal " << int(code) << endl;
        }
    }
}

void sendClear() {
    Packet p;
    p << Int8('c'); // for Clear
    while (sock.send(p, otherIp, otherPort) != Socket::Status::Done)
        ;
}