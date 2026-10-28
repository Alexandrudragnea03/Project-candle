#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

struct Block {
    sf::Vector2f position;
    sf::Vector2f size;
    Block(sf::Vector2f pos, sf::Vector2f sz) : position(pos), size(sz) {}
    void draw(sf::RenderWindow& window) {
        sf::RectangleShape shape(size);
        shape.setPosition(position);
        shape.setFillColor(sf::Color(100, 100, 100));
        shape.setOutlineThickness(1.f);
        shape.setOutlineColor(sf::Color(60, 60, 60));

        window.draw(shape);
    }
};

const float TileSize = 40.f;

const std::vector<std::string> map = {
    "########################",
    "#                      #",
    "#                      #",
    "#                      #",
    "#      ###             #",
    "#                      #",
    "#                      #",
    "#             ###      #",
    "#                      #",
    "#                      #",
    "#    ###               #",
    "#                      #",
    "#                      #",
    "#           ###        #",
    "#                      #",
    "#                      #",
    "########################"
};

struct Map {
    std::vector<Block> blocks;
    Map(const std::vector<std::string>& map, float TileSize) {
        for (size_t y = 0; y < map.size(); y++){
            for (size_t x = 0; x < map[y].size(); x++){
                if (map[y][x] == '#') {
                    sf::Vector2f BlockPos(x * TileSize, y * TileSize);
                    sf::Vector2f BlockSize(TileSize, TileSize);
                    blocks.push_back(Block(BlockPos, BlockSize));
                }
            }
        }
    }
    void draw(sf::RenderWindow& window) {
        for (auto& block : blocks) {
            block.draw(window);
        }
    }
};


int main(){
    sf::RenderWindow window(sf::VideoMode({960u, 680u}), "Project Candle - v0.2");

    sf::Clock clock;

    sf::RectangleShape candle({30.f, 60.f});
    candle.setFillColor(sf::Color(255, 220, 100));
    candle.setPosition({385.f, 270.f});
    
    Map gameMap(map, TileSize);
    
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
            window.close();
        }
        
        window.clear(sf::Color(30, 30, 30));
        gameMap.draw(window);
        window.draw(candle);
        window.display();
    }
}