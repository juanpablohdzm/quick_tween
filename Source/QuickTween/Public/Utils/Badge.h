#pragma once

template <typename T>
class Badge
{
private:
	friend T;
	Badge() = default;
};