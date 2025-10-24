#include <cmath>
#include "../../include/map/Map.h" // adjust path if needed


// ---------------- ParallaxLayer impl ----------------
void ParallaxLayer::init(const sf::Vector2f& size, const sf::Color& color, float factor, float y) {
	speedFactor = factor;
	stripA.setSize(size);
	stripB.setSize(size);
	stripA.setFillColor(color);
	stripB.setFillColor(color);
	stripA.setPosition(0.f, y);
	stripB.setPosition(size.x, y);
}


void ParallaxLayer::update(float cameraX, const sf::Vector2f& size) {
	float offset = -cameraX * speedFactor;
	float w = size.x;
	float x = std::fmod(offset, w);
	if (x > 0) x -= w;
	stripA.setPosition(x, stripA.getPosition().y);
	stripB.setPosition(x + w, stripB.getPosition().y);
}


void ParallaxLayer::draw(sf::RenderWindow& window) {
	window.draw(stripA);
	window.draw(stripB);
}


// ---------------- Map impl ----------------
Map::Map(const sf::Vector2u& screenSize)
	: screen(screenSize) {
	reset();
}


void Map::reset() {
	cameraX = 0.f;
	baseSpeed = 220.f;
	speed = baseSpeed;
	totalMeters = 0.f;
	lastSpeedStepMeters = 0.f;
	speedBuffTimer = 0.f;
	speedNerfTimer = 0.f;
	rng = Utils::RNG{};


	// Parallax layers
	layers.clear();
	float W = static_cast<float>(screen.x);
	float H = static_cast<float>(screen.y);


	auto makeLayer = [&](sf::Color c, float factor, float y) {
		ParallaxLayer L; L.init({ W, H }, c, factor, y); return L; };


	layers.push_back(makeLayer(sf::Color(135, 206, 235), 0.10f, 0.f)); // sky
	layers.push_back(makeLayer(sf::Color(100, 170, 220), 0.30f, H * 0.18f)); // hills band
	layers.push_back(makeLayer(sf::Color(60, 60, 60), 1.00f, groundY - 40.f)); // ground band


	// Ground tiles strip wrapped by camera modulo
	tiles.clear();
	tileSize = { 40.f, 40.f };
	int tilesAcross = static_cast<int>(std::ceil(W / tileSize.x)) + 3;
	for (int i = 0; i < tilesAcross; ++i) {
		sf::RectangleShape t(tileSize);
		t.setFillColor(sf::Color(110, 90, 60));
		t.setPosition(i * tileSize.x, groundY);
		tiles.push_back(t);
	}


	// Content
	platforms.clear();
	pickups.clear();
	spawnAhead = W * 2.f;
	worldRightEdge = 0.f;
	for (int i = 0; i < 8; ++i) spawnChunk();
}


float Map::update(float dt) {
	// Scroll
	cameraX += speed * dt;


	// Distance in meters
	totalMeters += (speed * dt) * metersPerPixel;


	// Progressive speed: +0.1% per 100 m
	if (totalMeters - lastSpeedStepMeters >= 100.f) {
		baseSpeed *= 1.001f;
	}