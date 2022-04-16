#include <bits/stdc++.h>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <time.h>
using namespace sf;

int ts = 54;  // tile size
Vector2i offset(48, 24);

struct piece
{
    int x, y, col, row, kind, match, alpha;
    piece()
    {
        match = 0;
        alpha = 255;
    }
};

piece grid[10][10];

void swap(piece p1, piece p2)
{
    std::swap(p1.col, p2.col);
    std::swap(p1.row, p2.row);

    grid[p1.row][p1.col] = p1;
    grid[p2.row][p2.col] = p2;
}

int main()
{
    srand(time(0));
    RenderWindow app(VideoMode(700, 480), "Match-3 Game!");

    app.setFramerateLimit(60);

    Texture t1, t2;
    t1.loadFromFile("images/background.png");
    t2.loadFromFile("images/gems.png");

    Sprite background(t1), gems(t2);

    Text scored, timer;
    Font font;
    font.loadFromFile("game_over.ttf");
    scored.setFont(font);
    timer.setFont(font);
    scored.setCharacterSize(70);
    scored.setPosition(Vector2f(500.0f, 270.0f));
    timer.setCharacterSize(70);
    timer.setPosition(Vector2f(500.0f, 350.0f));
    timer.setFillColor(Color::Black);
    scored.setFillColor(Color::Black);

    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
            grid[i][j].kind = rand() % 3;
            grid[i][j].col = j;
            grid[i][j].row = i;
            grid[i][j].x = j * ts;
            grid[i][j].y = i * ts;
        }
    }

    int highest = 0;
    int opponent = 0;
    int x0, y0, x, y;
    int click = 0;
    Vector2i pos;
    bool isSwap = false, isMoving = false;

    // Networking
    IpAddress ip = IpAddress::getLocalAddress();
    TcpSocket socket;
    char connType;
    cout << "(s) for server, (c) for client? ";
    cin >> connType;
    if (connType == 's') {
        TcpListener listener;
        listener.listen(2000);
        listener.accept(socket);
    } else if (connType == 'c') {
        socket.connect(ip, 2000);
    }
    socket.setBlocking(false);

    Clock c;
    int total_time = 60;

    while (app.isOpen()) {
        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed) {
                app.close();
            }

            if (e.type == Event::MouseButtonPressed) {
                if (e.mouseButton.button == Mouse::Left) {
                    if (!isSwap && !isMoving) {
                        click++;
                    }
                    pos = Mouse::getPosition(app) - offset;
                }
            }
        }

        int rem_time = total_time - c.getElapsedTime().asSeconds();
        if (rem_time == 0) {
            // game over, show score and exit
            Text gameOver;
            Text winLose;
            stringstream ss;
            ss << "Game Over!!\nYour Score: " << highest << "\nOpponent Score: " << opponent;
            gameOver.setColor(Color::Blue);
            gameOver.setFont(font);
            gameOver.setCharacterSize(120);
            gameOver.setPosition(offset.x, offset.y);
            gameOver.setOutlineThickness(2);
            gameOver.setOutlineColor(Color::White);
            gameOver.setString(ss.str());
            winLose.setFont(font);
            winLose.setCharacterSize(120);
            winLose.setPosition(offset.x, offset.y + 200);
            winLose.setOutlineThickness(2);
            winLose.setOutlineColor(Color::White);
            if (highest > opponent) {
                winLose.setString("You WIN!!");
                winLose.setColor(Color::Green);
            } else {
                winLose.setString("You LOSE!!");
                winLose.setColor(Color::Red);
            }
            app.draw(gameOver);
            app.draw(winLose);
            app.display();
            sleep(seconds(5));
            app.close();
        }

        // mouse click
        if (click == 1) {
            x0 = pos.x / ts + 1;
            y0 = pos.y / ts + 1;
        }
        if (click == 2) {
            x = pos.x / ts + 1;
            y = pos.y / ts + 1;
            if (abs(x - x0) + abs(y - y0) == 1) {
                swap(grid[y0][x0], grid[y][x]);
                isSwap = 1;
                click = 0;
            } else {
                click = 1;
            }
        }

        // Match finding
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                if (grid[i][j].kind == grid[i + 1][j].kind) {
                    if (grid[i][j].kind == grid[i - 1][j].kind) {
                        for (int n = -1; n <= 1; n++) {
                            grid[i + n][j].match = 1;
                        }
                    }
                }

                if (grid[i][j].kind == grid[i][j + 1].kind) {
                    if (grid[i][j].kind == grid[i][j - 1].kind) {
                        for (int n = -1; n <= 1; n++) {
                            grid[i][j + n].match = 1;
                        }
                    }
                }
            }
        }

        // Moving animation
        isMoving = false;
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                piece& p = grid[i][j];
                int dx, dy;
                for (int n = 0; n < 4; n++) { // 4 - speed
                    dx = p.x - p.col * ts;
                    dy = p.y - p.row * ts;
                    if (dx) {
                        p.x -= dx / abs(dx);
                    }
                    if (dy) {
                        p.y -= dy / abs(dy);
                    }
                }
                if (dx || dy) {
                    isMoving = 1;
                }
            }
        }

        // Deleting amimation
        if (!isMoving) {
            for (int i = 1; i <= 8; i++) {
                for (int j = 1; j <= 8; j++) {
                    if (grid[i][j].match) {
                        if (grid[i][j].alpha > 10) {
                            grid[i][j].alpha -= 10;
                            isMoving = true;
                        }
                    }
                }
            }
        }

        // Get score
        int score = 0;
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                score += grid[i][j].match;
            }
        }

        // Second swap if no match
        if (isSwap && !isMoving) {
            {
                if (!score) {
                    swap(grid[y0][x0], grid[y][x]);
                }
                isSwap = 0;
            }
            for (int i = 1; i <= 8; i++) {
                for (int j = 1; j <= 8; j++) {
                    grid[i][j].match = 0;
                }
            }
        }

        // Update grid
        if (!isMoving) {
            for (int i = 8; i > 0; i--) {
                for (int j = 1; j <= 8; j++) {
                    if (grid[i][j].match) {
                        for (int n = i; n > 0; n--) {
                            if (!grid[n][j].match) {
                                swap(grid[n][j], grid[i][j]);
                                break;
                            }
                        }
                    }
                }
            }

            for (int j = 1; j <= 8; j++) {
                for (int i = 8, n = 0; i > 0; i--) {
                    if (grid[i][j].match) {
                        grid[i][j].kind = rand() % 7;
                        grid[i][j].y = -ts * n++;
                        grid[i][j].match = 0;
                        grid[i][j].alpha = 255;
                    }
                }
            }
        }

        // Networking -- send and recv score
        Packet p;
        p << highest;
        socket.send(p);
        socket.receive(p);
        p >> opponent;

        //////draw///////
        app.draw(background);

        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                piece p = grid[i][j];
                gems.setTextureRect(IntRect(p.kind * 49, 0, 49, 49));
                gems.setColor(Color(255, 255, 255, p.alpha));
                gems.setPosition(p.x, p.y);
                gems.move(offset.x - ts, offset.y - ts);
                app.draw(gems);
            }
        }
        stringstream ss, st;
        highest = max(score + highest, highest);
        ss << "Score : " << highest << "\n";
        ss << "Opp.: " << opponent;
        st << "Time : " << rem_time;
        scored.setString(ss.str());
        timer.setString(st.str());
        app.draw(scored);
        app.draw(timer);
        app.display();
    }
    return 0;
}
