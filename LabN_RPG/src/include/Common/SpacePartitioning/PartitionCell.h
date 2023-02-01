#pragma once
#include "vgpch.h"
namespace vg 
{
	class PartitionCell
	{
	public:
		PartitionCell();
		const std::vector<entt::entity>& GetEntities() const { return m_entities; }
		void AddEntity(entt::entity entity);
		void RemoveEntity(entt::entity entity);
		bool Contains(entt::entity entity);
	private:
		std::vector<entt::entity> m_entities;
	};
}