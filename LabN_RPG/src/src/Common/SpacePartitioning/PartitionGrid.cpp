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
		m_cells.resize(gridSize);
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

	bool PartitionGrid::IsValid(int x, int y) const
	{
		return x > 0 && x <= m_gridWidth && y > 0 && y <= m_gridHeight;
	}
}