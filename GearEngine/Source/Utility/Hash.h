#ifndef HASH_H
#define HASH_H
#include <functional>
/**
  std::hash����c++14�ı�׼,��������Ϊ�˼���c++11�޸�hash����
*/

template <class T>
size_t Hash(const T& v)
{
	std::hash<T> hasher;
	return hasher(v);
}

template <class T>
void Hash(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
#endif
