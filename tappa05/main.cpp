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
    "#                   ####",
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

struct Candle {
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Vector2f Velocity;
    float gravity = 500.f;
    float jumpforce = -450.f;
    bool grounded = false;
    float speed;
    float melting_rate = 5.f;
    float min_Height = 10.f;


    Candle(sf::Vector2f pos, sf::Vector2f sz) : position(pos), size(sz), Velocity(0.f, 0.f), speed(200.f) {}

    void Input(){
        Velocity.x = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && grounded) {
            Velocity.y = jumpforce;
            grounded = false;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && Velocity.y < 0.f) { //permette alla candela di rallentare se space viene rilasciato a mezz'aria
                Velocity.y = Velocity.y * 0.75f;
            }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            Velocity.x -= speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            Velocity.x += speed;
        }
    }

    void Update(float deltaTime, const std::vector<Block>& blocks) {
        position.x += Velocity.x * deltaTime;
        if (size.y > min_Height) {
            size.y -= melting_rate * deltaTime;
            if (size.y < min_Height) {
                size.y = min_Height;
            }
        }


        for (const auto& block : blocks) {
            if (CheckCollision(block)) {
                if (Velocity.x > 0.f) {
                    position.x = block.position.x - size.x;
                } else if (Velocity.x < 0.f) {
                    position.x = block.position.x + block.size.x;
                }
                Velocity.x = 0.f;
            }
        }
        
        Velocity.y += gravity * deltaTime;
        position.y += Velocity.y * deltaTime;
        grounded = false;
        for (const auto& block : blocks) {
            if (CheckCollision(block)) {
                if (Velocity.y > 0.f) {
                    position.y = block.position.y - size.y;
                    grounded = true;
                    Velocity.y = 0.f;
                } else if (Velocity.y < 0.f) {
                    position.y = block.position.y + block.size.y;
                    Velocity.y = 0.f;
                }
            }
        }
    }

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape candleShape(size);
        candleShape.setPosition(position);
        sf::Color candleColor(255, 220, 100);
        candleShape.setFillColor(candleColor);
        window.draw(candleShape);
    }

    bool CheckCollision(const Block& block) {
        if ((position.x + size.x > block.position.x) && (position.x < block.position.x + block.size.x)
            && (position.y + size.y > block.position.y) && (position.y < block.position.y + block.size.y))
            return true;
        else
            return false;
    }
};

int main(){
    sf::RenderWindow window(sf::VideoMode({960u, 680u}), "Project Candle - v0.4");

    sf::Clock clock;
    /*
    ----------
    da modificare in futuro i valori della candela
    ----------
    */
    Candle player(sf::Vector2f(400.f, 600.f), sf::Vector2f(20.f, 60.f)); //test con 60 di altezza per verificare che si scioglie correttamente

    Map gameMap(map, TileSize);

    window.setFramerateLimit(60);

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
            window.close();
        }

        player.Input();
        player.Update(deltaTime, gameMap.blocks);
        window.clear(sf::Color(30, 30, 30));
        gameMap.draw(window);
        player.Draw(window);
        window.display();
    }
}