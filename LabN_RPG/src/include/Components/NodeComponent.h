#pragma once
#include "vgpch.h"
namespace vg 
{
	struct NodeComponent 
	{
		entt::entity Parent{ entt::null };
		std::vector<entt::entity> Children{};
	};
}