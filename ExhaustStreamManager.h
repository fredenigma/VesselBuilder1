#pragma once
struct EXHAUST_STREAM_DEF {
	PSTREAM_HANDLE psh;
	UINT th_index;
	UINT part_index;
	VECTOR3 pos;
	EXHAUST_STREAM_DEF() {
		psh = NULL;
		th_index = (UINT)-1;
		part_index = (UINT)-1;
		pos = _V(0, 0, 0);
	}
};

class ExhaustStreamManager {
public:
	ExhaustStreamManager(VesselBuilder1* _VB1);
	~ExhaustStreamManager();
	VesselBuilder1* VB1;
	vector<EXHAUST_STREAM_DEF> exst_def;
	UINT AddUndefinedExStDef();
	UINT AddExStDef(UINT th_index, VECTOR3 pos, UINT part_index);
	void DelExStDef(def_idx d_idx);
	void GetExStParams(def_idx d_idx, UINT &th_index, UINT &part_index);
	UINT GetExStCount();

};