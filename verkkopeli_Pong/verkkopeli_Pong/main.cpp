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
#include<map>
#include<conio.h>

/*sf::IpAddress ip = sf::IpAddress::getLocalAddress();
sf::TcpSocket socket;
char connectionType, mode;
char buffer[2000];
std::size_t received;
std::string text = "Connected to: ";

void Server()
{
		sf::TcpListener listener;
		listener.listen(2000);
		listener.accept(socket);
		text += "Server";
		mode = 's';
	
}

void Client()
{
		socket.connect(ip, 2000);
		text += "Client";
		mode = 'r';

}*/

int main()
{
	sf::Vector2f velocity;
	//sf::Thread* thread = new sf::Thread(&Server);

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();

	sf::UdpSocket socket;
	char connectionType, mode;
	char buffer[2000];
	std::size_t received;
	std::map<unsigned short, sf::IpAddress> computerID;
	std::string text = "Connected to: ";
	

	std::cout << "Enter (s) for server, enter (c) for client: " << std::endl;
	std::cin >> connectionType;

	unsigned short port;
	std::cout << "Set port number: ";
	std::cin >> port;

	socket.bind(port);

	if (connectionType == 's')
	{
		//Server();

		//thread->launch();

			sf::IpAddress rIp;
			unsigned short port;
			socket.receive(buffer, sizeof(buffer), received, rIp, port);
			if (received > 0)
				computerID[port] = rIp;
	
	}
	else
	{
		std::string sIp;
		std::cout << "Enter server ip: ";
		std::cin >> sIp;
		sf::IpAddress sendIP(sIp);
		socket.send(text.c_str(), text.length() + 1, sendIP, 2000);
	}

	bool done = false;

	while (!done)
	{	if (connectionType == 's')
		{
			std::getline(std::cin, text);
			std::map<unsigned short, sf::IpAddress>::iterator tempIterator;
			for (tempIterator = computerID.begin(); tempIterator != computerID.end(); tempIterator++)
				socket.send(text.c_str(), text.length() + 1, tempIterator->second, tempIterator->first);
		}

		else
		{
			sf::IpAddress tempIp;
			unsigned short tempPort;
			socket.receive(buffer, sizeof(buffer), received, tempIp, tempPort);
			if (received > 0)
				std::cout << "Received: " << buffer << std::endl;
		}
	}

			// Chat thingy, not needed
			/*
			socket.send(text.c_str(), text.length() + 1);

			socket.receive(buffer, sizeof(buffer), received);

			std::cout << buffer << std::endl;

			bool done = false;

			while (!done)
			{
				if (mode == 's')
				{
					std::getline(std::cin, text);
					socket.send(text.c_str(), text.length() + 1);
					mode = 'r';
				}
				else if (mode == 'r')
				{
					socket.receive(buffer, sizeof(buffer), received);
					if (received > 0)
					{
						std::cout << "Received: " << buffer << std::endl;
						mode = 's';
					}
				}
			}*/



			//////////////////// G A M E //////////////////////////////////////////////////////////////////////////////////

			//window1
			//sf::RenderWindow window1(sf::VideoMode(900, 700), "Window 1");
			//window1.setPosition(sf::Vector2i(0, 0));

			// Game window
			sf::RenderWindow window2(sf::VideoMode(900, 700), "Client Window");
			window2.setPosition(sf::Vector2i(900, 300));

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

			while (window2.isOpen())
			{
				if (playing)
				{
					//sf::Event event1;
					//while (window1.pollEvent(event1))
					//{
					//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					//	{
					//		window1.close();
					//	}
					//}

					sf::Event event2;
					while (window2.pollEvent(event2))
					{
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
						{
							window2.close();
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

					// Players' movements ////////////////////////////////////////////////////////

					// Right player movement:
					// Player movement upwards
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
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

					// Player movement downwards
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
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
					// Left player movement:

					// Player movement upwards
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
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

					// Player movement downwards
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					{
						leftPlayer.move(0, 0.3f);
						//std::cout << "No collision" << std::endl;

						// if the player hits the wall, it stops
						if (leftPlayer.getGlobalBounds().intersects(borderBottom.getGlobalBounds()))
						{
							//std::cout << "Collision" << std::endl;
							leftPlayer.move(0, -0.3f);
						}
						//////////////////////////////////////////////////////////////////////////////////////
					}

				}

				//window1.clear();
				window2.clear();

				//window1.draw(borderTop);
				//window1.draw(borderBottom);
				//window1.draw(borderLeft);
				//window1.draw(borderRight);
				//window1.draw(line, 2, sf::Lines);

				window2.draw(borderTop);
				window2.draw(borderBottom);
				window2.draw(borderLeft);
				window2.draw(borderRight);
				window2.draw(line, 2, sf::Lines);

				//window1.draw(leftPlayer);
				//window1.draw(rightPlayer);

				window2.draw(leftPlayer);
				window2.draw(rightPlayer);

				//window1.draw(ball);
				window2.draw(ball);

				//window1.display();
				window2.display();

			}

	
	


	system("pause");

	//thread->wait();
	//delete thread;

	return 0;
}

