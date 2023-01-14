#pragma once
#include "entt/entt.hpp"
#include <optional>
#include <vector>
namespace vg 
{
	struct NodeComponent 
	{
		entt::entity Parent{ entt::null };
		std::vector<entt::entity> Children{};
	};
}