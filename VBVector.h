#pragma once

#ifndef __VBVECTOR__H
#define __VBVECTOR__H

#include "Log.h"
#include <typeinfo>
#include <iostream>


using namespace std;

extern HWND _orbiter_window;



template <class T>
class VBVector {
public:
	typedef typename vector<T>::iterator iterator;
	VBVector() { vec.clear();
	}
	VBVector(vector<T>v) {
		vec = v;
		return;
	}
	virtual ~VBVector(){
		vec.~vector();
	}
	T& operator[](int idx) 
	{
//#ifndef DEBUG
		if (vec.size() == 0) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "WARNING: VesselBuilder1: VBVector Exception! called a vector position when size == 0 (type:%s idx:%i ver:%i)", typeid(T).name(), idx,VBVERSION);
			VBExceptionLog(cbuf);
			MessageBox(_orbiter_window, cbuf, "WARNING!", MB_OK);
			return value;
		}
//#endif // !DEBUG

		
		if (idx >= (int)vec.size()) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "WARNING: VesselBuilder1: VBVector Exception! called a vec position greater than vector size! (type: %s idx:%i size:%i ver:%i)", typeid(T).name(), idx, vec.size(),VBVERSION);
			VBExceptionLog(cbuf);
			MessageBox(_orbiter_window, cbuf, "WARNING!", MB_OK);
			return vec.back();
		}
		else {
			if (idx < 0) {
				char cbuf[256] = { '\0'};
				sprintf(cbuf, "WARNING: VesselBuilder1: VBVector Exception! called a vec position <0! (type: %s idx:%i size:%i ver:%i)", typeid(T).name(), idx, vec.size(),VBVERSION);
				VBExceptionLog(cbuf);
				MessageBox(_orbiter_window, cbuf, "WARNING!", MB_OK);
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
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "WARNING: VesselBuilder1: VBVector Exception! called erase on a vec position greater than vector size! (type: %s name:%s idx:%i size:%i ver:%i)", typeid(T).name(), position, vec.size(),VBVERSION);
			VBExceptionLog(cbuf);
			MessageBox(_orbiter_window, cbuf, "WARNING!", MB_OK);
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
		return vec.size() > 0 ? false : true;
	}
private:
	vector<T>vec;
	T value = T();
//	T dummy0 = 0;

};

#endif // !__VBVECTOR__H