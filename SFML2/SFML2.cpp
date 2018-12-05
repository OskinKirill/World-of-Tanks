#include <SFML/Graphics.hpp>

using namespace sf;
int main()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1000, 600, desktop.bitsPerPixel), "Lesson 4");

	Texture herotexture;
	herotexture.loadFromFile("images/hero.png");

	Sprite herosprite;
	herosprite.setTexture(herotexture);
	//получили нужный нам прямоугольник с танком
	herosprite.setTextureRect(IntRect(343, 177, 499, 313));
	herosprite.setPosition(250, 250); //выводим спрайт в позицию x y 

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			herosprite.move(-1, 0); herosprite.setTextureRect(IntRect(0, 187, 157, 323));
		}

		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			herosprite.move(1, 0); herosprite.setTextureRect(IntRect(343, 177, 499, 313));
		}

		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			herosprite.move(0, -1); herosprite.setTextureRect(IntRect(177, 0, 313, 157));
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			herosprite.move(0, 1); herosprite.setTextureRect(IntRect(188, 343, 324, 500));
		}

		window.clear();
		window.draw(herosprite);
		window.display();
	}

	return 0;
}
