#include "SpacePartitioning/PartitionGrid.h"
#include <cmath>
namespace vg 
{
	PartitionGrid::PartitionGrid() : PartitionGrid(0, 0, 1.0f) {}
	PartitionGrid::PartitionGrid(std::size_t width, std::size_t height, float cellSize):
		m_gridWidth(width),
		m_gridHeight(height),
		m_cellSize(cellSize),
		m_cells()
	{
		std::size_t gridSize = m_gridWidth * m_gridHeight;
		m_cells.reserve(gridSize);
		for (size_t i = 0; i < gridSize; ++i)
		{
			m_cells.emplace_back(PartitionCell{});
		}
	}

	PartitionCell& PartitionGrid::GetCell(const sf::Vector2f& globalPosition)
	{
		int x = globalPosition.x / m_cellSize;
		int y = globalPosition.y / m_cellSize;

		return GetCell(x, y);
	}

	PartitionCell& PartitionGrid::GetCell(std::size_t x, std::size_t y)
	{
		x = std::clamp(x, std::size_t{ 0 }, m_gridWidth);
		y = std::clamp(y, std::size_t{ 0 }, m_gridHeight);

		return m_cells[m_gridWidth * y + x];
	}

	PartitionCell& PartitionGrid::GetCell(std::size_t i)
	{
		return m_cells[i];
	}

	void PartitionGrid::GetAllCellsContainsRect(const sf::FloatRect& rect, std::vector<std::size_t>& cellsIndices)
	{
		sf::Vector2f cellSize{ m_cellSize, m_cellSize };

		for (size_t i = 0; i < m_cells.size(); ++i)
		{
			int x = i % m_gridWidth;
			int y = i / m_gridWidth;

			sf::Vector2f cellPosition{ x * m_cellSize, y * m_cellSize };
			sf::FloatRect cellRect{ cellPosition, cellSize };

			if (rect.intersects(cellRect)) 
			{
				cellsIndices.push_back(i);
			}
		}
	}

	void PartitionGrid::GetAllCellsAround(std::size_t cellIndex, std::vector<std::size_t>& resultCells)
	{
		int x = cellIndex % m_gridWidth;
		int y = cellIndex / m_gridWidth;

		resultCells.push_back(cellIndex);

		for (size_t i = 0; i < m_directions.size(); ++i)
		{
			int neighbourX = x + m_directions[i].x;
			int neighbourY = y + m_directions[i].y;
			if (IsValid(neighbourX, neighbourY)) 
			{
				resultCells.push_back(m_gridWidth * neighbourY + neighbourX);
			}
		}
	}

	bool PartitionGrid::IsValid(int x, int y) const
	{
		return x >= 0 && x < m_gridWidth && y >= 0 && y < m_gridHeight;
	}

	const std::vector<sf::Vector2i> PartitionGrid::m_directions = std::vector<sf::Vector2i>
	{ 
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1},
		{-1, 0},
		{-1, -1},
		{0, -1},
		{1, -1}
	};
}