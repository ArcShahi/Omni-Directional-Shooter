
#include <SFML/Graphics.hpp>
#include <SFML\Audio.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include "random.hpp"

/// Use class later

/*
* Possilbe Re-structuring for readability
* Enemies moving towards player
* Health Bar
*/

struct Entity
{
	sf::RectangleShape shape{};
	sf::Vector2f curVelocity{};
	float maxSpeed{};

	Entity(const sf::Vector2f& dim = { 10.0f,5.0f })
		:curVelocity(0.f, 0.f), maxSpeed(15.f)
	{
		this->shape.setFillColor(sf::Color::Cyan);
		this->shape.setSize(dim);
	}
};

int main()
{


	sf::RenderWindow window(sf::VideoMode(1920,1080), "All Rounder");
	window.setFramerateLimit(120);

	sf::Clock clock{};

	//Player
	sf::CircleShape player{ 25.0f };
	player.setOutlineThickness(5.0f);
	player.setOutlineColor(sf::Color::Cyan);
	float distance{};
	sf::Vector2f playerCenter{};

	///vectors
	sf::Vector2f mousePosWindow{};
	sf::Vector2f aimDirection{};
	sf::Vector2f aimDirNorm{};


	/// lazer
	Entity bullet{};

	std::vector<Entity> magzine{};

	///enemy

	sf::CircleShape enemy{ 40.0f };
	enemy.setPointCount(3);
	enemy.setFillColor(sf::Color::Red);
	short spawnCounter{ 200 };

	std::vector<sf::CircleShape> eArmy{};





	/// Music

	sf::Music music;
	if (!music.openFromFile("sfx/rumors.ogg"))
		std::cerr << "Couldn't open File\n"; // error
	music.play();

	///sfx

	//gunshot
	sf::SoundBuffer buff;
	if (!buff.loadFromFile("sfx/gun.ogg"))
		std::cerr << "couldn't laod gun.ogg";

	sf::Sound gunshot;
	gunshot.setBuffer(buff);

	//enemy destroyed

	sf::SoundBuffer destBuff;

	if (!destBuff.loadFromFile("sfx/boom.ogg"))
		std::cerr << "couldn't laod gun.ogg";

	sf::Sound dest;
	dest.setBuffer(destBuff);


	//Game loop
	while (window.isOpen())
	{
		sf::Time deltaTime = clock.restart();

		sf::Event event{};
		while (window.pollEvent(event)) // Event Handler
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}


		window.clear();

		/// Vector
		playerCenter = { player.getPosition().x + player.getRadius(),player.getPosition().y + player.getRadius() };
		mousePosWindow = sf::Vector2f(sf::Mouse::getPosition(window));

		aimDirection = mousePosWindow - playerCenter;
		aimDirNorm = aimDirection / std::sqrt(std::pow(aimDirection.x, 2.0f) + std::pow(aimDirection.y, 2.0f));


		/// UPDATE PLayer




		// Don't go out of bounds

		// Get the size of the window
		sf::Vector2u windowSize = window.getSize();

		// Prevent moving out of bounds in the vertical direction
		if (player.getPosition().y > windowSize.y - player.getGlobalBounds().height)
		{
			player.setPosition(player.getPosition().x, windowSize.y - player.getGlobalBounds().height);
		}
		else if (player.getPosition().y < 0)
		{
			player.setPosition(player.getPosition().x, 0.f);
		}

		// Prevent moving out of bounds in the horizontal direction
		if (player.getPosition().x > windowSize.x - player.getGlobalBounds().width)
		{
			player.setPosition(windowSize.x - player.getGlobalBounds().width, player.getPosition().y);
		}
		else if (player.getPosition().x < 0)
		{
			player.setPosition(0.f, player.getPosition().y);
		}



		float speed = 250.f;
		distance = speed * deltaTime.asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			player.move(-distance, 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			player.move(distance, 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			player.move(0.f, -distance);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			player.move(0.f, distance);
		}

		/// Enemy spawn

		if (spawnCounter < 500)
		{
			spawnCounter+=10;
		}

		if (spawnCounter >= 500 && eArmy.size() < 500)
		{
			enemy.setPosition(sf::Vector2f(getRandom(0.f, window.getSize().x), getRandom(0.f, window.getSize().x)));
			eArmy.emplace_back(enemy);
			spawnCounter = 0;
		}




		/// Shooting
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{

			gunshot.play();
			bullet.shape.setPosition(playerCenter);
			bullet.curVelocity = aimDirNorm * bullet.maxSpeed;

			magzine.emplace_back(bullet);
		}

		// Moving the bullets
		for (unsigned int x = 0; x < magzine.size(); x++)
		{
			magzine[x].shape.move(magzine[x].curVelocity);

			//check out of bounds
			if (magzine[x].shape.getPosition().x<0 || magzine[x].shape.getPosition().x > window.getSize().x ||
				magzine[x].shape.getPosition().y < 0 || magzine[x].shape.getPosition().y > window.getSize().y)
			{
				magzine.erase(magzine.begin() + x);

			}
			else
			{
				/// Enermy Collison

				for (unsigned int e = 0; e < eArmy.size(); e++)
				{
					if (magzine[x].shape.getGlobalBounds().intersects(eArmy[e].getGlobalBounds()))
					{
						dest.play();
						magzine.erase(magzine.begin() + x);
						eArmy.erase(eArmy.begin() + e);
					}
				}
			}




		}


		/// Draw the PLayer

		for (unsigned int i = 0; i < eArmy.size(); i++)
		{
			window.draw(eArmy[i]);
		}

		window.draw(player);

		//fire bullets
		for (const auto& fire : magzine)
		{
			window.draw(fire.shape);
		}

		window.display();


	}
	std::cin.get();
}