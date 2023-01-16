#pragma once
#include <cmath>
#include "SFML/Graphics.hpp"

namespace vg 
{
	namespace VGMath
	{
		static const float PI = 3.1415f;

		static const sf::Vector2<float> Right = sf::Vector2<float>{ 1.0f, 0.0f };
		static const sf::Vector2<float> Left = sf::Vector2<float>{ -1.0f, 0.0f };
		static const sf::Vector2<float> Up = sf::Vector2<float>{ 0.0f, 1.0f };
		static const sf::Vector2<float> Down = sf::Vector2<float>{ 0.0f, -1.0f };
		static const sf::Vector2<float> Zero = sf::Vector2<float>{ 0.0f, 0.0f };
		static const sf::Vector2<float> One = sf::Vector2<float>{ 1.0f, 1.0f };

		template<typename T>
		float GetLengthOfVector2(const sf::Vector2<T>& vector)
		{
			float x = (float)vector.x;
			float y = (float)vector.y;
			return std::sqrt(x * x + y * y);
		}

		template<typename T>
		float GetSqrtLengthOfVector2(const sf::Vector2<T>& vector)
		{
			float x = (float)vector.x;
			float y = (float)vector.y;
			return x * x + y * y;
		}

		template<typename T>
		sf::Vector2<T> NormalizeVector2(const sf::Vector2<T>& vector)
		{
			float length = GetLengthOfVector2(vector);
			if (length <= 0.0f)
			{
				return sf::Vector2<T>{};
			}
			length = std::sqrt(1.0f / length);
			T x = (T)(vector.x * length);
			T y = (T)(vector.y * length);
			return sf::Vector2<T>{x, y};
		}

		template<typename T, typename V>
		static bool Intersects2D(const sf::Rect<T>& rect1, const sf::Rect<V>& rect2)
		{
			static_assert(std::is_arithmetic_v<T> && std::is_arithmetic_v<V>);

			sf::Vector2<T> rect1Min = sf::Vector2<T>{ rect1.left, rect1.top };
			sf::Vector2<V> rect2Min = sf::Vector2<V>{ rect2.left, rect2.top };
			sf::Vector2<T> rect1Max = rect1Min + sf::Vector2<T>{ rect1.width, rect1.height };
			sf::Vector2<V> rect2Max = rect2Min + sf::Vector2<V>{ rect2.width, rect2.height };

			return rect1Min.x <= rect2Max.x &&
				rect1Max.x >= rect2Min.x &&
				rect1Min.y <= rect2Max.y &&
				rect1Max.y >= rect2Min.y;
		}

		template<typename T>
		static sf::Rect<T> TwoVectorsToRect(const sf::Vector2<T>& leftTop, const sf::Vector2<T>& rightBottom)
		{
			static_assert(std::is_arithmetic_v<T>);

			sf::Vector2<T> size = sf::Vector2<T>{ rightBottom.x - leftTop.x, rightBottom.y - leftTop.y };
			return sf::Rect<T>{leftTop, size};
		}

		template<typename L, typename R>
		float Dot(const sf::Vector2<L>& lhs, const sf::Vector2<R>& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}

		template<typename L, typename R>
		float Dot(const sf::Vector3<L>& lhs, const sf::Vector3<R>& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}

		template <typename T>
		float operator*(const sf::Vector2<T>& vec1, const sf::Vector2<T>& vec2)
		{
			return vec1.x * vec2.x + vec1.y * vec2.y;
		}

		template <typename T>
		float RadToDeg(T radians)
		{
			return radians * 180.0f / PI;
		}

		template <typename T>
		float DegToRad(T degree)
		{
			return degree / 180.0f * PI;
		}
	}
}

