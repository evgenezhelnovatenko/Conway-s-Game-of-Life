#include <SFML/Graphics.hpp>
#include "Space.h"
#include <iostream>

#define MOVE_SPEED 10

void updateDt(sf::Clock& clock, float& dt)
{
	dt = clock.restart().asSeconds();
}

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

void createOverlaySpace(Space*& overlaySpace, const sf::Vector2u& windowSize, const sf::Vector2i& mousePosition, const std::vector<sf::Vector2i>& set, const int& cellSize)
{
	sf::Vector2i overlaySpaceSize = computeSpaceSizeByLiveCellsSet(set);
	overlaySpace = new Space(windowSize, overlaySpaceSize, cellSize, sf::Color(230, 230, 230, 255));

	sf::Vector2i overlaySpaceCoords = mousePosition - overlaySpace->getSize() / 2;
	overlaySpace->setLiveCells(set);
	overlaySpace->setPosition(overlaySpaceCoords);
}

std::vector<sf::Vector2i> convertLiveCellsCoordsToRelativeSpaceCoords(const Space& space, const Space& overlaySpace)
{
	std::vector<sf::Vector2i> overlaySpaceLiveCellsCoords = overlaySpace.getLiveCells();

	// Convert all live cells relative overlaySpace coords to relative space coords.
	for (auto& liveCell : overlaySpaceLiveCellsCoords)
	{
		Space::convertToGlobalCoords(liveCell, overlaySpace.getCellSize());
		overlaySpace.convertRelativeCoordsToAbsoluteCoords(liveCell);
		space.convertAbsoluteCoordsToRelativeCoords(liveCell);
	}

	return overlaySpaceLiveCellsCoords;
}

