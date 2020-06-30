#include <iostream>
#include <map>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

int main() {
    RenderWindow w(VideoMode(500, 300), "Hello World");
    w.setVerticalSyncEnabled(true);

    CircleShape r[100];
    for (int i = 0; i < 100; i++) {
        r[i].setRadius(20);
        r[i].setOrigin((20) / 2, (20) / 2);
        r[i].setFillColor(Color(255, i * 2, i * 2));
        r[i].setPosition(500 / 2, 300 / 2);
    }

    Vector2f position(500 / 2, 300 / 2);
    Vector2f speed;
    Vector2f acc;
    const float max_acc = 5000;
    const float drag = 5;

    map<Keyboard::Key, bool> keysPressed;

    Clock c;

    // game loop
    while (w.isOpen()) {
        Time delta = c.restart();

        Event ev;

        float up = keysPressed[Keyboard::W] || keysPressed[Keyboard::Up];
        float down = keysPressed[Keyboard::S] || keysPressed[Keyboard::Down];
        float left = keysPressed[Keyboard::A] || keysPressed[Keyboard::Left];
        float right = keysPressed[Keyboard::D] || keysPressed[Keyboard::Right];

        // physics
        acc = max_acc * (up * Vector2f(0, -1) + down * Vector2f(0, +1) +
                         left * Vector2f(-1, 0) + right * Vector2f(+1, 0));

        speed += acc * delta.asSeconds();
        speed -= speed * drag * delta.asSeconds();
        position += speed * delta.asSeconds();

        // events
        while (w.pollEvent(ev)) {
            switch (ev.type) {
                case Event::Closed:
                    w.close();
                    break;

                case Event::KeyPressed:
                    keysPressed[ev.key.code] = true;
                    break;

                case Event::KeyReleased:
                    keysPressed[ev.key.code] = false;
                    break;

                default:
                    break;
            }
        }

        w.clear(Color::White);

        for (int i = 99; i > 0; i--) {
            r[i].setPosition(r[i - 1].getPosition());
        }
        r[0].setPosition(position);
        for (int i = 0; i < 100; i++) {
            w.draw(r[99 - i]);
        }

        w.display();
    }
}
