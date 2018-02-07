#pragma once

class CHashMap
{
public:
	CHashMap(int size);
	~CHashMap();
public:
	void clear();
	void init();
	void add(unsigned long long key, int value);
	bool find(unsigned long long key, int& );
private:
	int hash(unsigned long long key);
	bool isPrime(int x);
private:
	int hashit;
	int* _nxt;
	int* _head;
	unsigned long long* _key;
	int* _val;
	int _size;
	int tol;
};

