#include <SFML/Graphics.hpp>
#include <iostream> 
#include <sstream>
#include <list>

using namespace sf;

////////////////////////////КЛАСС СУЩНОСТЬ////////////////////////
class Entity {
public:
	enum { left, right, up, down, stay } state;// тип перечисления - состояние объекта
	float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей
	int w, h, health; //переменная “health”, хранящая жизни игрока
	bool life; //переменная “life” жизнь, логическая
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт 
	float CurrentFrame;//хранит текущий кадр
	std::string name;//враги могут быть разные, врагов можно различать по именам
	//каждому можно дать свое действие в update() в зависимости от имени



	Entity(Image &image, float X, float Y, int W, int H, std::string Name) {
		x = X; y = Y; //координата появления спрайта
		w = W; h = H;
		name = Name;
		moveTimer = 0;
		dx = 0; dy = 0;
		speed = 0;

		CurrentFrame = 0;
		health = 100;
		life = true; //инициализировали логическую переменную жизни, герой жив
		texture.loadFromImage(image); //заносим наше изображение в текстуру
		sprite.setTexture(texture); //заливаем спрайт текстурой
	}

	FloatRect getRect() {//метод получения прямоугольника. его коорд, размеры (шир,высот).
		FloatRect FR(x, y, w, h); // переменная FR типа FloatRect
		return FR;
		//Тип данных (класс) "sf::FloatRect" позволяет хранить четыре координаты прямоугольника
		//в нашей игре это координаты текущего расположения тайла на карте
		//далее это позволит спросить, есть ли ещё какой-либо тайл на этом месте 
		//эта ф-ция нужна для проверки пересечений 
	}

	virtual void update(float time) = 0;

};


////////////////////////////КЛАСС ВРАГА////////////////////////
class Enemy :public Entity {
public:
	int direction;//направление движения врага
	Enemy(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name) {
		if (name == "EasyEnemy") {
			//Задаем спрайту один прямоугольник для
			//вывода одного игрока. IntRect – для приведения типов
			sprite.setTextureRect(IntRect(0, 0, w, h));
			direction = rand() % (3); //Направление движения врага задаём случайным образом
				//через генератор случайных чисел
			speed = 0.1;//даем скорость.этот объект всегда двигается
			dx = speed;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф-ция проверки столкновений с картой
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//если элемент - тайлик земли //карта (Шагид)
				{
					if (Dy > 0) {
						y = i * 32 - h;  dy = -0.1;
						direction = rand() % (3); //Направление движения врага
					}//по Y 
					if (Dy < 0) {
						y = i * 32 + 32; dy = 0.1;
						direction = rand() % (3);//Направление движения врага 
					}//столкновение с верхними краями 
					if (Dx > 0) {
						x = j * 32 - w; dx = -0.1;
						direction = rand() % (3);//Направление движения врага 
					}//с правым краем карты
					if (Dx < 0) {
						x = j * 32 + 32; dx = 0.1;
						direction = rand() % (3); //Направление движения врага
					}// с левым краем карты
				}
			}
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {//для персонажа с таким именем логика будет такой

			if (life) {//проверяем, жив ли герой
				switch (direction)//делаются различные действия в зависимости от состояния
				{
				case 0: {//состояние идти вправо
					dx = speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 3) CurrentFrame -= 3;
					sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
					break;
				}
				case 1: {//состояние идти влево
					dx = -speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 3) CurrentFrame -= 3;
					sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
					break;
				}
				case 2: {//идти вверх
					dy = -speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 3) CurrentFrame -= 3;
					sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
					break;
				}
				case 3: {//идти вниз
					dy = speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 3) CurrentFrame -= 3;
					sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
					break;
				}
				}

				x += dx * time; //движение по “X”
				checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х

				y += dy * time; //движение по “Y”
				checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y

				sprite.setPosition(x, y); //спрайт в позиции (x, y).

				if (health <= 0) { life = false; }//если жизней меньше 0, либо равно 0, то умираем
			}
		}
	}
};//класс Enemy закрыт


////////////////////////////КЛАСС ПУЛИ////////////////////////
class Bullet :public Entity {//класс пули
public:
	int direction;//направление пули
	Bullet(Image &image, float X, float Y, int W, int H, std::string  Name, int dir) :Entity(image, X, Y, W, H, Name) {
		x = X; //координаты пули на карте игры
		y = Y;
		direction = dir; //направление полета пули
		speed = 0.8;
		w = h = 16; //размеры изображения пули
		life = true; //пуля жива
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

		if (life) { // если пуля жива
			x += dx * time;//само движение пули по х
			y += dy * time;//по у

			for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
				for (int j = x / 32; j < (x + w) / 32; j++)
				{
					if (TileMap[i][j] == '0')//если элемент наш тайлик земли, то //карта (Шагид)
						life = false;// то пуля умирает
				}
			sprite.setPosition(x + w / 2, y + h / 2); //задается позицию пули
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
	//получили нужный нам прямоугольник с танком
	herosprite.setTextureRect(IntRect(343, 177, 499, 313));
	herosprite.setPosition(250, 250); //выводим спрайт в позицию x y 

	Clock clock;
	Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры 
	int gameTime = 0;//объявили игровое время, инициализировали.


	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/enemy.png"); // загружаем изображение врага //его пока что нет, надо скачать и заменить!

	Image BulletImage;//изображение для пули
	BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения

	std::list<Entity*>  enemies; //список врагов
	std::list<Entity*>  Bullets; //список пуль
	std::list<Entity*>::iterator it; //итератор чтобы проходить по элементам списка

	const int ENEMY_COUNT = 1;	//максимальное количество врагов в игре
	int enemiesCount = 0;	//текущее количество врагов в игре

	//Заполняем список объектами врагами
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		float xr = 150 + rand() % 500; // случайная координата врага на поле игры по оси “x”
		float yr = 150 + rand() % 350; // случайная координата врага на поле игры по оси “y”

			//создаем врагов и помещаем в список
		enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
		enemiesCount += 1; //увеличили счётчик врагов
	}

	int createObjectForMapTimer = 0;//Переменная под время для генерирования камней

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//игровое время в //опредлеить игрока (Кирилл)
			//секундах идёт вперед, пока жив игрок. Перезагружать как time его не надо. 
			//оно не обновляет логику игры

		clock.restart();
		time = time / 800;

		createObjectForMapTimer += time;//наращиваем таймер
		if (createObjectForMapTimer > 3000) {
			randomMapGenerate();//генерация  камней //определить препядствия (Шагидыч)
			createObjectForMapTimer = 0;//обнуляем таймер
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//стреляем по нажатию клавиши "P"
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::P)
				{
					Bullets.push_back(new Bullet(BulletImage, p.x, p.y, 16, 16, "Bullet", p.state));
				}
			}
		}

		//оживляем врагов
		for (it = enemies.begin(); it != enemies.end(); it++)
		{
			(*it)->update(time); //запускаем метод update()
		}

		//оживляем пули
		for (it = Bullets.begin(); it != Bullets.end(); it++)
		{
			(*it)->update(time); //запускаем метод update()
		}

		//Проверяем список на наличие "мертвых" пуль и удаляем их
		for (it = Bullets.begin(); it != Bullets.end(); )//говорим что проходимся от начала до конца
		{// если этот объект мертв, то удаляем его
			if ((*it)->life == false) { it = Bullets.erase(it); }
			else  it++;//и идем курсором (итератором) к след объекту.		
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
