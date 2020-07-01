#include <iostream>
#include <map>
#include <string>
using namespace std;

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
using namespace sf;

#include "mark.h"

const int w = 500, h = 300;

vector<Mark> marks;

UdpSocket sock;
IpAddress otherIp;
unsigned short int otherPort;

void sendMark(Mark m);
Mark recvMark();

void markRecvThread();

int main() {
    sock.bind(Socket::AnyPort);

    cout << "Local Ip: " << IpAddress::getLocalAddress();
    cout << "Local Port: " << sock.getLocalPort() << endl;

    cout << "Enter Other Ip: 127.0.0.1\n";
    string s;
    s = "127.0.0.1";  // getline(cin, s);
    otherIp = IpAddress(s);

    cout << "Enter other port: ";
    cin >> otherPort;

    bool pressed;

    RenderWindow window(VideoMode(w, h), "Oh-My-Game");

    Thread markReciever(markRecvThread);
    markReciever.launch();

    while (window.isOpen()) {
        Event windowEvent;
        Packet p;
        string s = "Did sth";
        while (window.pollEvent(windowEvent)) {
            switch (windowEvent.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseButtonPressed:
                    pressed = true;
                    if (marks.size() % 2 == 1) {
                        marks.emplace_back(Color::Black);
                    } else {
                        marks.emplace_back(Color::Red);
                    }
                    break;

                case Event::MouseButtonReleased:
                    pressed = false;
                    sendMark(marks.back());
                    break;

                case Event::MouseMoved:
                    if (pressed) {
                        auto pos =
                            window.mapPixelToCoords(Mouse::getPosition(window));
                        marks.back().addPoint(pos);
                    }
                    break;

                default:
                    break;
            }
        }

        window.clear(Color::White);
        for (auto& mark : marks) {
            window.draw(mark);
        }

        window.display();
    }
}