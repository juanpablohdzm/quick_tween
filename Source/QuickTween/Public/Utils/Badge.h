#pragma once

template <typename T>
class Badge<T>
{
	friend T;
	Badge() = default;
};