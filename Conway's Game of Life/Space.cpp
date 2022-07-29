#include "Space.h"
#include <iostream>

Space::Space(const sf::Vector2u& windowSize
	, const sf::Vector2i& size
	, const unsigned int& cellSize
	, const unsigned int& gridWidth
	, const sf::Color& gridColor
	, const sf::Color& liveCellColor)
	: _windowSize(windowSize)
	, _width(size.x / cellSize)
	, _height(size.y / cellSize)
	, _cellSize(cellSize)
	, _gridWidth(gridWidth)
	, _gridColor(gridColor)
	, _liveCellColor(liveCellColor)
	, _gridLineVertices(std::make_unique<sf::VertexArray>())
{

}

Space::Space(const sf::Vector2u& windowSize
			, const sf::Vector2i& size
			, const unsigned int& cellSize
			, const sf::Color& liveCellColor)
	: _windowSize(windowSize)
	, _width(size.x)
	, _height(size.y)
	, _cellSize(cellSize)
	, _gridWidth(0)
	, _liveCellColor(liveCellColor)
	, _gridLineVertices(nullptr)
{

}

void Space::initialisation(const std::vector<sf::Vector2i>& preset)
{
	_liveCells = preset;
}

sf::Vector2i Space::getSize() const
{
	return sf::Vector2i(_width, _height) * _cellSize;
}

void Space::setWidth(const int &width)
{
	if (_width == width)
		return;
	_width = width;
}

int Space::getCellSize() const
{
	return _cellSize;
}

void Space::setLiveCells(const std::vector<sf::Vector2i>& set)
{
	if (_liveCells == set)
		return;

	_liveCells = set;
}

const std::vector<sf::Vector2i>& Space::getLiveCells() const
{
	return _liveCells;
}

bool Space::isCellAlive(const sf::Vector2i& cell) const
{
	return isCellInTheSet(_liveCells, cell);
}

sf::VertexArray& Space::getGridLineVertices() const
{
	return *_gridLineVertices;
}

sf::Vector2i Space::getPosition() const
{
	return _position;
}

void Space::setPosition(const sf::Vector2i& position)
{
	if (_position == position)
		return;
	_position = position;
}

bool Space::addLiveCell(sf::Vector2i cellCoords)
{
	convertToLocalCoords(cellCoords);
	return addLiveCellToSet(_liveCells, cellCoords);
}

bool Space::removeLiveCell(sf::Vector2i cellCoords)
{
	convertToLocalCoords(cellCoords);
	return removeLiveCellFromSet(_liveCells, cellCoords);
}

void Space::createGrid()
{
	_gridLineVertices->setPrimitiveType(sf::Quads);
	_gridLineVertices->resize((_width + _height + 2) * 4);

	int j = 0;
	for (int i = 0; i <= _width; i++)
	{
		// Vertical line.
		(*_gridLineVertices)[j].position = sf::Vector2f(i * _cellSize, 0);
		(*_gridLineVertices)[j + 1].position = sf::Vector2f(i * _cellSize + _gridWidth, 0);
		(*_gridLineVertices)[j + 2].position = sf::Vector2f(i * _cellSize + _gridWidth, _height * _cellSize + _gridWidth);
		(*_gridLineVertices)[j + 3].position = sf::Vector2f(i * _cellSize, _height * _cellSize + _gridWidth);

		// Set line's color
		(*_gridLineVertices)[j].color = _gridColor;
		(*_gridLineVertices)[j + 1].color = _gridColor;
		(*_gridLineVertices)[j + 2].color = _gridColor;
		(*_gridLineVertices)[j + 3].color = _gridColor;

		j += 4;
	}

	for (int i = 0; i <= _height; i++)
	{
		// Horizontal line.
		(*_gridLineVertices)[j].position = sf::Vector2f(0, i * _cellSize);
		(*_gridLineVertices)[j + 1].position = sf::Vector2f(_width * _cellSize, i * _cellSize);
		(*_gridLineVertices)[j + 2].position = sf::Vector2f(_width * _cellSize, i * _cellSize + _gridWidth);
		(*_gridLineVertices)[j + 3].position = sf::Vector2f(0, i * _cellSize + _gridWidth);

		// Set line's color
		(*_gridLineVertices)[j].color = _gridColor;
		(*_gridLineVertices)[j + 1].color = _gridColor;
		(*_gridLineVertices)[j + 2].color = _gridColor;
		(*_gridLineVertices)[j + 3].color = _gridColor;

		j += 4;
	}

	convertVerticesCoordsToAbsoluteCoords();
}

