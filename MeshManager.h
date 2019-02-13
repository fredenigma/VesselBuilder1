#pragma once

struct MSH_DEF {
	string meshname;
	VECTOR3 ofs;
	MATRIX3 rm;
	msh_idx msh_idx;
	MESHHANDLE msh_h;
	MESHHANDLE template_msh_h;
	DEVMESHHANDLE devmsh_h;
	VECTOR3 dir;
	VECTOR3 rot;
	UINT ngrps;
	//vector<UINT>animated_grps;
	bool Highlighted;
	//vector<UINT>HighlightedGrps;
	bool *HighlightedGrps;
	MSH_DEF() {
		meshname.clear();
		ofs = _V(0, 0, 0);
		rm = _M(1, 0, 0, 0, 1, 0, 0, 0, 1);
		msh_idx = -1;// 1000000;
		msh_h = NULL;
		template_msh_h = NULL;
		devmsh_h = NULL;
		dir = _V(0, 0, 1);
		rot = _V(0, 1, 0);
		Highlighted = false;
		ngrps = 0;
		HighlightedGrps = false;
		//	animated_grps.clear();
		//HighlightedGrps.clear();
	}
};

class MeshManager {
public:
	MeshManager(StationBuilder1 *_SB1);
	~MeshManager();
	vector<MSH_DEF> msh_defs;
	StationBuilder1 *SB1;
	void LoadMeshes();
	UINT loading_msh_counter;
	void RotateMeshLoading(MESHHANDLE mesh, MATRIX3 rm);
	void RotateMesh(MESHHANDLE mesh, DEVMESHHANDLE devmesh, MATRIX3 rm);
	void RotateMesh(msh_idx msh_idx, MATRIX3 rm);
	void RotateMeshFromTemplate(msh_idx msh_idx, MATRIX3 rm);
	bool HighlightMesh(msh_idx msh_idx, bool Highlight);
	void ResetHighlights();
	bool HighlightMeshGroup(msh_idx msh_idx, UINT grp_idx, bool Highlight);
	def_idx GetMeshDefIndexfromMsh_idx(msh_idx idx);
	UINT HighLightColor;
	void SetHighLightColor(UINT coloroption);
	UINT GetHighLightColor();
	bool DeleteMeshDef(def_idx idx);
	void AddMeshDef();
	void AddMeshDef(MSH_DEF md);
	void AddMeshDef(string meshname, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot);
	UINT nMeshes;
	bool ChangeMeshFile(def_idx idx, string newmeshname);
	void VisualCreated(VISHANDLE vis, int refcount);
	void VisualDestroyed(VISHANDLE vis, int refcount);
	UINT GetMeshCount();
	void WriteCfg(FILEHANDLE fh);
	vector<MSH_DEF> GetAllDefs();
	string GetMeshName(def_idx def_idx);
	msh_idx IdxDef2Msh(def_idx def_idx);
	def_idx IdxMsh2Def(msh_idx msh_idx);
	MATRIX3 GetMeshDefRM(def_idx def_idx);
	void SetMeshDefRM(def_idx def_idx, MATRIX3 rm);
	MESHHANDLE GetMeshDefMH(def_idx def_idx);
	DEVMESHHANDLE GetMeshDefDMH(def_idx def_idx);
	string GetMeshDefName(def_idx def_idx);
	VECTOR3 GetMeshDefPos(def_idx def_idx);
	VECTOR3 GetMeshDefDir(def_idx def_idx);
	VECTOR3 GetMEshDefRot(def_idx def_idx);
	void MoveMeshDef(def_idx def_idx, VECTOR3 delta_pos);
	void SetMeshDefDirRot(def_idx def_idx, VECTOR3 dir, VECTOR3 rot);
	UINT GetMeshDefNGrps(def_idx def_idx);
	vector<MSH_DEF> md_restore;
	bool UsingD3D9();
	void LoadingRearrangeMaterials(MESHHANDLE msh);
	MATERIAL *matred;
	MATERIAL *matgreen;
	MATERIAL *matblue;
	MATERIAL *defaultMaterial;
	bool MeshGroupIsHighlighted(msh_idx msh_idx, UINT grp_idx);
	void ResetMeshGroupHighlights(msh_idx msh_idx);

};