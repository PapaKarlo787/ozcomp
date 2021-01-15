#include <SFML/Graphics.hpp>
#include <string>
#include "charset.cpp"

class PCD8544{
private:
	sf::RectangleShape*** map;
	sf::RenderWindow* window;
	unsigned int cursor = 0;
	void render(){
        for (unsigned int i = 0; i < 48; i++)
			for (unsigned int l = 0; l < 84; l++)
				window->draw(*map[i][l]);
        window->display();
	}

public:
	void begin(){
		window = new sf::RenderWindow(sf::VideoMode(84, 48), "MacEmu");
		window->setSize(sf::Vector2u(420, 240));
		map = new sf::RectangleShape**[48];
		for (unsigned int i = 0; i < 48; i++){
			map[i] = new sf::RectangleShape*[84];
			for (unsigned int l = 0; l < 84; l++){
				map[i][l] = new sf::RectangleShape(sf::Vector2f(1, 1));
				map[i][l]->setPosition(l, i);
				map[i][l]->setFillColor(sf::Color::Black);
			}
		}
	}

	void setCursor(unsigned int x, unsigned int y){
		x %= 84;
		y %= 48;
		cursor = x + (y / 8) * 84;
	}

	void write(unsigned char c){
		if (c > 127) c = 127;
		unsigned char* chars = charset[c-32];
		for (unsigned int i = 0; i < 5; i++)
			send(0, chars[i]);
		cursor = (cursor + 1) % 504;
	}

	void print(float num){
		std::string str = to_string(num);
		for (int i = 0; i < str.size(); i++)
			write(str[i]);
	}
	
	void print(int num){
		std::string str = to_string(num);
		for (int i = 0; i < str.size(); i++)
			write(str[i]);
	}

	void send(unsigned int _, unsigned char num){
		unsigned int x = cursor % 84;
		unsigned int y = cursor / 84 * 8;
		cursor = (cursor + 1) % 504;
		for (unsigned int i = 0; i < 8; i++)
			map[y+i][x]->setFillColor((num >> i) & 1 ? sf::Color::White : sf::Color::Black);
		render();
	}
};
