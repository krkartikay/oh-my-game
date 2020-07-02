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
Mutex marks_mutex;

UdpSocket sock;
IpAddress otherIp;
unsigned short int otherPort;

int main() {
    sock.bind(Socket::AnyPort);

    cout << "Local Ip: " << IpAddress::getLocalAddress() << endl;
    cout << "Local Port: " << sock.getLocalPort() << endl;

    cout << "Enter Other Ip: 127.0.0.1\n";
    string s;
    s = "127.0.0.1";  // getline(cin, s);
    otherIp = IpAddress(s);

    cout << "Enter other port: ";
    cin >> otherPort;

    bool pressed;

    RenderWindow window(VideoMode(w, h), "Oh-My-Game");

    Thread markReciever(recvThread);
    markReciever.launch();

    while (window.isOpen()) {
        Event windowEvent;
        Packet p;
        while (window.pollEvent(windowEvent)) {
            switch (windowEvent.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseButtonPressed:
                    pressed = true;
                    {
                        Lock l(marks_mutex);
                        marks.emplace_back(
                            Color::Red);  // the one we're drawing
                                          // should always be red
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

                case Event::KeyPressed:
                    if (windowEvent.key.code == Keyboard::C) {
                        // press 'c' to clear screen
                        {
                            Lock l(marks_mutex);
                            sendClear();
                            marks.clear();
                        }
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