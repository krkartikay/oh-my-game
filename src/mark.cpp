#include "mark.h"

extern UdpSocket sock;
extern vector<Mark> marks;
extern IpAddress otherIp;
extern unsigned short int otherPort;

IpAddress recvIp;
unsigned short int recvPort;

Mark::Mark(Color c) : markColor(c) {
    points.setPrimitiveType(LineStrip);
}

void Mark::addPoint(Vector2f pos) {
    points.append(Vertex(pos, markColor));
}

void Mark::draw(RenderTarget& target, RenderStates states) const {
    target.draw(points, states);
}

void sendMark(Mark m) {
    Packet p;
    static int i;
    i++;
    p << i;
    cout << "trying to send to " << otherIp.toString() << ":" << otherPort << endl;
    sock.send(p, otherIp, otherPort);
}

Mark recvMark() {
    Packet p;
    sock.receive(p, recvIp, recvPort);
    int i;
    p >> i;
    cout << i << endl;
    return Mark(Color::Red);
}

void markRecvThread() {
    while (true) {
        cout << "trying to recv" << endl;
        recvMark();
    }
}