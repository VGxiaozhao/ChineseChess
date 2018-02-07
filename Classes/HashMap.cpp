#include "HashMap.h"
#include <cstring>
#include <math.h>

CHashMap::CHashMap(int size)
{
	_key = new unsigned long long[size + 7];
	_val = new int[size + 7];
	_nxt = new int[size + 1];
	_head = new int[size + 1];
	_size = size;
	for (hashit = size;!isPrime(hashit); hashit--);
	init();
}

bool CHashMap::isPrime(int x)
{
	for (int i = 2; i <= sqrt(x*1.0) + 1; i++)
	{
		if (x%i == 0) return false;
	}
	return true;
}

void CHashMap::init()
{
	for (int i = 0; i <= _size; i++)
		_head[i] = -1;
	tol = 0;
}

void CHashMap::clear()
{
	init();
}

int CHashMap::hash(unsigned long long x)
{
	return x%hashit;
}

bool CHashMap::find(unsigned long long key, int& val)
{
	int u = hash(key);
	for (int i = _head[u]; i != -1; i = _nxt[i])
	{
		if (_key[i] == key)
		{
			val = _val[i];
			return true;
		}
	}
	return false;
}

void CHashMap::add(unsigned long long key, int val)
{
	int u = hash(key);
	for (int i = _head[u]; i != -1; i = _nxt[i])
	{
		if (_key[i] == key)
		{
			_val[i] = val; 
			return;
		}
	}
	_key[tol] = key;
	_val[tol] = val;
	_nxt[tol] = _head[u];
	_head[u] = tol++;
}

CHashMap::~CHashMap()
{
	delete[] _nxt;
	delete[] _head;
	delete[] _key;
	delete[] _val;
}
