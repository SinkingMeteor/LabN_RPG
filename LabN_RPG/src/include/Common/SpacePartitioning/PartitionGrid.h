#pragma once
#include <vector>
#include <memory>
#include "SFML/Graphics.hpp"
#include "SpacePartitioning/PartitionCell.h"
#include "entt/entt.hpp"
namespace vg 
{
	class PartitionGrid
	{
	public:
		PartitionGrid();
		PartitionGrid(std::size_t width, std::size_t height, float cellSize);
		PartitionCell& GetCell(const sf::Vector2f& globalPosition);
		PartitionCell& GetCell(std::size_t x, std::size_t y);
		PartitionCell& GetCell(std::size_t i);
		void GetAllCellsContainsRect(const sf::FloatRect& rect, std::vector<std::size_t>& cellsIndices);
		void GetAllCellsAround(std::size_t cellIndex, std::vector<std::size_t>& resultCells);
		float GetCellSize() const { return m_cellSize; }
		sf::Vector2<std::size_t> GetGridSize() const { return sf::Vector2<std::size_t>{ m_gridWidth, m_gridHeight }; }
		const std::vector<PartitionCell>& GetCellArray() const { return m_cells; }
	private:
		bool IsValid(int x, int y) const;

		std::size_t m_gridWidth;
		std::size_t m_gridHeight;
		float m_cellSize;
		std::vector<PartitionCell> m_cells;

		const static std::vector<sf::Vector2i> m_directions;
	};
}