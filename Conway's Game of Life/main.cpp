#include <SFML/Graphics.hpp>
#include "Space.h"
#include <iostream>

#define MOVE_SPEED 1

sf::Vector2i computeSpaceSizeByLiveCellsSet(const std::vector<sf::Vector2i>& liveCellsSet)
{
	sf::Vector2i spaceSize(0, 0);
	for (auto& cell : liveCellsSet)
	{
		if (spaceSize.x < cell.x)
			spaceSize.x = cell.x;
		if (spaceSize.y < cell.y)
			spaceSize.y = cell.y;
	}
	return spaceSize;
}

bool updateGame(Space &space, sf::Clock &clock) {
	
	sf::Time elapsed = clock.getElapsedTime();
	if (elapsed.asMilliseconds() >= MOVE_SPEED * 100)
	{
		clock.restart();
		return space.update();
	}

	return true;
}

int main()
{
	// Объект, который, собственно, является главным окном приложения
	sf::RenderWindow window(sf::VideoMode(1720, 1080), "SFML Works!", sf::Style::Close);
	window.setFramerateLimit(120);

	//std::vector<sf::Vector2i> set = {{ 1, 2 }, { 2, 3 }, { 3, 3 }, { 3, 2 }, { 3, 1 }};
	std::vector<sf::Vector2i> set2 = { { 0, 4 }, { 1, 4 }, { 1, 5 }, { 0, 5 }, { 10, 4 }, { 10, 5 }, { 10, 6 }, { 11, 3 },
	{ 12, 2 }, { 13, 2 }, { 15, 3 }, { 16, 4 }, { 16, 5 }, { 16, 6 }, { 17, 5 }, { 14, 5 },
	{ 15, 7 }, { 13, 8 }, { 12, 8 }, { 11, 7 }, { 20, 4 }, { 21, 4 }, { 21, 3 }, { 21, 2 },
	{ 20, 2 }, { 20, 3 }, { 22, 1 }, { 24, 0 }, { 24, 1 }, { 22, 5 }, { 24, 5 }, { 24, 6 },
	{ 34, 2 }, { 35, 2 }, { 35, 3 }, { 34, 3 } };

	const int CELL_SIZE = 30;
	const int GRID_WIDTH = 2;

	Space space(window.getSize(), sf::Vector2i(50 * CELL_SIZE, 25 * CELL_SIZE), CELL_SIZE, GRID_WIDTH, sf::Color(245, 245, 245, 100), sf::Color::Cyan);
	sf::Vector2i windowCenterCoords = sf::Vector2i(window.getSize()) / 2;
	sf::Vector2i spaceCoords = windowCenterCoords - space.getSize() / 2;
	space.setPosition(spaceCoords);
	space.createGrid();

	Space* overlaySpace = nullptr;

	bool isGameInProgress = false;
	bool isBuildMode = false;

	sf::Clock clock;
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen())
	{
		// Обрабатываем очередь событий в цикле
		sf::Event event;

		while (window.pollEvent(event))
		{
			mousePosition = sf::Mouse::getPosition(window);
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == sf::Event::Closed)
				// тогда закрываем его
				window.close();
			if (event.type == sf::Event::KeyPressed) 
			{
				if (event.key.code == sf::Keyboard::Space) 
				{
					isGameInProgress = !isGameInProgress;
				}
				else if (event.key.code == sf::Keyboard::C)
				{
					space.clear();
				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					isBuildMode = false;
					if (overlaySpace != nullptr)
					{
						delete overlaySpace;
						overlaySpace = nullptr;
					}
				}
				else if (event.key.code == sf::Keyboard::Numpad0)
				{
					Space::showLiveCells(space.getLiveCells());
				}
				else if (event.key.code == sf::Keyboard::Numpad2) {
					sf::Vector2i overlaySpaceSize = computeSpaceSizeByLiveCellsSet(set2);
					overlaySpace = new Space(window.getSize(), overlaySpaceSize, CELL_SIZE, sf::Color(230, 230, 230, 255));

					sf::Vector2i overlaySpaceCoords = mousePosition - overlaySpace->getSize() / 2;
					overlaySpace->setLiveCells(set2);
					overlaySpace->setPosition(overlaySpaceCoords);

					isBuildMode = true;
				}
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left) {
					space.convertAbsoluteCoordsToRelativeCoords(mousePosition);
					space.addLiveCell(mousePosition);
				}
				else if (event.key.code == sf::Mouse::Right)
				{
					space.convertAbsoluteCoordsToRelativeCoords(mousePosition);
					space.removeLiveCell(mousePosition);
				}

			}

			if (event.type == sf::Event::MouseMoved)
			{
				if (isBuildMode)
				{
					sf::Vector2i overlaySpaceCoords = mousePosition - overlaySpace->getSize() / 2;
					space.convertAbsoluteCoordsToRelativeCoords(overlaySpaceCoords);
					Space::convertToLocalCoords(overlaySpaceCoords, CELL_SIZE);
					sf::Vector2i prevOverlaySpacePosition = overlaySpace->getPosition();
					space.convertAbsoluteCoordsToRelativeCoords(prevOverlaySpacePosition);
					Space::convertToLocalCoords(prevOverlaySpacePosition, CELL_SIZE);

					if (prevOverlaySpacePosition != overlaySpaceCoords)
					{
						Space::convertToGlobalCoords(overlaySpaceCoords, CELL_SIZE);
						space.convertRelativeCoordsToAbsoluteCoords(overlaySpaceCoords);
						overlaySpace->setPosition(overlaySpaceCoords);
						Space::showLiveCells(overlaySpace->getLiveCells());
					}
						
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					space.convertAbsoluteCoordsToRelativeCoords(mousePosition);
					space.addLiveCell(mousePosition);

				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					space.convertAbsoluteCoordsToRelativeCoords(mousePosition);
					space.removeLiveCell(mousePosition);
				}

			}

		}
		window.clear(sf::Color(51, 51, 51, 0));

		
		if (isGameInProgress)
		{
			isGameInProgress = updateGame(space, clock);
			std::cout << (isGameInProgress ? "Game is in a progress." : "Game has stoped.") << std::endl;
		}
		

		if (overlaySpace != nullptr)
			window.draw(*overlaySpace);
		window.draw(space);
		// Отрисовка окна	
		window.display();
	}

	if (overlaySpace != nullptr)
	{
		delete overlaySpace;
		overlaySpace = nullptr;
	}

	return 0;
}