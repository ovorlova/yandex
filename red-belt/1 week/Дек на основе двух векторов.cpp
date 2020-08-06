#include <vector>
#include <iostream>

using namespace std;

template <typename T>
class Deque {
public:
	Deque() = default;
	bool Empty() const {
		return deque_front.size() == 0 && deque_back.size() == 0;
	}
	size_t Size() const {
		return deque_front.size() + deque_back.size();
	}

	T& operator[](size_t index) {
		if (index >= 0 && index < deque_front.size())
			return deque_front[deque_front.size() - 1 - index];
		else
			return deque_back[index - deque_front.size()];
	}
	const T& operator[](size_t index) const {
		if (index >= 0 && index < deque_front.size())
			return deque_front[deque_front.size() - 1 - index];
		else
			return deque_back[index - deque_front.size()];
	}
	T& At(size_t index) {
			if (index < 0 || index >= deque_front.size() + deque_back.size()) {
				throw out_of_range("Index out of range!");
			}
			else {
				if (index >= 0 && index < deque_front.size())
					return deque_front[deque_front.size() - 1 - index];
				else
					return deque_back[index - deque_front.size()];
			}
		
	}

	const T& At(size_t index) const {

			if (index < 0 || index >= deque_front.size() + deque_back.size()) {
				throw out_of_range("Index out of range!");
			}
			else {
				if (index >= 0 && index < deque_front.size())
					return deque_front[deque_front.size() - 1 - index];
				else
					return deque_back[index - deque_front.size()];

			}
		
	}
	T& Front() {
		if (!deque_front.empty())
			return deque_front[deque_front.size() - 1];
		else
			return deque_back[0];
	}
	const T& Front() const {
		if (!deque_front.empty())
			return deque_front[deque_front.size() - 1];
		else
			return deque_back[0];
	}
	T& Back() {
		if (!deque_back.empty())
			return deque_back[deque_back.size() - 1];
		else
			return deque_front[0];
	}
	const T& Back() const {
		if (!deque_back.empty())
			return deque_back[deque_back.size() - 1];
		else
			return deque_front[0];
	}
	void PushFront(T element) {
		deque_front.push_back(element);
	}
	void PushBack(T element) {
		deque_back.push_back(element);
	}
private:
	vector<T> deque_front;
	vector<T> deque_back;
};
