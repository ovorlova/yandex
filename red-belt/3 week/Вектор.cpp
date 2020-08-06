#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
	T* Allocate() {
		if (!pool_free.empty()) {
			auto p = pool_free.front();
			pool_free.pop();
			pool_highlighted.insert(p);
			return p;
		}
		else {
			auto p = new T;
			pool_highlighted.insert(p);
			return p;
		}
	}
	T* TryAllocate() {
		if (!pool_free.empty()) {
			auto p = pool_free.front();
			pool_free.pop();
			pool_highlighted.insert(p);
			return p;
		}
		else {
			return nullptr;
		}
	}

	void Deallocate(T* object) {
		auto it = pool_highlighted.find(object);
		if (it == pool_highlighted.end()) {
			throw invalid_argument("no object");
		}
		else {
			pool_highlighted.erase(it);
		}
		pool_free.push(object);
	}

	~ObjectPool() {
		while (!pool_free.empty()) {
			delete pool_free.front();
			pool_free.pop();
		}
		for (auto& p : pool_highlighted) {
			delete p;
		}
	}

private:
	queue<T*> pool_free;
	set<T*> pool_highlighted;

};

void TestObjectPool() {
	ObjectPool<string> pool;

	auto p1 = pool.Allocate();
	auto p2 = pool.Allocate();
	auto p3 = pool.Allocate();

	*p1 = "first";
	*p2 = "second";
	*p3 = "third";

	pool.Deallocate(p2);
	ASSERT_EQUAL(*pool.Allocate(), "second");

	pool.Deallocate(p3);
	pool.Deallocate(p1);
	ASSERT_EQUAL(*pool.Allocate(), "third");
	ASSERT_EQUAL(*pool.Allocate(), "first");

	pool.Deallocate(p1);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestObjectPool);
	return 0;
}