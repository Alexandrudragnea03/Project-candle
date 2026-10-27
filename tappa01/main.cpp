#include<iostream>
#include<SFML/Graphics.hpp>

int main(){
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "Project Candle - v0.1");

    sf::Clock clock;

    sf::RectangleShape candle({30.f, 60.f});
    candle.setFillColor(sf::Color(255, 220, 100));
    candle.setPosition({385.f, 270.f});
    
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
            window.close();
        }
        
        window.clear(sf::Color(30, 30, 30));
        window.draw(candle);

        window.display();
    }
}