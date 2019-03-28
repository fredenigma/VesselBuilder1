#pragma once

#ifndef __VBVECTOR__H
#define __VBVECTOR__H



using namespace std;

template <class T>
class VBVector {
public:
	typedef T* iterator;
	VBVector() { vec.clear(); }
	virtual ~VBVector(){
		vec.~vector();
	}
	T& operator[](int idx) 
	{
		if (vec.size() == 0) {
			return dummy0;
		}
		if (idx >= (int)vec.size()) {
			return vec.back();
		}
		else {
			if (idx < 0) {
				return vec[0];
			}
			else {
				return vec[idx];
			}
		}
	}
	void push_back(T t) {
		vec.push_back(t);
	}
	T& back() {
		return vec.back();
	}
	T& front() {
		return vec.front();
	}
	iterator begin() {
		return vector<T>::vec.begin();
	}
	iterator end() {
		return vec.end();
	}
	VBVector<T>& operator=(const VBVector& x) {
		return (*this);
	}
	VBVector<T>& operator=(const vector<T> x) {
		vec = x;
	}
	size_t size() {
		return vec.size();
	}
	void clear() {
		vec.clear();
	}
	void erase(unsigned int position) {
		vec.erase(vec.begin()+position);
		return;
	}
	void erase(iterator pos) {
		vec.erase(pos);
		return;
	}
	/*void erase(iterator first, iterator last) {
		vec.erase(first, last);
		return;
	}*/
	bool VBFind(T t, int &idx) {
		for (UINT i = 0; i < vec.size(); i++) {
			if (vec[i] == t) {
				idx = i;
				return true;
			}
		}
		return false;
	}
private:
	vector<T>vec;
	T dummy0 = 0;

};

#endif // !__VBVECTOR__H