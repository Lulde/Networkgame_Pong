// Pong Network game
// Suvi Vikstedt & Laura Nevala

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <SFML/Window.hpp>
#include <string>
#include <map>
#include <conio.h>


sf::IpAddress ip = sf::IpAddress::getLocalAddress();
sf::UdpSocket socket;
char connectionType, mode;
char buffer[2000];
std::size_t received;
std::map<unsigned short, sf::IpAddress> computerID;
std::string text;
sf::Vector2f velocity;
               

void runGame()
{

	//window
	sf::RenderWindow window(sf::VideoMode(900, 700), "Pong");
	window.setPosition(sf::Vector2i(0, 0));


	//Left Player
	sf::RectangleShape leftPlayer(sf::Vector2f(10.f, 130.f));
	leftPlayer.setFillColor(sf::Color::Magenta);
	leftPlayer.setOutlineColor(sf::Color::Blue);
	leftPlayer.setOutlineThickness(2.f);
	leftPlayer.setPosition(sf::Vector2f(20, 60));

	//Right Player
	sf::RectangleShape rightPlayer(sf::Vector2f(10.f, 130.f));
	rightPlayer.setFillColor(sf::Color::Magenta);
	rightPlayer.setOutlineColor(sf::Color::Blue);
	rightPlayer.setOutlineThickness(2.f);
	rightPlayer.setPosition(sf::Vector2f(870, 60));

	//Border top
	sf::RectangleShape borderTop(sf::Vector2f(898.f, 2.f));
	borderTop.setFillColor(sf::Color::White);
	borderTop.setPosition(sf::Vector2f(1, 1));

	//Border bottom
	sf::RectangleShape borderBottom(sf::Vector2f(898.f, 2.f));
	borderBottom.setFillColor(sf::Color::White);
	borderBottom.setPosition(sf::Vector2f(1, 697));

	//Border left
	sf::RectangleShape borderLeft(sf::Vector2f(2.f, 698.f));
	borderLeft.setFillColor(sf::Color::White);
	borderLeft.setPosition(sf::Vector2f(1, 1));

	//Border right
	sf::RectangleShape borderRight(sf::Vector2f(2.f, 698.f));
	borderRight.setFillColor(sf::Color::White);
	borderRight.setPosition(sf::Vector2f(897, 1));

	// Middle line
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(900 / 2 + 1, 0)),
		sf::Vertex(sf::Vector2f(900 / 2 + 1, 700))
	};



	// Create the ball
	float ballRadius = 10.f;
	sf::CircleShape ball;
	//Ball radius minus the outline thickness of the ball
	ball.setRadius(ballRadius - 3);
	ball.setOutlineThickness(3);
	ball.setOutlineColor(sf::Color::Black);
	ball.setFillColor(sf::Color::White);
	//ball origin is (x and y): ballRadius divided by 2
	ball.setOrigin(ballRadius / 2, ballRadius / 2);
	//set constant speed for the ball
	float ballSpeed = 300.f;
	float ballAngle = 75.f;
	//Reset ball position
	ball.setPosition(900 / 2, 700 / 2);




	//run prog as long as the window is open
	sf::Clock clock;
	bool playing = true;

	while (window.isOpen())
	{
		if (playing)
		{
			sf::Event event1;
			while (window.pollEvent(event1))
			{
				//close the window
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window.close();
				}

			}

			// Ball movement/////////////////////////////////////////////////////////////////

			float deltaTime = clock.restart().asSeconds();
			float factor = deltaTime * ballSpeed;
			velocity.x = std::cos(ballAngle)*factor;
			velocity.y = std::sin(ballAngle)*factor;


			ball.move(velocity.x, velocity.y);

			if (ball.getGlobalBounds().intersects(rightPlayer.getGlobalBounds()) || ball.getGlobalBounds().intersects(leftPlayer.getGlobalBounds()))
			{
				//ball.move(sf::Vector2f (-(velocity.x), -(velocity.y)));
				velocity.x = -(velocity.x);
				ballSpeed = -ballSpeed;
				ballAngle = -ballAngle;
			}

			if (ball.getGlobalBounds().intersects(borderBottom.getGlobalBounds()) || ball.getGlobalBounds().intersects(borderTop.getGlobalBounds()))
			{
				velocity.x = -velocity.x;
				ballAngle = -ballAngle;
			}

			if (ball.getGlobalBounds().intersects(borderLeft.getGlobalBounds()) || ball.getGlobalBounds().intersects(borderRight.getGlobalBounds()))
			{
				ball.setPosition(900 / 2, 700 / 2);
			}


			char leftDown[100] = { 'S' };
			//TARVITAAN SOCKET.SEND/////////////////////////////////////////////////////////////
	
			if (connectionType == 'c')
			{
				sf::IpAddress recipient = "192.168.10.37";
				unsigned short port1 = 2000;

				// Right player movement////////////////////////////////////////////////////////
				// Player movement upwards
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					char rightUp[] = { 'U' };
					socket.send(rightUp, text.length() + 1, recipient, port1);
				}
				// Player movement downwards
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					char rightDown[] = { 'D' };
					socket.send(rightDown, text.length() + 1, recipient, port1);
				}

				// Left player movement/////////////////////////////////////////////////////////
				// Player movement upwards
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					char leftUp[] = { 'W' };
					socket.send(leftUp, text.length() + 1, recipient, port1);
					std::cout << "W pushed" << std::endl;
				}
				// Player movement downwards
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{

					std::cout << "Arvot ovat oikein" << std::endl;
					socket.send(leftDown, 100, recipient, port1);
					std::cout << "S pushed" << std::endl;
				}
			}

			//TARVITAAN SOCKET.RECEIVE
			if (connectionType == 's')
			{
				sf::IpAddress serversIP = "192.168.10.37";
				unsigned short PORT = 2001;


				socket.receive(leftDown, 100, received, serversIP, PORT);
				std::cout << "Received " << received << " bytes from " << serversIP << " on port " << PORT << std::endl;
				if (received == 'S')
				{
					leftPlayer.move(0, 0.3f);
					//std::cout << "No collision" << std::endl;

					// if the player hits the wall, it stops
					if (leftPlayer.getGlobalBounds().intersects(borderBottom.getGlobalBounds()))
					{
						//std::cout << "Collision" << std::endl;
						leftPlayer.move(0, -0.3f);
					}
				}

				if (received == 'W')
				{
					leftPlayer.move(0, -0.3f);
					//std::cout << "No collision" << std::endl;

					// If the player hits the wall, it stops
					if (leftPlayer.getGlobalBounds().intersects(borderTop.getGlobalBounds()))
					{
						//std::cout << "Collision" << std::endl;
						leftPlayer.move(0, 0.3f);
					}
				}
				if (received == 'D')
				{
					rightPlayer.move(0, 0.3f);
					//std::cout << "No collision" << std::endl;

					// if the player hits the wall, it stops
					if (rightPlayer.getGlobalBounds().intersects(borderBottom.getGlobalBounds()))
					{
						rightPlayer.move(0, -0.3f);
						//std::cout << "Collision" << std::endl;
					}
				}
				if (received == 'U')
				{
					rightPlayer.move(0, -0.3f);
					//std::cout << "No collision" << std::endl;

					// If the player hits the wall, it stops
					if (rightPlayer.getGlobalBounds().intersects(borderTop.getGlobalBounds()))
					{
						rightPlayer.move(0, 0.3f);
						//std::cout << "Collision" << std::endl;
					}
				}
			}



			//Piirtäminen tapahtuu tässä
			window.clear();

			window.draw(borderTop);
			window.draw(borderBottom);
			window.draw(borderLeft);
			window.draw(borderRight);
			window.draw(line, 2, sf::Lines);

			window.draw(leftPlayer);
			window.draw(rightPlayer);

			window.draw(ball);

			window.display();


		}
	}
}

int main()
{

	std::cout << "Enter (s) for server, enter (c) for client: " << std::endl;
	std::cin >> connectionType;

	unsigned short port;
	std::cout << "set port number: ";
	std::cin >> port;

	socket.bind(port);


	if (connectionType == 's')
	{
		sf::IpAddress rIp;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, rIp, port);
		std::cout << "Client was connected to the server" << std::endl;
		if (received > 0)
		{
			computerID[port] = rIp;
		}
		runGame(); // doesn't work properly, window opens after client connects, but it crashes
	}
	else
	{
		std::string sIp;
		std::cout << "Enter server ip: ";
		std::cin >> sIp;
		sf::IpAddress sendIP(sIp);
		socket.send(text.c_str(), text.length() + 1, sendIP, 2000);
	}

	system("pause");

	return 0;

}