bool Space::update()
{
	std::vector<sf::Vector2i> newLiveCellsSet(_liveCells);
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			sf::Vector2i currCell(j, i);
			int numberOfLiveNeighbors = findTheNumberOfLiveNeighbors(currCell);

			if (isCellAlive(currCell)
				&& (numberOfLiveNeighbors < 2 || numberOfLiveNeighbors > 3))
				removeLiveCellFromSet(newLiveCellsSet, currCell);
			else if (!isCellAlive(currCell)
				&& numberOfLiveNeighbors == 3)
				addLiveCellToSet(newLiveCellsSet, currCell);
		}
	}

	if (_liveCells.empty())
		return false;
	if (_liveCells == newLiveCellsSet)
		return false;
	else
		_liveCells = newLiveCellsSet;

	return true;
}

void Space::clear()
{
	_liveCells.clear();
}

void Space::convertToLocalCoords(sf::Vector2i& coords, const int& cellSize)
{
	coords /= cellSize;
}

void Space::convertToGlobalCoords(sf::Vector2i& coords, const int& cellSize)
{
	coords *= cellSize;
}

bool Space::addLiveCellToSet(std::vector<sf::Vector2i>& liveCellsSet, const sf::Vector2i& cellCoords)
{
	if (isCellInTheSet(liveCellsSet, cellCoords))
		return false;
	else
		liveCellsSet.push_back(cellCoords);

	return true;
}

bool Space::removeLiveCellFromSet(std::vector<sf::Vector2i>& liveCellsSet, const sf::Vector2i& cellCoords)
{
	auto elementIter = std::find(begin(liveCellsSet), end(liveCellsSet), cellCoords);
	if (elementIter != std::end(liveCellsSet))
		liveCellsSet.erase(elementIter);
	else
		return false;

	return true;
}

void Space::convertRelativeCoordsToAbsoluteCoords(sf::Vector2i& coords) const
{
	coords += _position;
}

void Space::convertAbsoluteCoordsToRelativeCoords(sf::Vector2i& coords) const
{
	coords -= _position;
}

bool Space::isCellInTheSet(const std::vector<sf::Vector2i>& liveCellsSet, const sf::Vector2i& cell)
{
	auto elementIter = std::find(begin(liveCellsSet), end(liveCellsSet), cell);
	return (elementIter != std::end(liveCellsSet));
}

void Space::showLiveCells(const std::vector<sf::Vector2i>& liveCellsSet)
{
	for (auto& liveCell : liveCellsSet)
	{
		std::cout << "{ " << liveCell.x << "; " << liveCell.y << " }, ";
	}
	std::cout << std::endl;
}

void Space::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = NULL;

	// Drawing a grid lines.
	if (_gridLineVertices != nullptr)
		target.draw(*_gridLineVertices, states);

	// Drawing a live cells.

	for (auto liveCell : _liveCells)
	{
		sf::RectangleShape cell(sf::Vector2f(_cellSize, _cellSize));
		cell.setFillColor(_liveCellColor);

		convertToGlobalCoords(liveCell);

		if (liveCell.x < 0 || liveCell.x > getSize().x ||
			liveCell.y < 0 || liveCell.y > getSize().y)
			continue;


		convertRelativeCoordsToAbsoluteCoords(liveCell);
		cell.move(sf::Vector2f(liveCell));

		target.draw(cell);
	}
}

void Space::convertToLocalCoords(sf::Vector2i& coords) const
{
	convertToLocalCoords(coords, _cellSize);
}

void Space::convertToGlobalCoords(sf::Vector2i &coords) const
{
	convertToGlobalCoords(coords, _cellSize);
}

int Space::findTheNumberOfLiveNeighbors(const sf::Vector2i& cellCoords) const
{
	int numberOfLiveNeighbors = 0;
	sf::Vector2i neighborCellCoords(cellCoords.x - 1, cellCoords.y - 1);

	for (; neighborCellCoords.y <= cellCoords.y + 1; neighborCellCoords.y++)
	{
		if (neighborCellCoords.y < 0)
			continue;
		if (neighborCellCoords.y >= _height)
			break;

		for (; neighborCellCoords.x <= cellCoords.x + 1; neighborCellCoords.x++)
		{
			if (neighborCellCoords.x < 0)
				continue;
			if (neighborCellCoords == cellCoords)
				continue;
			if (neighborCellCoords.x >= _width)
				break;

			for (auto& liveCell : _liveCells)
			{
				if (liveCell == neighborCellCoords)
				{
					numberOfLiveNeighbors++;
					break;
				}
			}
		}
		neighborCellCoords.x = cellCoords.x - 1;
	}
	return numberOfLiveNeighbors;
}

void Space::convertVerticesCoordsToAbsoluteCoords()
{
	for (int i = 0; i < _gridLineVertices->getVertexCount(); i++)
		(*_gridLineVertices)[i].position += sf::Vector2f(_position);
}

void Space::convertLiveCellsCoordsToAbsoluteCoords()
{
	for (int i = 0; i < _liveCells.size(); i++)
		_liveCells.at(i) += _position;
}




