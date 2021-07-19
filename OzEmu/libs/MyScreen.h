#include <SFML/Graphics.hpp>
#include <string>
#include "charset.cpp"

class PCD8544{
private:
	sf::RectangleShape*** map;
	sf::RenderWindow* window;

public:
	uint16_t cursor = 0;
	uint8_t screen_buffer[504];

	void begin(){
		window = new sf::RenderWindow(sf::VideoMode(84, 48), "OzEmu");
		window->setSize(sf::Vector2u(420, 240));
		map = new sf::RectangleShape**[48];
		for (unsigned int i = 0; i < 48; i++){
			map[i] = new sf::RectangleShape*[84];
			for (unsigned int l = 0; l < 84; l++){
				map[i][l] = new sf::RectangleShape(sf::Vector2f(1, 1));
				map[i][l]->setPosition(l, i);
				map[i][l]->setFillColor(sf::Color::Black);
				window->draw(*map[i][l]);
			}
		}
		window->display();
	}

	void setCursor(uint8_t x, uint8_t y){
		x %= 84;
		y %= 6;
		cursor = x + y * 84;
	}

	void write(uint8_t c){
		if (c > 127) c = 127;
		for (uint8_t i = 0; i < 5; i++)
			send(0, screen_buffer[cursor] = charset[c-32][i]);
		send(0, screen_buffer[cursor] = 0);
	}

	void print(float num){
		std::string str = to_string(num);
		for (int i = 0; i < str.size(); i++)
			write(str[i]);
	}
	
	void print(uint32_t num){
		std::string str = to_string(num);
		for (int i = 0; i < str.size(); i++)
			write(str[i]);
	}

	void send(uint32_t _, uint8_t num){
		uint8_t x = cursor % 84;
		uint8_t y = cursor / 84 * 8;
		cursor = (cursor + 1) % 504;
		for (unsigned int i = 0; i < 8; i++){
			map[y+i][x]->setFillColor((num >> i) & 1 ? sf::Color::White : sf::Color::Black);
			window->draw(*map[y+i][x]);
		}
		window->display();
	}
};
