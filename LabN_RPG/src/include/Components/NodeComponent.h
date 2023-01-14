#pragma once
#include "entt/entt.hpp"
#include <optional>
#include <vector>
namespace vg 
{
	struct NodeComponent 
	{
		std::optional<entt::entity> Parent{};
		std::vector<entt::entity> Children{};
	};
}