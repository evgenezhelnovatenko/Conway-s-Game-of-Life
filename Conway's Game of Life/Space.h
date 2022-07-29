#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>


class Space : public sf::Drawable, public sf::Transformable
{

public:
	Space(const sf::Vector2u& windowSize
		, const sf::Vector2i& size
		, const unsigned int& cellSize
		, const unsigned int& gridWidth
		, const sf::Color& gridColor
		, const sf::Color& liveCellColor);

	Space(const sf::Vector2u& windowSize
		, const sf::Vector2i& size
		, const unsigned int& cellSize
		, const sf::Color& liveCellColor);

	void initialisation(const std::vector<sf::Vector2i>& preset);
	sf::Vector2i getSize() const;
	void setWidth(const int &width);
	int getCellSize() const;
	void setPosition(const sf::Vector2i& position);
	sf::Vector2i getPosition() const;
	sf::VertexArray& getGridLineVertices() const;
	void setLiveCells(const std::vector<sf::Vector2i>& set);
	const std::vector<sf::Vector2i>& getLiveCells() const;
	bool isCellAlive(const sf::Vector2i& cell) const;
	bool addLiveCell(sf::Vector2i cellCoords);
	bool removeLiveCell(sf::Vector2i cellCoords);
	void convertRelativeCoordsToAbsoluteCoords(sf::Vector2i& coords) const;
	void convertAbsoluteCoordsToRelativeCoords(sf::Vector2i& coords) const;
	void createGrid();
	bool update();
	void clear();
	
	static void convertToLocalCoords(sf::Vector2i& coords, const int& cellSize);
	static void convertToGlobalCoords(sf::Vector2i& coords, const int& cellSize);
	static bool addLiveCellToSet(std::vector<sf::Vector2i>& liveCellsSet, const sf::Vector2i& cellCoords);
	static bool removeLiveCellFromSet(std::vector<sf::Vector2i>& liveCellsSet, const sf::Vector2i& cellCoords);
	static bool isCellInTheSet(const std::vector<sf::Vector2i>& liveCellsSet, const sf::Vector2i& cell);
	static void showLiveCells(const std::vector<sf::Vector2i>& liveCellsSet);

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void convertToLocalCoords(sf::Vector2i& coords) const;
	void convertToGlobalCoords(sf::Vector2i& coords) const;
	int findTheNumberOfLiveNeighbors(const sf::Vector2i& cellCoords) const;
	
	void convertVerticesCoordsToAbsoluteCoords();
	void convertLiveCellsCoordsToAbsoluteCoords();



private:

	sf::Vector2u _windowSize;
	int _width;
	int _height;
	int _cellSize;
	int _gridWidth;
	sf::Vector2i _position;
	sf::Color _gridColor;
	sf::Color _liveCellColor;
	
	std::unique_ptr<sf::VertexArray> _gridLineVertices;
	std::vector<sf::Vector2i> _liveCells;
	
};

