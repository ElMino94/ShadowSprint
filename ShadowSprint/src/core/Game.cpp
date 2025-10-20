#include "../../include/core/Game.h"

Game::Game()
    : window(sf::VideoMode({ 1920, 1080 }), "Runner 2d", sf::Style::None),
    currentState(MAINMENU),
    mainMenu(window)
{
    window.setFramerateLimit(60);
    window.setPosition({ 0, 0 });
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        processEvents();
        float dt = clock.restart().asSeconds();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->scancode == sf::Keyboard::Scancode::Escape)
                window.close();
        }

        if (currentState == MAINMENU) {
            if (mainMenu.handleEvent(*event, window)) {
                currentState = PLAYING;
            }
        }
    }
}

void Game::update(float dt) {
    if (currentState == MAINMENU) {
        mainMenu.update(dt);
    }

    if (currentState == PLAYING) {

    }
}

void Game::render() {
    window.clear();
    if (currentState == MAINMENU) {
        mainMenu.draw(window);
    }

    if (currentState == PLAYING) {

    }
    window.display();
}