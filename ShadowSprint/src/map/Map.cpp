#include "../../include/map/Map.h"
#include <cmath>
#include <iostream>

using namespace sf;

// =============================
// === ParallaxLayer Methods ===
// =============================
void ParallaxLayer::init(const sf::Vector2f& size, const std::string& texturePath, float factor, float y) {
    texture = std::make_shared<Texture>();
    if (!texture->loadFromFile(texturePath)) {
        std::cerr << "Erreur chargement : " << texturePath << "\n";
    }
    texture->setRepeated(true);

    stripA.setSize(size);
    stripB.setSize(size);
    stripA.setTexture(texture.get());
    stripB.setTexture(texture.get());

    stripA.setPosition({ 0.f, y });
    stripB.setPosition({ size.x, y });

    speedFactor = factor;
}

void ParallaxLayer::update(float cameraX, const sf::Vector2f& size) {
    float offset = std::fmod(-cameraX * speedFactor, size.x);
    stripA.setPosition({ offset, stripA.getPosition().y });
    stripB.setPosition({ offset + size.x, stripB.getPosition().y });
}

void ParallaxLayer::draw(sf::RenderWindow& window) {
    window.draw(stripA);
    window.draw(stripB);
}

// =============================
// ========= Map ===============
// =============================

Map::Map(const sf::Vector2u& screenSize)
    : screen(screenSize)
{
    reset();
}

void Map::reset() {
    cameraX = 0.f;
    baseSpeed = 220.f;
    speed = baseSpeed;
    lastSpeedStepMeters = 0.f;
    rng = Utils::RNG{};

    const float W = static_cast<float>(screen.x);
    const float H = static_cast<float>(screen.y);
    layers.clear();

    // Position du sol à 100 px du bas de la fenêtre
    groundY = H - 100.f;

    // --- PARALLAX BACKGROUND ---
    layers.push_back(makeLayer("../assets/textures/city 2/1.png", 0.05f, 0.f));
    layers.push_back(makeLayer("../assets/textures/city 2/2.png", 0.10f, H * 0.05f));
    layers.push_back(makeLayer("../assets/textures/city 2/3.png", 0.20f, H * 0.10f));
    layers.push_back(makeLayer("../assets/textures/city 2/4.png", 0.30f, H * 0.15f));
    layers.push_back(makeLayer("../assets/textures/city 2/5.png", 0.40f, H * 0.20f));
    layers.push_back(makeLayer("../assets/textures/city 2/6.png", 0.60f, H * 0.25f));

    // --- SOL (TILESET) ---
    groundTexture = std::make_unique<Texture>();
    if (!groundTexture->loadFromFile("../assets/textures/tiles/tileset.png")) {
        std::cerr << "Erreur chargement tileset.png\n";
    }
    groundTexture->setRepeated(true);

    tiles.clear();
    tileSize = { 40.f, 40.f };
    int tilesAcross = static_cast<int>(std::ceil(W / tileSize.x)) + 3;

    for (int i = 0; i < tilesAcross; ++i) {
        RectangleShape tile(tileSize);
        tile.setTexture(groundTexture.get());
        tile.setPosition({ i * tileSize.x, groundY });
        tiles.push_back(tile);
    }

    // --- Réinitialisation des plateformes ---
    platforms.clear();
    platformSpawnTimer = 0.f;
}

float Map::update(float dt, float score) {
    cameraX += speed * dt;

    float adjustedScore = score / 2160.f;
    float curve = std::pow(adjustedScore, 2.f);
    curve = std::clamp(curve, 0.f, 1.f);
    speed = baseSpeed + curve * (10000.f - baseSpeed);

    const Vector2f screenSize(static_cast<float>(screen.x), static_cast<float>(screen.y));
    for (auto& layer : layers)
        layer.update(cameraX, screenSize);

    // === Gestion des plateformes ===
    platformSpawnTimer += dt;

    // Nouvelle plateforme toutes les 2 à 5 secondes
    if (platformSpawnTimer > 2.f + static_cast<float>(std::rand() % 3)) {
        platformSpawnTimer = 0.f;

        float x = static_cast<float>(screen.x) + 200.f; // spawn à droite
        float y = static_cast<float>(std::rand() % 400 + 500); // hauteur aléatoire
        int tileCount = std::rand() % 6 + 3; // 3 à 8 tuiles (largeur 120–320 px)

        if (groundTexture) {
            platforms.push_back(std::make_unique<Platform>(
                sf::Vector2f(x, y),
                tileCount,
                *groundTexture,
                tileSize
            ));
        }
    }

    // Mise à jour et nettoyage
    for (auto it = platforms.begin(); it != platforms.end();) {
        (*it)->update(dt, speed);
        if ((*it)->isOffScreen())
            it = platforms.erase(it);
        else
            ++it;
    }

    return speed;
}

void Map::draw(RenderWindow& window) {
    for (auto& layer : layers)
        layer.draw(window);

    for (auto& tile : tiles)
        window.draw(tile);

    for (auto& platform : platforms)
        platform->draw(window);
}

int Map::tryConsumePickup(const FloatRect&) {
    return 0;
}

ParallaxLayer Map::makeLayer(const std::string& file, float speedFactor, float y) {
    ParallaxLayer layer;
    layer.init({ static_cast<float>(screen.x), static_cast<float>(screen.y) }, file, speedFactor, y);
    return layer;
}

float Map::getGroundY() const {
    return groundY;
}


const std::vector <std::unique_ptr<class Platform>>& Map::getPlatforms() const {
    return platforms;
}