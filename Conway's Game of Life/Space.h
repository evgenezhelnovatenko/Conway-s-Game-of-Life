#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>


using std::vector, std::unique_ptr;

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

	void initialisation(const vector<sf::Vector2i>& preset);
	sf::Vector2i getSize() const;
	void setWidth(const int &width);
	int getCellSize() const;
	void setPosition(const sf::Vector2i& position);
	sf::Vector2i getPosition() const;
	sf::VertexArray& getGridLineVertices() const;
	void setLiveCells(const vector<sf::Vector2i>& set);
	const vector<sf::Vector2i>& getLiveCells() const;
	bool isCellAlive(const sf::Vector2i& cell) const;
	bool addLiveCell(sf::Vector2i cellCoords);
	void addLiveCells(const vector<sf::Vector2i>& cells);
	bool removeLiveCell(sf::Vector2i cellCoords);
	void convertRelativeCoordsToAbsoluteCoords(sf::Vector2i& coords) const;
	void convertAbsoluteCoordsToRelativeCoords(sf::Vector2i& coords) const;
	void createGrid();
	bool update();
	void clear();
	
	static void convertToLocalCoords(sf::Vector2i& coords, const int& cellSize);
	static void convertToGlobalCoords(sf::Vector2i& coords, const int& cellSize);
	static bool addLiveCellToSet(vector<sf::Vector2i>& liveCellsSet, const sf::Vector2i& cellCoords);
	static bool removeLiveCellFromSet(vector<sf::Vector2i>& liveCellsSet, const sf::Vector2i& cellCoords);
	static bool isCellInTheSet(const vector<sf::Vector2i>& liveCellsSet, const sf::Vector2i& cell);
	static void showLiveCells(const vector<sf::Vector2i>& liveCellsSet);

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void convertToLocalCoords(sf::Vector2i& coords) const;
	void convertToGlobalCoords(sf::Vector2i& coords) const;
	int findTheNumberOfLiveNeighbors(const sf::Vector2i& cellCoords) const;
	
	void convertVerticesCoordsToAbsoluteCoords();




private:

	sf::Vector2u _windowSize;
	int _width;
	int _height;
	int _cellSize;
	int _gridWidth;
	sf::Vector2i _position;
	sf::Color _gridColor;
	sf::Color _liveCellColor;
	
	unique_ptr<sf::VertexArray> _gridLineVertices;
	vector<sf::Vector2i> _liveCells;
	
};

