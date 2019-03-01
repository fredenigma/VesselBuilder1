#pragma once




class AnimCompDef {
public:
	AnimCompDef();
	~AnimCompDef();
	double state0;
	double state1;
	string name;
//	string prefix;
//	string fullname;
	vector<UINT> grps;
	int ngrps;
	int mesh;
	UINT *pgrps;
	VECTOR3 ref;
	VECTOR3 shift;
	VECTOR3 scale;
	VECTOR3 axis;
	double angle;
	ANIMATIONCOMPONENT_HANDLE ach;
	ANIMATIONCOMPONENT_HANDLE parent_ach;
	//UINT parent_comp_idx;
	UINT anim_idx;
	//UINT seq_idx;
	//UINT comp_idx;
	MGROUP_TRANSFORM *mgt;
	bool valid;
	void Init(MGROUP_TRANSFORM::TYPE type);
	void Init(MGROUP_TRANSFORM::TYPE type, double state0, double state1, string name, vector<UINT>grps, UINT mesh, VECTOR3 ref, VECTOR3 shift, VECTOR3 scale, VECTOR3 axis, double angle, UINT parent_comp_idx);
	void SetState0(double newstate0);
	void SetState1(double newstate1);
	void SetRef(VECTOR3 newRef);
	void SetScale(VECTOR3 newScale);
	void SetShift(VECTOR3 newshift);
	void SetAxis(VECTOR3 newAxis);
	void SetAngle(double newAngle);
	void SetName(string newname);
	//void SetPrefix(string newprefix);
	void SetMesh(UINT msh_idx);
	void AddGroups(vector<UINT> addgrps);
	void RemoveGroups(vector<UINT>remgrps);
	void ResetGroups();
	void RemakeGroups();
	string GetName();
//	string GetFullName();
	//string GetPrefix();
	double GetState0();
	double GetState1();
	void GetStates(double &state0, double &state1);
	VECTOR3 GetRef();
	VECTOR3 GetAxis();
	VECTOR3 GetShift();
	VECTOR3 GetScale();
	double GetAngle();
	void GetRotParams(VECTOR3 &ref, VECTOR3 &axis, double &angle);
	void GetScaleParams(VECTOR3 &ref, VECTOR3 &scale);
	UINT GetMesh();
	vector<UINT> GetGroups();
	int GetNgrps();
	void AssignToAnimation(UINT anim_idx,VESSEL *v);
	//void SetParent(ANIMATIONCOMPONENT_HANDLE _parent);
	void SetParent(AnimCompDef* parent_acd);
	/*void SetCompIdx(UINT _comp_idx);
	void SetSeqIdx(UINT _seq_idx);
	void SetParentCompIdx(UINT _parent_comp_idx);
	UINT GetCompIdx();
	UINT GetSeqIdx();
	UINT GetParentCompIdx();*/
	
	ANIMATIONCOMPONENT_HANDLE GetParentACH();
	MGROUP_TRANSFORM::TYPE GetType();
	ANIMATIONCOMPONENT_HANDLE GetACH();
	bool IsValid();
	void Validate();
	void Invalidate();
	void Reset();
	AnimCompDef* ParentACD;
	vector<AnimCompDef*> ChildrenACD;
	bool ArmTip;
	VECTOR3 oldTip[3];
	VECTOR3 Tip[3];
	def_idx TipAttIdx;
	bool IsArmTip();
	void GetTips(VECTOR3 &v1, VECTOR3 &v2, VECTOR3 &v3);
	void SetTips(VECTOR3 v1, VECTOR3 v2, VECTOR3 v3);
	void SetAttTip(def_idx AttIdx);
	def_idx GetAttTip();
	void SetIsArmTip(bool set);


};
