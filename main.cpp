#include "cfg_manipulator.hpp"
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string.h>
using namespace std;

cfg_manipulator::cfg_file settings;

sf::RenderWindow window;
sf::Event event;
sf::RectangleShape sectors[3][3];
sf::Font font;
sf::Text text[3][3];

char arr[3][3];

struct window_settings {
    glm::vec4 bounds;
    const char *title;
} window_settings;

void init_window_settings() {
    window_settings.bounds.z = atoi(settings.read("window", "width"));
    window_settings.bounds.w = atoi(settings.read("window", "height"));

    if (strcmp(settings.read("window", "pos_x"), "CENTERED") == 0)
        window_settings.bounds.x = sf::VideoMode::getDesktopMode().width / 2 -
                                   window_settings.bounds.z / 2;
    else
        window_settings.bounds.x = atoi(settings.read("window", "pos_x"));
    if (strcmp(settings.read("window", "pos_y"), "CENTERED") == 0)
        window_settings.bounds.y = sf::VideoMode::getDesktopMode().height / 2 -
                                   window_settings.bounds.w / 2;
    else
        window_settings.bounds.y = atoi(settings.read("window", "pos_y"));

    window_settings.title = settings.read("window", "title");
}

void create_window() {
    window.create(
        sf::VideoMode(window_settings.bounds.z, window_settings.bounds.w),
        window_settings.title, sf::Style::Close);
    window.setPosition(
        sf::Vector2i(window_settings.bounds.x, window_settings.bounds.y));
}

void init_sectors() {
    size_t width = window_settings.bounds.z / 3,
           height = window_settings.bounds.w / 3, pos_x = 0, pos_y = 0;

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            sectors[i][j].setSize(sf::Vector2f(width, height));
            sectors[i][j].setPosition(sf::Vector2f(pos_x, pos_y));
            sectors[i][j].setFillColor(sf::Color::White);

            pos_x += width + 1;
        }

        pos_x = 0;
        pos_y += height + 1;
    }
}

bool is_mouse_left_clicked(sf::Vector2f size, sf::Vector2f position) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (sf::Mouse::getPosition(window).x >= position.x &&
            sf::Mouse::getPosition(window).x <= position.x + size.x) {
            if (sf::Mouse::getPosition(window).y >= position.y &&
                sf::Mouse::getPosition(window).y <= position.y + size.y)
                return true;
        }
    }

    return false;
}

bool is_mouse_right_clicked(sf::Vector2f size, sf::Vector2f position) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        if (sf::Mouse::getPosition(window).x >= position.x &&
            sf::Mouse::getPosition(window).x <= position.x + size.x) {
            if (sf::Mouse::getPosition(window).y >= position.y &&
                sf::Mouse::getPosition(window).y <= position.y + size.y)
                return true;
        }
    }

    return false;
}

void check() {
    string horizontally, vertically, oblique, _oblique;
    size_t length = 0;

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            if (arr[i][j] == 'x' || arr[i][j] == '0')
                length++;
        }
    }

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            horizontally += arr[i][j];
            vertically += arr[j][i];
        }

        oblique += arr[i][i];
        _oblique += arr[i][2 - i];

        if (horizontally == "xxx" || vertically == "xxx" || oblique == "xxx" ||
            _oblique == "xxx")
            exit(EXIT_SUCCESS);

        if (horizontally == "000" || vertically == "000" || oblique == "000" ||
            _oblique == "000")
            exit(EXIT_SUCCESS);

        horizontally = "";
        vertically = "";
    }
}

int main() {
    settings.open("settings.cfg");

    init_window_settings();

    init_sectors();

    create_window();

    font.loadFromFile("font.ttf");

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++)
            text[i][j].setFont(font);
    }

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                if (arr[i][j] == 0) {
                    if (is_mouse_left_clicked(sectors[i][j].getSize(),
                                              sectors[i][j].getPosition())) {
                        arr[i][j] = 'x';

                        text[i][j].setFillColor(sf::Color::Black);
                        text[i][j].setCharacterSize(200);
                        text[i][j].setString('x');
                        text[i][j].setOrigin(
                            text[i][j].getLocalBounds().left +
                                text[i][j].getLocalBounds().width / 2,
                            text[i][j].getLocalBounds().top +
                                text[i][j].getLocalBounds().height / 2);
                        text[i][j].setPosition(
                            sf::Vector2f(sectors[i][j].getPosition().x +
                                             sectors[i][j].getSize().x / 2,
                                         sectors[i][j].getPosition().y +
                                             sectors[i][j].getSize().y / 2));

                        check();
                    }

                    if (is_mouse_right_clicked(sectors[i][j].getSize(),
                                               sectors[i][j].getPosition())) {
                        arr[i][j] = '0';

                        text[i][j].setFillColor(sf::Color::Black);
                        text[i][j].setCharacterSize(200);
                        text[i][j].setString('0');
                        text[i][j].setOrigin(
                            text[i][j].getLocalBounds().left +
                                text[i][j].getLocalBounds().width / 2,
                            text[i][j].getLocalBounds().top +
                                text[i][j].getLocalBounds().height / 2);
                        text[i][j].setPosition(
                            sf::Vector2f(sectors[i][j].getPosition().x +
                                             sectors[i][j].getSize().x / 2,
                                         sectors[i][j].getPosition().y +
                                             sectors[i][j].getSize().y / 2));

                        check();
                    }
                }

                window.draw(sectors[i][j]);
            }
        }

        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < 3; j++)
                window.draw(text[i][j]);
        }

        window.display();
    }
}