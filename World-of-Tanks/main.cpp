#include <SFML/Graphics.hpp>
#include <iostream> 
#include <sstream>
#include <list>

using namespace sf;

////////////////////////////����� ��������////////////////////////
class Entity {
public:
	enum { left, right, up, down, stay } state;// ��� ������������ - ��������� �������
	float dx, dy, x, y, speed, moveTimer;//�������� ���������� ������ ��� ������� �����
	int w, h, health; //���������� �health�, �������� ����� ������
	bool life; //���������� �life� �����, ����������
	Texture texture;//���� ��������
	Sprite sprite;//���� ������ 
	float CurrentFrame;//������ ������� ����
	std::string name;//����� ����� ���� ������, ������ ����� ��������� �� ������
	//������� ����� ���� ���� �������� � update() � ����������� �� �����



	Entity(Image &image, float X, float Y, int W, int H, std::string Name) {
		x = X; y = Y; //���������� ��������� �������
		w = W; h = H;
		name = Name;
		moveTimer = 0;
		dx = 0; dy = 0;
		speed = 0;

		CurrentFrame = 0;
		health = 100;
		life = true; //���������������� ���������� ���������� �����, ����� ���
		texture.loadFromImage(image); //������� ���� ����������� � ��������
		sprite.setTexture(texture); //�������� ������ ���������
	}

	FloatRect getRect() {//����� ��������� ��������������. ��� �����, ������� (���,�����).
		FloatRect FR(x, y, w, h); // ���������� FR ���� FloatRect
		return FR;
		//��� ������ (�����) "sf::FloatRect" ��������� ������� ������ ���������� ��������������
		//� ����� ���� ��� ���������� �������� ������������ ����� �� �����
		//����� ��� �������� ��������, ���� �� ��� �����-���� ���� �� ���� ����� 
		//��� �-��� ����� ��� �������� ����������� 
	}

	virtual void update(float time) = 0;

};


////////////////////////////����� �����////////////////////////
class Enemy :public Entity {
public:
	int direction;//����������� �������� �����
	Enemy(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name) {
		if (name == "EasyEnemy") {
			//������ ������� ���� ������������� ���
			//������ ������ ������. IntRect � ��� ���������� �����
			sprite.setTextureRect(IntRect(0, 0, w, h));
			direction = rand() % (3); //����������� �������� ����� ����� ��������� �������
				//����� ��������� ��������� �����
			speed = 0.1;//���� ��������.���� ������ ������ ���������
			dx = speed;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//�-��� �������� ������������ � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//���� ������� - ������ ����� //����� (�����)
				{
					if (Dy > 0) {
						y = i * 32 - h;  dy = -0.1;
						direction = rand() % (3); //����������� �������� �����
					}//�� Y 
					if (Dy < 0) {
						y = i * 32 + 32; dy = 0.1;
						direction = rand() % (3);//����������� �������� ����� 
					}//������������ � �������� ������ 
					if (Dx > 0) {
						x = j * 32 - w; dx = -0.1;
						direction = rand() % (3);//����������� �������� ����� 
					}//� ������ ����� �����
					if (Dx < 0) {
						x = j * 32 + 32; dx = 0.1;
						direction = rand() % (3); //����������� �������� �����
					}// � ����� ����� �����
				}
			}
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {//��� ��������� � ����� ������ ������ ����� �����

			if (life) {//���������, ��� �� �����
				switch (direction)//�������� ��������� �������� � ����������� �� ���������
				{
				case 0: {//��������� ���� ������
					dx = speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 3) CurrentFrame -= 3;
					sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
					break;
				}
				case 1: {//��������� ���� �����
					dx = -speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 3) CurrentFrame -= 3;
					sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
					break;
				}
				case 2: {//���� �����
					dy = -speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 3) CurrentFrame -= 3;
					sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
					break;
				}
				case 3: {//���� ����
					dy = speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 3) CurrentFrame -= 3;
					sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
					break;
				}
				}

				x += dx * time; //�������� �� �X�
				checkCollisionWithMap(dx, 0);//������������ ������������ �� �

				y += dy * time; //�������� �� �Y�
				checkCollisionWithMap(0, dy);//������������ ������������ �� Y

				sprite.setPosition(x, y); //������ � ������� (x, y).

				if (health <= 0) { life = false; }//���� ������ ������ 0, ���� ����� 0, �� �������
			}
		}
	}
};//����� Enemy ������


