#include "../../include/map/Map.h"
#include <cmath>
#include <iostream>

// ============================================================
// ParallaxLayer
// ============================================================
void ParallaxLayer::init(const sf::Vector2f& size, const std::string& texturePath, float factor, float y)
{
    speedFactor = factor;

    stripA.setSize(size);
    stripB.setSize(size);
    stripA.setPosition({ 0.f, y });
    stripB.setPosition({ size.x, y });

    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(texturePath)) {
        std::cerr << "Erreur chargement texture: " << texturePath << "\n";
        stripA.setFillColor(sf::Color(50, 50, 50));
        stripB.setFillColor(sf::Color(80, 80, 80));
    }
    else {
        texture->setRepeated(true);
        stripA.setTexture(texture.get());
        stripB.setTexture(texture.get());
    }
    this->texture = texture; // keep alive
}

void ParallaxLayer::update(float cameraX, const sf::Vector2f& size)
{
    float offset = -cameraX * speedFactor;
    float w = size.x;

    float x = std::fmod(offset, w);
    if (x > 0) x -= w;

    stripA.setPosition({ x, stripA.getPosition().y });
    stripB.setPosition({ x + w, stripB.getPosition().y });
}

void ParallaxLayer::draw(sf::RenderWindow& window)
{
    window.draw(stripA);
    window.draw(stripB);
}

// ============================================================
// Map
// ============================================================
Map::Map(const sf::Vector2u& screenSize)
    : screen(screenSize)
{
    reset();
}

void Map::reset()
{
    cameraX = 0.f;
    baseSpeed = 220.f;
    speed = baseSpeed;
    totalMeters = 0.f;
    lastSpeedStepMeters = 0.f;
    rng = Utils::RNG{};

    const float W = static_cast<float>(screen.x);
    const float H = static_cast<float>(screen.y);
    layers.clear();

    // Load 7 parallax layers (your provided images)
    layers.push_back(makeLayer("../assets/textures/city 2/1.png", 0.05f, 0.f));     // sky (very slow)
    layers.push_back(makeLayer("../assets/textures/city 2/2.png", 0.10f, H * 0.05f));
    layers.push_back(makeLayer("../assets/textures/city 2/3.png", 0.20f, H * 0.10f));
    layers.push_back(makeLayer("../assets/textures/city 2/4.png", 0.30f, H * 0.15f));
    layers.push_back(makeLayer("../assets/textures/city 2/5.png", 0.40f, H * 0.20f));
    layers.push_back(makeLayer("../assets/textures/city 2/6.png", 0.60f, H * 0.25f));
    // Ground texture
    groundTexture = std::make_unique<sf::Texture>();
    if (!groundTexture->loadFromFile("../assets/textures/tiles/tileset.png"))
        std::cerr << "Error loading ground_tile.png\n";
    groundTexture->setRepeated(true);

    tiles.clear();
    tileSize = { 40.f, 40.f };
    int tilesAcross = static_cast<int>(std::ceil(W / tileSize.x)) + 3;
    for (int i = 0; i < tilesAcross; ++i) {
        sf::RectangleShape tile(tileSize);
        tile.setTexture(groundTexture.get());
        tile.setPosition({ i * tileSize.x, groundY });
        tiles.push_back(tile);
    }
}

ParallaxLayer Map::makeLayer(const std::string& file, float speedFactor, float y)
{
    ParallaxLayer layer;
    layer.init({ static_cast<float>(screen.x), static_cast<float>(screen.y) }, file, speedFactor, y);
    return layer;
}

float Map::update(float dt)
{
    cameraX += speed * dt;
    totalMeters += (speed * dt) * metersPerPixel;

    // +0.1% per 100 meters
    if (totalMeters - lastSpeedStepMeters >= 100.f) {
        baseSpeed *= 1.001f;
        lastSpeedStepMeters += 100.f;
        speed = baseSpeed;
    }

    for (auto& L : layers)
        L.update(cameraX, { static_cast<float>(screen.x), static_cast<float>(screen.y) });

    // Loop ground tiles
    float xmod = std::fmod(cameraX, tileSize.x);
    float stripWidth = static_cast<float>(tiles.size()) * tileSize.x;
    for (std::size_t i = 0; i < tiles.size(); ++i) {
        float baseX = static_cast<float>(i) * tileSize.x;
        float posX = -xmod + baseX;
        if (posX > static_cast<float>(screen.x))
            posX -= stripWidth;
        tiles[i].setPosition({ posX, groundY });
    }

    return speed;
}

void Map::draw(sf::RenderWindow& window)
{
    for (auto& L : layers) L.draw(window);
    for (auto& t : tiles) window.draw(t);
}

bool Map::resolvePlayerCollisions(sf::FloatRect& playerBounds, sf::Vector2f& velocity)
{
    bool onGround = false;

    sf::FloatRect groundRect;
    groundRect.position = { cameraX - 1200.f, groundY };
    groundRect.size = { 5000.f, static_cast<float>(screen.y) - groundY };

    sf::Vector2f push(0.f, 0.f);
    if (Utils::resolveAABB(playerBounds, groundRect, push)) {
        playerBounds.position.x += push.x;
        playerBounds.position.y += push.y;
        if (push.y < 0.f) {
            velocity.y = 0.f;
            onGround = true;
        }
    }

    return onGround;
}

int Map::tryConsumePickup(const sf::FloatRect&)
{
    return 0;
}
