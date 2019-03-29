#pragma once

#ifndef __VBVECTOR__H
#define __VBVECTOR__H

#include "Log.h"


using namespace std;

template <class T>
class VBVector {
public:
	typedef typename vector<T>::iterator iterator;
	VBVector() { vec.clear();
	}
	virtual ~VBVector(){
		vec.~vector();
	}
	T& operator[](int idx) 
	{
		if (vec.size() == 0) {
			T value = T();
			VBExceptionLog("WARNING: VesselBuilder1: VBVector Exception! called a vector position when size == 0 (idx:%i)",idx);
			oapiWriteLogV("WARNING: VesselBuilder1: VBVector Exception! called a vector position when size == 0 (idx:%i)",idx);
			return value;
		}
		if (idx >= (int)vec.size()) {
			VBExceptionLog("WARNING: VesselBuilder1: VBVector Exception! called a vec position greater than vector size! (idx:%i size:%i)", idx, vec.size());
			oapiWriteLogV("WARNING: VesselBuilder1: VBVector Exception! called a vec position greater than vector size! (idx:%i size:%i)", idx, vec.size());
			return vec.back();
		}
		else {
			if (idx < 0) {
				oapiWriteLogV("WARNING: VesselBuilder1: VBVector Exception! called a vec position <0! (idx:%i size:%i)", idx, vec.size());
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
	typename vector<T>::iterator begin() {
		return vec.begin();
	}
	typename vector<T>::iterator end() {
		return vec.end();
	}
	VBVector<T>& operator=(const VBVector& x) {
		vector<T> v1 = x.getvec();
		this->setvec(v1);
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
	iterator erase(unsigned int position) {
		if (position >= vec.size()) {
			oapiWriteLogV("WARNING: VesselBuilder1: VBVector Exception! called erase on a vec position greater than vector size! (idx:%i size:%i)", position, vec.size());
			return 0;
		}
		return vec.erase(vec.begin()+position);
	}
	iterator erase(iterator pos) {
	//	if (pos > vec.end()) { return 0; }
		return vec.erase(pos);		
	}
	void insert(iterator pos, T val) {
		vec.insert(pos, val);
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
	vector<T> getvec() const {
		return vec;
	}
	void setvec(vector<T> v) {
		vec.clear();
		vec = v;
		return;
	}
	bool empty() {
		return vec.size() == 0 ? true : false;
	}
private:
	vector<T>vec;
//	T dummy0 = 0;

};

#endif // !__VBVECTOR__H