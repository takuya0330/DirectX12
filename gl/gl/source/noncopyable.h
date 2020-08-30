#pragma once

class noncopyable
{
protected:
	noncopyable() = default;
	~noncopyable() = default;

private:
	void operator =(const noncopyable&) {}
	noncopyable(const noncopyable& _src) {}
};