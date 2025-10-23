#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class TileMap {
public:
    // Charge le tileset et construit la map à partir d'un tableau de tuiles
    bool load(const std::string& tileset, sf::Vector2u tileSize,
        const std::vector<int>& tiles, unsigned int width, unsigned int height);

    // Dessine la map dans la fenêtre
    void draw(sf::RenderWindow& window);

    // Retourne les coordonnées d'une tuile (utile pour les collisions)
    sf::FloatRect getTileBounds(unsigned int x, unsigned int y) const;

private:
    sf::VertexArray m_vertices;    // Contient les sommets pour dessiner les tuiles
    sf::Texture     m_tileset;
    unsigned int    m_width = 0;
    unsigned int    m_height = 0;
    sf::Vector2u    m_tileSize;
};
