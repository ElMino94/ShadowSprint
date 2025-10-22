#include "../../include/core/Game.h"

using namespace sf;

Game::Game()
    : window(sf::VideoMode({ 1920, 1080 }), "Runner 2d", sf::Style::None),
    currentState(MAINMENU),
    mainMenu(window), optionMenu(window), pauseMenu(window), igUI(window)
{
    window.setFramerateLimit(60);
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
                if (currentState == PLAYING) {
                    currentState = PAUSEMENU;
                    pauseMenu.activate();
                }
                else if (currentState == PAUSEMENU) {
                    currentState = PLAYING;
                }
        }

        // MAIN MENU
        if (currentState == MAINMENU) {
            MainMenu::MainMenuAction action = mainMenu.handleEvent(*event);

            switch (action) {
            case MainMenu::MainMenuAction::Play:
                currentState = PLAYING;
                mainMenu.activate();
                break;
            case MainMenu::MainMenuAction::Options:
                currentState = OPTIONSMENU;
                optionMenu.setReturnContext(OptionMenu::ReturnContext::MainMenu);
                optionMenu.activate();
                break;
            case MainMenu::MainMenuAction::Quit:
                window.close();
                break;
            case MainMenu::MainMenuAction::None:
            default:
                break;
            }
        }

        // OPTIONS MENU
        if (currentState == OPTIONSMENU) {
            OptionMenu::OptionAction action = optionMenu.handleEvent(*event);

            if (action == OptionMenu::OptionAction::Back) {
                bool fullscreen = optionMenu.isFullscreenEnabled();
                bool vsync = optionMenu.isVsyncEnabled();

                applyDisplaySettings(fullscreen, vsync);

                if (optionMenu.getReturnContext() == OptionMenu::ReturnContext::MainMenu) {
                    currentState = MAINMENU;
                    mainMenu.activate();
                }
                else {
                    currentState = PAUSEMENU;
                    pauseMenu.activate();
                }
            }
        }

        // PAUSE MENU
        if (currentState == PAUSEMENU) {
            PauseMenu::Action action = pauseMenu.handleEvent(*event);

            switch (action) {
            case PauseMenu::Action::Resume:
                currentState = PLAYING;
                break;
            case PauseMenu::Action::Options:
                currentState = OPTIONSMENU;
                optionMenu.setReturnContext(OptionMenu::ReturnContext::PauseMenu);
                optionMenu.activate();
                break;
            case PauseMenu::Action::Restart:
                resetGame();
                currentState = PLAYING;
                break;
            case PauseMenu::Action::BackToMenu:
                resetGame();
                currentState = MAINMENU;
                mainMenu.activate();
                break;
            case PauseMenu::Action::None:
            default:
                break;
            }
        }
    }
}

void Game::update(float dt) {
    if (currentState == MAINMENU) {
        mainMenu.update(dt);
    }

    if (currentState == OPTIONSMENU) {
        optionMenu.update(dt);
    }

    if (currentState == PAUSEMENU) {
        pauseMenu.update(dt);
    }

    if (currentState == PLAYING) {
        igUI.update(dt, 100.f);
    }
}

void Game::render() {
    window.clear();
    if (currentState == MAINMENU) {
        mainMenu.draw(window);
    }

    if (currentState == OPTIONSMENU) {
        optionMenu.draw(window);
    }

    if (currentState == PAUSEMENU) {
        pauseMenu.draw(window);
    }

    if (currentState == PLAYING) {
        igUI.draw(window);
    }
    window.display();
}

void Game::applyDisplaySettings(bool fullscreen, bool vsync) {
    sf::VideoMode mode = fullscreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode({ 1280, 720 });
    unsigned int style = fullscreen ? sf::Style::None : sf::Style::Default;

    window.create(mode, "Runner 2d", style);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(vsync);

    mainMenu.setFullscreen(fullscreen);
    optionMenu.setFullscreen(fullscreen);
    pauseMenu.setFullscreen(fullscreen);
    igUI.setFullscreen(fullscreen);
}

void Game::resetGame() {
    // Reset player, level, score, etc.
}