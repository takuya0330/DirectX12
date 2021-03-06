#pragma once
#include <functional>

template<typename T>
struct Property
{
	//Property() = default;
	//Property(const Property& other) {}

	Property& operator=(const Property& value)
	{
		set(value.get());
		return *this;
	}

	void operator()(const std::function<T()>& getter, const std::function<void(T value)>& setter)
	{
		if (!get) get = getter;
		if (!set) set = setter;
	}

	T operator()()const { return get(); }
	void operator()(const T value) { set(value); }

	operator T() { return get(); }
	void operator=(const T& value) { set(value); }

	std::function<T()> get = nullptr;
	std::function<void(const T&)> set = nullptr;
};