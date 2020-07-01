#include <iostream>
#include <map>
using namespace std;

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
using namespace sf;

#include "mark.h"

const int w = 500, h = 300;

int main() {
    RenderWindow window(VideoMode(w, h), "Oh-My-Game");

    UdpSocket u;

    char c;
    cout << "Server or client? [s/c] ";
    cin >> c;
    if (c == 's') {
        u.bind(1337);
        Packet p;
        IpAddress ip;
        unsigned short int port;
        u.receive(p, ip, port);
        string msg;
        p >> msg;
        cout << "Recieved msg: " << msg << endl;
    } else {
        string msg;
        cout << "Enter msg: ";
        cin >> msg;
        Packet p;
        p << msg;
        u.send(p, "127.0.0.1", 1337);
    }

    bool pressed;
    vector<Mark> marks;

    while (window.isOpen()) {
        Event windowEvent;
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
