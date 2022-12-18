#pragma once
#include "ResourceManagement/AnimationProvider.h"
namespace vg 
{
	struct AnimationComponent 
	{
		AnimationPack* CurrentAnimationPack;
		Animation* CurrentAnimation;
		float CurrentFrame;
		int CurrentIndex;
	};
}