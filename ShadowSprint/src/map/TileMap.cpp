#include "../../include/map/TileMap.h"

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize,
    const std::vector<int>& tiles, unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    m_tileSize = tileSize;

    if (!m_tileset.loadFromFile(tileset))
        return false;

    // Chaque tuile est dessinée avec deux triangles, donc 6 sommets vu que sfml ne possede pas les quads
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(width * height * 6);

    const unsigned int tilesPerRow = m_tileset.getSize().x / tileSize.x;

    //fonction pour créer le vertex
    auto putVertex = [](sf::Vertex& v, float px, float py, float tx, float ty) {
        v.position = sf::Vector2f(px, py);
        v.texCoords = sf::Vector2f(tx, ty);
        };

    for (unsigned int i = 0; i < width; ++i) {
        for (unsigned int j = 0; j < height; ++j) {
            const int tileNumber = tiles[i + j * width];
            if (tileNumber < 0) continue; // -1 = tuile vide, on ignore

            // Coordonnées de la tuile dans le tileset
            const int tu = tileNumber % tilesPerRow;
            const int tv = tileNumber / tilesPerRow;

            // Position en pixels sur la map
            const float px = static_cast<float>(i * tileSize.x);
            const float py = static_cast<float>(j * tileSize.y);

            // Taille de la tuile
            const float pw = static_cast<float>(tileSize.x);
            const float ph = static_cast<float>(tileSize.y);

            // Coordonnées de la tuile dans la texture
            const float tx = static_cast<float>(tu * tileSize.x);
            const float ty = static_cast<float>(tv * tileSize.y);
            const float tw = static_cast<float>(tileSize.x);
            const float th = static_cast<float>(tileSize.y);

            // Récupère le tableau de 6 sommets correspondant à la tuile
            sf::Vertex* tri = &m_vertices[(i + j * width) * 6];

            // Premier triangle : top-left, top-right, bottom-right
            putVertex(tri[0], px, py, tx, ty);
            putVertex(tri[1], px + pw, py, tx + tw, ty);
            putVertex(tri[2], px + pw, py + ph, tx + tw, ty + th);

            // Deuxième triangle : top-left, bottom-right, bottom-left
            putVertex(tri[3], px, py, tx, ty);
            putVertex(tri[4], px + pw, py + ph, tx + tw, ty + th);
            putVertex(tri[5], px, py + ph, tx, ty + th);
        }
    }

    return true;
}

void TileMap::draw(sf::RenderWindow& window) {
    sf::RenderStates states;
    states.texture = &m_tileset;
    window.draw(m_vertices, states);
}

sf::FloatRect TileMap::getTileBounds(unsigned int x, unsigned int y) const {
    // Retourne la position de x et y et la taille d'une tuile sous forme de rectangle

    return sf::FloatRect(
        sf::Vector2f(static_cast<float>(x * m_tileSize.x),
            static_cast<float>(y * m_tileSize.y)),
        sf::Vector2f(static_cast<float>(m_tileSize.x),
            static_cast<float>(m_tileSize.y))
    );
}