bool updateGame(Space &space, sf::Clock &clock, float& dt) {
	
	sf::Time elapsed = clock.getElapsedTime();
	if (elapsed.asSeconds() >= MOVE_SPEED / 200)
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

	std::vector<sf::Vector2i> set;

	const int CELL_SIZE = 15;
	const int GRID_WIDTH = 1;

	Space space(window.getSize(), sf::Vector2i(100 * CELL_SIZE, 70 * CELL_SIZE), CELL_SIZE, GRID_WIDTH, sf::Color(245, 245, 245, 100), sf::Color::Cyan);
	sf::Vector2i windowCenterCoords = sf::Vector2i(window.getSize()) / 2;
	sf::Vector2i spaceCoords = windowCenterCoords - space.getSize() / 2;
	space.setPosition(spaceCoords);
	space.createGrid();

	Space* overlaySpace = nullptr;

	float dt = 0.f;
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
				switch (event.key.code)
				{
				case sf::Keyboard::Space:
				{
					isGameInProgress = !isGameInProgress;
					std::cout << "The game has begun\n";
					break;
				}
				case sf::Keyboard::C:
				{
					space.clear();
					break;
				}
				case sf::Keyboard::Escape:
				{
					isBuildMode = false;
					if (overlaySpace != nullptr)
					{
						delete overlaySpace;
						overlaySpace = nullptr;
					}
					break;
				}
				case sf::Keyboard::Numpad0:
				{
					Space::showLiveCells(space.getLiveCells());
					break;
				}
				case sf::Keyboard::Numpad1:
				{
					set = { { 1, 2 }, { 2, 3 }, { 3, 3 }, { 3, 2 }, { 3, 1 } };

					createOverlaySpace(overlaySpace, window.getSize(), mousePosition, set, CELL_SIZE);

					isBuildMode = true;
					break;
				}
				case sf::Keyboard::Numpad2:
				{
					set = { { 0, 4 }, { 1, 4 }, { 1, 5 }, { 0, 5 }, { 10, 4 }, { 10, 5 }, { 10, 6 }, { 11, 3 },
							{ 12, 2 }, { 13, 2 }, { 15, 3 }, { 16, 4 }, { 16, 5 }, { 16, 6 }, { 17, 5 }, { 14, 5 },
							{ 15, 7 }, { 13, 8 }, { 12, 8 }, { 11, 7 }, { 20, 4 }, { 21, 4 }, { 21, 3 }, { 21, 2 },
							{ 20, 2 }, { 20, 3 }, { 22, 1 }, { 24, 0 }, { 24, 1 }, { 22, 5 }, { 24, 5 }, { 24, 6 },
							{ 34, 2 }, { 35, 2 }, { 35, 3 }, { 34, 3 }
					};

					createOverlaySpace(overlaySpace, window.getSize(), mousePosition, set, CELL_SIZE);

					isBuildMode = true;
					break;
				}
				case sf::Keyboard::Numpad3:
				{
					set = { { 6, 2 }, { 6, 1 }, { 5, 1 }, { 5, 2 }, { 4, 2 }, { 3, 3 }, { 3, 4 }, { 2, 4 }, { 2, 5 },
							{ 2, 6 }, { 2, 7 }, { 2, 8 }, { 3, 9 }, { 3, 10 }, { 4, 10 }, { 4, 11 }, { 5, 11 }, { 6, 11 },
							{ 6, 12 }, { 7, 12 }, { 8, 12 }, { 9, 12 }, { 10, 12 }, { 11, 12 }, { 12, 12 }, { 12, 11 },
							{ 12, 10 }, { 13, 10 }, { 13, 9 }, { 13, 8 }, { 13, 7 }, { 13, 6 }, { 13, 5 }, { 12, 5 },
							{ 12, 4 }, { 12, 3 }, { 11, 3 }, { 11, 2 }, { 10, 2 }, { 9, 2 }, { 8, 2 }, { 8, 1 }, { 7, 1 },
							{ 4, 12 }, { 4, 13 }, { 3, 13 }, { 3, 14 }, { 3, 15 }, { 3, 16 }, { 3, 17 }, { 3, 18 }, { 3, 19 },
							{ 3, 20 }, { 4, 20 }, { 4, 21 }, { 5, 21 }, { 5, 22 }, { 6, 22 }, { 7, 22 }, { 8, 23 }, { 9, 23 },
							{ 10, 23 }, { 11, 23 }, { 12, 23 }, { 12, 22 }, { 13, 22 }, { 14, 22 }, { 14, 21 }, { 15, 21 },
							{ 15, 20 },	{ 16, 20 }, { 16, 19 }, { 16, 18 }, { 16, 17 }, { 16, 16 }, { 16, 15 }, { 15, 15 },
							{ 15, 14 }, { 14, 14 },	{ 14, 13 }, { 13, 13 }, { 12, 13 }, { 14, 7 }, { 15, 7 }, { 15, 8 },
							{ 16, 8 }, { 17, 8 }, { 18, 8 }, { 19, 8 },	{ 20, 8 }, { 21, 8 }, { 22, 8 }, { 23, 8 }, { 24, 8 },
							{ 25, 8 }, { 25, 7 }, { 26, 7 }, { 27, 7 }, { 28, 7 }, { 29, 7 }, { 30, 7 }, { 30, 6 }, { 31, 6 },
							{ 32, 6 }, { 33, 6 }, { 33, 5 }, { 34, 5 }, { 35, 5 }, { 35, 4 }, { 36, 4 }, { 37, 4 }, { 37, 3 },
							{ 38, 3 }, { 39, 3 }, { 39, 2 }, { 40, 2 }, { 41, 2 }, { 42, 2 }, { 43, 2 }, { 44, 2 }, { 44, 3 },
							{ 45, 3 }, { 46, 3 }, { 46, 4 }, { 47, 4 }, { 47, 5 }, { 47, 6 }, { 48, 6 }, { 48, 7 }, { 48, 8 },
							{ 49, 8 }, { 49, 9 }, { 49, 10 }, { 49, 11 }, { 49, 12 }, { 49, 13 }, { 48, 13 }, { 48, 14 },
							{ 47, 15 }, { 46, 15 }, { 46, 16 }, { 45, 16 }, { 44, 16 }, { 43, 16 }, { 42, 16 }, { 41, 16 },
							{ 40, 17 }, { 39, 17 }, { 38, 17 }, { 37, 17 }, { 36, 17 }, { 35, 17 }, { 34, 17 }, { 33, 17 },
							{ 32, 17 }, { 31, 17 }, { 30, 17 }, { 29, 17 }, { 28, 17 }, { 27, 17 }, { 26, 17 }, { 25, 17 },
							{ 24, 17 }, { 23, 17 }, { 23, 16 }, { 22, 16 }, { 21, 16 }, { 20, 16 }, { 19, 16 }, { 18, 16 },
							{ 17, 16 }, { 17, 15 }, { 34, 6 }, { 33, 7 }, { 33, 8 }, { 33, 9 }, { 33, 10 }, { 33, 11 },
							{ 33, 12 }, { 33, 13 }, { 34, 13 }, { 34, 14 }, { 34, 15 }, { 35, 15 }, { 35, 16 }, { 34, 12 },
							{ 35, 12 }, { 36, 12 }, { 37, 12 }, { 38, 12 }, { 39, 12 }, { 39, 11 }, { 40, 11 }, { 41, 11 },
							{ 42, 11 }, { 42, 10 }, { 43, 10 }, { 44, 10 }, { 44, 9 }, { 45, 9 }, { 45, 8 }, { 46, 8 },
							{ 46, 7 }, { 47, 7 }
					};

					createOverlaySpace(overlaySpace, window.getSize(), mousePosition, set, CELL_SIZE);

					isBuildMode = true;
					break;
				}
				default:
					break;
				}
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left) {
					if (isBuildMode)
					{
						std::vector<sf::Vector2i> liveCellsCoordsRelativeSpace = convertLiveCellsCoordsToRelativeSpaceCoords(space, *overlaySpace);
						space.addLiveCells(liveCellsCoordsRelativeSpace);
					}
					else
					{
						space.convertAbsoluteCoordsToRelativeCoords(mousePosition);
						space.addLiveCell(mousePosition);
					}

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
					}
						
				}
				else
				{
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

		}
		window.clear(sf::Color(51, 51, 51, 0));

		
		if (isGameInProgress)
		{
			isGameInProgress = updateGame(space, clock, dt);
			if (!isGameInProgress)
				std::cout << "The game has stopped." << std::endl;
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