////////////////////////////����� ����////////////////////////
class Bullet :public Entity {//����� ����
public:
	int direction;//����������� ����
	Bullet(Image &image, float X, float Y, int W, int H, std::string  Name, int dir) :Entity(image, X, Y, W, H, Name) {
		x = X; //���������� ���� �� ����� ����
		y = Y;
		direction = dir; //����������� ������ ����
		speed = 0.8;
		w = h = 16; //������� ����������� ����
		life = true; //���� ����
	}


	void update(float time)
	{
		switch (direction)
		{		
		case 0: dx = -speed; dy = 0;   break;// state = left
		case 1: dx = speed; dy = 0;   break;// state = right
		case 2: dx = 0; dy = -speed;   break;// state = up
		case 3: dx = 0; dy = speed;   break;// state = down
		}

		if (life) { // ���� ���� ����
			x += dx * time;//���� �������� ���� �� �
			y += dy * time;//�� �

			for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
				for (int j = x / 32; j < (x + w) / 32; j++)
				{
					if (TileMap[i][j] == '0')//���� ������� ��� ������ �����, �� //����� (�����)
						life = false;// �� ���� �������
				}
			sprite.setPosition(x + w / 2, y + h / 2); //�������� ������� ����
		} 
	}
};



int main()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1000, 600, desktop.bitsPerPixel), "Lesson 4");

	Texture herotexture;
	herotexture.loadFromFile("images/hero.png");

	Sprite herosprite;
	herosprite.setTexture(herotexture);
	//�������� ������ ��� ������������� � ������
	herosprite.setTextureRect(IntRect(343, 177, 499, 313));
	herosprite.setPosition(250, 250); //������� ������ � ������� x y 

	Clock clock;
	Clock gameTimeClock;//���������� �������� �������, ����� ����� ������� ����� ���� 
	int gameTime = 0;//�������� ������� �����, ����������������.


	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/enemy.png"); // ��������� ����������� ����� //��� ���� ��� ���, ���� ������� � ��������!

	Image BulletImage;//����������� ��� ����
	BulletImage.loadFromFile("images/bullet.png");//��������� �������� � ������ �����������

	std::list<Entity*>  enemies; //������ ������
	std::list<Entity*>  Bullets; //������ ����
	std::list<Entity*>::iterator it; //�������� ����� ��������� �� ��������� ������

	const int ENEMY_COUNT = 1;	//������������ ���������� ������ � ����
	int enemiesCount = 0;	//������� ���������� ������ � ����

	//��������� ������ ��������� �������
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		float xr = 150 + rand() % 500; // ��������� ���������� ����� �� ���� ���� �� ��� �x�
		float yr = 150 + rand() % 350; // ��������� ���������� ����� �� ���� ���� �� ��� �y�

			//������� ������ � �������� � ������
		enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
		enemiesCount += 1; //��������� ������� ������
	}

	int createObjectForMapTimer = 0;//���������� ��� ����� ��� ������������� ������

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//������� ����� � //���������� ������ (������)
			//�������� ��� ������, ���� ��� �����. ������������� ��� time ��� �� ����. 
			//��� �� ��������� ������ ����

		clock.restart();
		time = time / 800;

		createObjectForMapTimer += time;//���������� ������
		if (createObjectForMapTimer > 3000) {
			randomMapGenerate();//���������  ������ //���������� ����������� (�������)
			createObjectForMapTimer = 0;//�������� ������
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//�������� �� ������� ������� "P"
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::P)
				{
					Bullets.push_back(new Bullet(BulletImage, p.x, p.y, 16, 16, "Bullet", p.state));
				}
			}
		}

		//�������� ������
		for (it = enemies.begin(); it != enemies.end(); it++)
		{
			(*it)->update(time); //��������� ����� update()
		}

		//�������� ����
		for (it = Bullets.begin(); it != Bullets.end(); it++)
		{
			(*it)->update(time); //��������� ����� update()
		}

		//��������� ������ �� ������� "�������" ���� � ������� ��
		for (it = Bullets.begin(); it != Bullets.end(); )//������� ��� ���������� �� ������ �� �����
		{// ���� ���� ������ �����, �� ������� ���
			if ((*it)->life == false) { it = Bullets.erase(it); }
			else  it++;//� ���� �������� (����������) � ���� �������.		
		}



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
