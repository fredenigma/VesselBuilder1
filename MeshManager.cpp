#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "MeshManager.h"
#include "gcAPI.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)


bool WD3D9 = false;// MM->UsingD3D9();
MATRIX3 g_RM = _M(1, 0, 0, 0, 1, 0, 0, 0, 1);

void RotateMeshLoading(MESHHANDLE mesh, MATRIX3 rm) {
	int k = oapiMeshGroupCount(mesh);
	if (k <= 0) { return; }
	

	for (int i = 0; i<k; i++) {
		GROUPEDITSPEC grpspc;
		grpspc.flags = GRPEDIT_VTXCRD | GRPEDIT_VTXNML;

		MESHGROUP *m = oapiMeshGroup(mesh, i);
		DWORD l = m->nVtx;
		grpspc.nVtx = l;
		grpspc.Vtx = new NTVERTEX[grpspc.nVtx];
		grpspc.vIdx = new WORD[grpspc.nVtx];
		for (DWORD j = 0; j<l; j++)
		{
			VECTOR3 p = _V(m->Vtx[j].x, m->Vtx[j].y, m->Vtx[j].z);
			VECTOR3 n = _V(m->Vtx[j].nx, m->Vtx[j].ny, m->Vtx[j].nz);
			p = mul(rm, (p));
			n = mul(rm, (n));

			grpspc.Vtx[j].x = p.x;
			grpspc.Vtx[j].y = p.y;
			grpspc.Vtx[j].z = p.z;
			grpspc.Vtx[j].nx = n.x;
			grpspc.Vtx[j].ny = n.y;
			grpspc.Vtx[j].nz = n.z;
			grpspc.vIdx[j] = j;

		}
		oapiEditMeshGroup(mesh, i, &grpspc);
		delete[] grpspc.Vtx;
		delete[] grpspc.vIdx;
	}

}
void LoadingRearrangeMaterials(MESHHANDLE msh) {
	//oapiWriteLogV("D3D9 %i", WD3D9);
	if (WD3D9) { //If D3D9
		MATERIAL *defaultMaterial = new MATERIAL;
		COLOUR4 colwhite = { 1,1,1,1 };
		COLOUR4 colblack = { 0,0,0,1 };
		defaultMaterial->diffuse = colwhite;
		defaultMaterial->ambient = colblack;
		defaultMaterial->emissive = colblack;
		defaultMaterial->power = 0;
		defaultMaterial->specular = colblack;
		DWORD groups_count = oapiMeshGroupCount(msh);
		DWORD initial_material_count = oapiMeshMaterialCount(msh);  //Get Initial Material Count to be used afterwards for cleanup

		for (DWORD i = 0; i < groups_count; i++) {		//for every group			
			MESHGROUP *mg = oapiMeshGroup(msh, i);		// get the group
			int matidx = mg->MtrlIdx;					// get its original material index;
			MATERIAL *mat;
			if (matidx <= 0) {
				mat = defaultMaterial;
			}
			else {
				mat = oapiMeshMaterial(msh, matidx);		//get the group material
			}

			DWORD newmatidx = oapiAddMaterial(msh, mat);		//add a copy of the material at the bootm
			mg->MtrlIdx = newmatidx;							//set the group material to the copy just added
			
		}
		delete defaultMaterial;
		for (DWORD i = 1; i < initial_material_count; i++) {	//delete all the initial materials since now all the groups point to the copies
																//	oapiDeleteMaterial(msh, i);
		}
	}
	else {	//if NOT D3D9
		COLOUR4 colred = { 1,0,0,1 };	//Red Color
		COLOUR4 colgreen = { 0,1,0,1 };	//Green Color
		COLOUR4 colblue = { 0,0,1,1 };	//Blue Color
		MATERIAL *matred = new MATERIAL;			//Create the tree new materials 
		matred->ambient = colred;
		matred->diffuse = colred;
		matred->emissive = colred;
		matred->specular = colred;
		matred->power = 0;
		MATERIAL *matgreen = new MATERIAL;
		matgreen->ambient = colgreen;
		matgreen->diffuse = colgreen;
		matgreen->emissive = colgreen;
		matgreen->specular = colgreen;
		matgreen->power = 0;
		MATERIAL *matblue = new MATERIAL;
		matblue->ambient = colblue;
		matblue->diffuse = colblue;
		matblue->emissive = colblue;
		matblue->specular = colblue;
		matblue->power = 0;

		DWORD green_idx = oapiAddMaterial(msh, matgreen);	//Add the new materials to the mesh
		DWORD blue_idx = oapiAddMaterial(msh, matblue);
		DWORD red_idx = oapiAddMaterial(msh, matred);
		delete matred;									//cleanup
		delete matgreen;
		delete matblue;
	}

	return;
}
void LoadMeshClbk(MESHHANDLE msh_h, bool firstload) {
	WD3D9 = gcEnabled();

	if (firstload) {
		RotateMeshLoading(msh_h, g_RM);
		LoadingRearrangeMaterials(msh_h);
	}
	return;
}







MeshManager::MeshManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	msh_defs.clear();
	//loading_msh_counter = 0;
	//MM = this;
	HighLightColor = HIGHLIGHTCOLORRED;
	md_restore.clear();
	nMeshes = 0;
	//MM = this;
//	WD3D9 = UsingD3D9();
	return;
}
MeshManager::~MeshManager(){
	for (UINT i = 0; i < msh_defs.size(); i++) {
		if (msh_defs[i].HighlightedGrps) {
			delete[] msh_defs[i].HighlightedGrps;
			msh_defs[i].HighlightedGrps = NULL;
		}
	}
	VB1 = NULL;
//	MM = NULL;
	
}
void MeshManager::AddMeshDef() {
	MSH_DEF md = MSH_DEF();
	return AddMeshDef(md);
}
void MeshManager::AddMeshDef(MSH_DEF md) {
	md.rm = VB1->FindRM(md.dir, md.rot);
	msh_defs.push_back(md);
	nMeshes = msh_defs.size();
	return;
}
void MeshManager::AddMeshDef(string meshname, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot,WORD visibility) {
	LogV("Adding Mesh Def:%s pos:%.3f %.3f %.3f dir:%.3f %.3f %.3f rot:%.3f %.3f %.3f Visibility:%i", meshname.c_str(), pos.x, pos.y, pos.z, dir.x, dir.y, dir.z, rot.x, rot.y, rot.z, visibility);
	MSH_DEF md = MSH_DEF();
	md.meshname = meshname;
	md.ofs = pos;
	md.dir = dir;
	md.rot = rot;
	md.visibility = visibility;
	return AddMeshDef(md);
}


void MeshManager::LoadMeshes() {
	LogV("Load Meshes Started");
//	SBLog("Loading of %i Meshes...",nMeshes);
	if (msh_defs.size() <= 0) {
		LogV("WARNING: No Mesh Definitions found");
//		SBLog("WARNING: No Mesh Definitions");
		return;
	}
	
	//for (UINT i = 0; i < msh_defs.size(); i++) {
	//	oapiWriteLogV("preloading:%s", msh_defs[i].meshname.c_str());
	//	msh_defs[i].template_msh_h = oapiLoadMeshGlobal(msh_defs[i].meshname.c_str(),LoadMeshClbk);
	//}
	for (UINT i = 0; i < msh_defs.size(); i++) {
		//loading_msh_counter = i;
		g_RM = msh_defs[i].rm;
		msh_defs[i].msh_h = oapiLoadMeshGlobal(msh_defs[i].meshname.c_str(), LoadMeshClbk);
		msh_defs[i].ngrps = oapiMeshGroupCount(msh_defs[i].msh_h);
		msh_defs[i].msh_idx = VB1->AddMesh(msh_defs[i].msh_h, &msh_defs[i].ofs);
		VB1->SetMeshVisibilityMode(msh_defs[i].msh_idx, msh_defs[i].visibility);
		msh_defs[i].template_msh_h = VB1->CopyMeshFromTemplate(msh_defs[i].msh_idx);
		if (msh_defs[i].ngrps > 0) {
			msh_defs[i].HighlightedGrps = new bool[msh_defs[i].ngrps];
			for (int j = 0; j < msh_defs[i].ngrps; j++) {
				msh_defs[i].HighlightedGrps[j] = false;
			}
		}
		
		if (msh_defs[i].msh_h != NULL) {
			LogV("Loaded Mesh : %s @:%.3f %.3f %.3f", msh_defs[i].meshname.c_str(), msh_defs[i].ofs.x, msh_defs[i].ofs.y, msh_defs[i].ofs.z);
	//		SBLog("Loaded Mesh: %s @:%.3f %.3f %.3f", msh_defs[i].meshname.c_str(), msh_defs[i].ofs.x, msh_defs[i].ofs.y, msh_defs[i].ofs.z);
		}
		else {
			LogV("WARNING: Unable to load mesh %s", msh_defs[i].meshname.c_str());
		//	SBLog("WARNING: Unable to load mesh %s", msh_defs[i].meshname.c_str());
		}
	}
	LogV("Loading of %i Meshes Completed", msh_defs.size());
	//SBLog("Loading of %i Meshes Complete", msh_defs.size());
	return;
}

void MeshManager::RotateMeshFromTemplate(msh_idx msh_idx, MATRIX3 rm) { //CORRETTO MA CON IL CLBKLOADFUNCTION IL TEMPLATE E RUOTATO E NON SI PUO USARE

	MESHHANDLE msh_h = VB1->GetMeshTemplate(msh_idx);
	DEVMESHHANDLE devmsh_h = VB1->GetDevMesh(VB1->visual, msh_idx);
	UINT group_count = oapiMeshGroupCount(msh_h);
	if (group_count == 0) { return; }

	for (UINT i = 0; i < group_count; i++)
	{
		MESHGROUP *mg = oapiMeshGroup(msh_h, i);
		DWORD nVtx = mg->nVtx;
		DWORD nIdx = mg->nIdx;
		GROUPEDITSPEC grpspc;
		grpspc.flags = GRPEDIT_VTXCRD | GRPEDIT_VTXNML;
		grpspc.nVtx = nVtx;
		grpspc.Vtx = new NTVERTEX[grpspc.nVtx];
		grpspc.vIdx = new WORD[grpspc.nVtx];
		for (DWORD j = 0; j < nVtx; j++) {
			VECTOR3 vx = _V(mg->Vtx[j].x, mg->Vtx[j].y, mg->Vtx[j].z);
			VECTOR3 nml = _V(mg->Vtx[j].nx, mg->Vtx[j].ny, mg->Vtx[j].nz);
			VECTOR3 newvx = mul(rm, vx);
			VECTOR3 newnml = mul(rm, nml);
			grpspc.Vtx[j].x = newvx.x;
			grpspc.Vtx[j].y = newvx.y;
			grpspc.Vtx[j].z = newvx.z;
			grpspc.Vtx[j].nx = newnml.x;
			grpspc.Vtx[j].ny = newnml.y;
			grpspc.Vtx[j].nz = newnml.z;
			grpspc.vIdx[j] = j;
		}
		oapiEditMeshGroup(devmsh_h, i, &grpspc);
	}
	return;
}

void MeshManager::RotateMesh(msh_idx msh_idx, MATRIX3 rm) {
	MESHHANDLE msh_h = VB1->GetMeshTemplate(msh_idx);
	DEVMESHHANDLE devmsh_h = VB1->GetDevMesh(VB1->visual, msh_idx);
	return RotateMesh(msh_h, devmsh_h, rm);
}
void MeshManager::RotateMesh(MESHHANDLE mesh, DEVMESHHANDLE devmesh, MATRIX3 rm) {
	UINT group_count = oapiMeshGroupCount(mesh);
	if (group_count == 0) { return; }

	for (UINT i = 0; i<group_count; i++)
	{

		MESHGROUP *mg = oapiMeshGroup(mesh, i);
		DWORD nVtx = mg->nVtx;
		DWORD nIdx = mg->nIdx;

		GROUPREQUESTSPEC grprqstspc;
		grprqstspc.nVtx = nVtx;
		grprqstspc.nIdx = nIdx;
		grprqstspc.Vtx = new NTVERTEX[grprqstspc.nVtx];
		grprqstspc.Idx = new WORD[grprqstspc.nIdx];
		grprqstspc.VtxPerm = NULL;// new WORD[nVtx];
		grprqstspc.IdxPerm = NULL;// new WORD[nIdx];
		oapiGetMeshGroup(devmesh, i, &grprqstspc);

		GROUPEDITSPEC grpspc;
		grpspc.flags = GRPEDIT_VTXCRD | GRPEDIT_VTXNML;
		nVtx = grprqstspc.nVtx;
		grpspc.nVtx = nVtx;
		grpspc.Vtx = new NTVERTEX[grpspc.nVtx];
		grpspc.vIdx = new WORD[grpspc.nVtx];
		for (DWORD j = 0; j < nVtx; j++) {
			VECTOR3 vx = _V(grprqstspc.Vtx[j].x, grprqstspc.Vtx[j].y, grprqstspc.Vtx[j].z);
			VECTOR3 nml = _V(grprqstspc.Vtx[j].nx, grprqstspc.Vtx[j].ny, grprqstspc.Vtx[j].nz);
			VECTOR3 newvx = mul(rm, vx);
			VECTOR3 newnml = mul(rm, nml);
			grpspc.Vtx[j].x = newvx.x;
			grpspc.Vtx[j].y = newvx.y;
			grpspc.Vtx[j].z = newvx.z;
			grpspc.Vtx[j].nx = newnml.x;
			grpspc.Vtx[j].ny = newnml.y;
			grpspc.Vtx[j].nz = newnml.z;
			grpspc.vIdx[j] = j;
		}
		oapiEditMeshGroup(devmesh, i, &grpspc);

		delete[] grprqstspc.Vtx;
		delete[] grprqstspc.Idx;
		delete[] grpspc.Vtx;
		delete[] grpspc.vIdx;
	}
}

bool MeshManager::HighlightMesh(msh_idx msh_idx, bool Highlight) {
	def_idx mdidx = GetMeshDefIndexfromMsh_idx(msh_idx);
	if (mdidx == (UINT)-1) { return false; }

	
	DEVMESHHANDLE devmsh_h = VB1->GetDevMesh(VB1->visual, msh_idx);
	MESHHANDLE msh_h = VB1->GetMeshTemplate(msh_idx);
	if (msh_h == NULL) { return false; }
	UINT matcount = oapiMeshMaterialCount(msh_h);
	for (DWORD i = 0; i < matcount; i++) {
		MATERIAL *mat = oapiMeshMaterial(msh_h, i);
		MATERIAL mat2; //ARE WE SURE WE DON'T NEEAD A POINTER AND A NEW AND ALL THE STUFF HERE???
		mat2.ambient = mat->ambient;
		mat2.diffuse = mat->diffuse;
		mat2.emissive = mat->emissive;
		mat2.power = mat->power;
		mat2.specular = mat->specular;
		if (Highlight) {
			msh_defs[mdidx].Highlighted = true;
			if (HighLightColor == HIGHLIGHTCOLORRED) {
				mat2.emissive.r = 1;
			}
			else if (HighLightColor == HIGHLIGHTCOLORBLUE) {
				mat2.emissive.b = 1;
			}
			else if (HighLightColor == HIGHLIGHTCOLORGREEN) {
				mat2.emissive.g = 1;
			}

		}
		else {
			msh_defs[mdidx].Highlighted = false;
		}

		oapiSetMaterial(devmsh_h, i, &mat2);

	}

	return true;
}

void MeshManager::SetHighLightColor(UINT coloroption) {
	if (coloroption <= 2) {
		HighLightColor = coloroption;
	}
	else {
		HighLightColor = HIGHLIGHTCOLORRED;
	}
	return;	
}
UINT MeshManager::GetHighLightColor() {
	return HighLightColor;
}

void MeshManager::ResetHighlights() {
	for (UINT i = 0; i < msh_defs.size(); i++) {
		if (msh_defs[i].Highlighted) {
			HighlightMesh(msh_defs[i].msh_idx, false);
		}
	}
	return;
}
def_idx MeshManager::GetMeshDefIndexfromMsh_idx(msh_idx idx) {
	def_idx mdefindex = (UINT)-1;
	for (UINT i = 0; i < msh_defs.size(); i++) {
		if (msh_defs[i].msh_idx == idx) {
			mdefindex = i;
		}
	}
	return mdefindex;
}

bool MeshManager::DeleteMeshDef(def_idx idx) {
	if ((msh_defs.size() - 1 < idx) || (idx<0)) {
	//	SBLog("WARNING: Called a Delete Mesh Definition with out of range index");
		return false;
	}
	VB1->DelMesh(msh_defs[idx].msh_idx, false);
	if (msh_defs[idx].HighlightedGrps) {
		delete[] msh_defs[idx].HighlightedGrps;
		msh_defs[idx].HighlightedGrps = NULL;
	}
	msh_defs.erase(msh_defs.begin() + idx);
	nMeshes = msh_defs.size();
	return true;
}
bool MeshManager::ChangeMeshFile(def_idx idx, string newmeshname) {
	LogV("Substituting mesh definitions for index: %i old meshname:%s new meshname:%s", idx, msh_defs[idx].meshname.c_str(), newmeshname.c_str());
	if (msh_defs[idx].msh_idx != -1) {
		VB1->DelMesh(msh_defs[idx].msh_idx, false);
	}

	msh_defs[idx].meshname = newmeshname;
	msh_defs[idx].msh_h = NULL;
	//loading_msh_counter = idx;
	//oapiWriteLogV("loadingmc:%i idx:%i", loading_msh_counter, idx);
	g_RM = msh_defs[idx].rm;
	msh_defs[idx].msh_h = oapiLoadMeshGlobal(newmeshname.c_str(), LoadMeshClbk);
	msh_defs[idx].ngrps = oapiMeshGroupCount(msh_defs[idx].msh_h);


	if (msh_defs[idx].HighlightedGrps) {
		delete[] msh_defs[idx].HighlightedGrps;
		msh_defs[idx].HighlightedGrps = NULL;
	}
	if (msh_defs[idx].ngrps > 0) {
		msh_defs[idx].HighlightedGrps = new bool[msh_defs[idx].ngrps];
		for (int i = 0; i < msh_defs[idx].ngrps; i++) {
			msh_defs[idx].HighlightedGrps[i] = false;
		}
	}
	


	if (msh_defs[idx].msh_h == NULL) {
		LogV("WARNING! Mesh File Not Valid!");
		return false;
	}
	msh_defs[idx].msh_idx = VB1->AddMesh(msh_defs[idx].msh_h, &msh_defs[idx].ofs);
	msh_defs[idx].devmsh_h = VB1->GetDevMesh(VB1->visual, msh_defs[idx].msh_idx);
	LogV("Substitution complete: mesh added with mesh index:%i", msh_defs[idx].msh_idx);
	return true;

}
bool MeshManager::HighlightMeshGroup(msh_idx msh_idx, UINT grp_idx, bool Highlight) {

	DEVMESHHANDLE devmsh_h = VB1->GetDevMesh(VB1->visual, msh_idx);		//let's get the devmesh from the vessel
	MESHHANDLE temp_msh_h = VB1->GetMeshTemplate(msh_idx);				//let's get the template mesh from the vessel to have the original values
	if (temp_msh_h == NULL) { return false; }// you'll never know...

	MESHGROUP *mg = oapiMeshGroup(temp_msh_h, grp_idx);		//Get the original mesh group 
	if (UsingD3D9()) {											// if D3D9
		DWORD matidx = mg->MtrlIdx;								//Get The original material index
		MATERIAL *mat = oapiMeshMaterial(temp_msh_h, matidx);	//Get the original material definition
		MATERIAL *mat2 = new MATERIAL;							//Create the new material for the excahnge
		mat2->ambient = mat->ambient;							//Set all the values of the new material exactly as the original
		mat2->diffuse = mat->diffuse;
		mat2->emissive = mat->emissive;
		mat2->specular = mat->specular;
		mat2->power = mat->power;
		UINT index = IdxMsh2Def(msh_idx);
		if (Highlight) {										//if Highlight
			if (HighLightColor == HIGHLIGHTCOLORRED) {				
				mat2->emissive.r = 1;							// Set the emissive color of the new material at maximum 
			}
			else if (HighLightColor == HIGHLIGHTCOLORBLUE) {
				mat2->emissive.b = 1;
			}
			else if (HighLightColor == HIGHLIGHTCOLORGREEN) {
				mat2->emissive.g = 1;
			}
			
			msh_defs[index].HighlightedGrps[grp_idx] = true;
		}
		else {
			msh_defs[index].HighlightedGrps[grp_idx] = false;
		}
		oapiSetMaterial(devmsh_h, matidx, mat2);		//change the material to the new one. if not highlight the original will then be restored
		delete mat2;									//cleanup
	}
	else {												//if Inline client
		MESHHANDLE msh_h = VB1->GetMesh(VB1->visual, msh_idx);		//Get the Live mesh from the sim
		if (msh_h == NULL) { return false; }// you'll never know...
		DWORD mat_count = oapiMeshMaterialCount(msh_h);				//Get the material count to know the know the index
		MESHGROUP *mg_inline = oapiMeshGroup(msh_h, grp_idx);		//Get the meshgroup from the live sim
		UINT index = IdxMsh2Def(msh_idx);
		if (Highlight) {											//if highlight
			if (HighLightColor == HIGHLIGHTCOLORRED) {
				mg_inline->MtrlIdx = mat_count -1;					//change the material index to the one created at the beginning
			}
			else if (HighLightColor == HIGHLIGHTCOLORBLUE) {
				mg_inline->MtrlIdx = mat_count - 2;
			}
			else if (HighLightColor == HIGHLIGHTCOLORGREEN) {
				mg_inline->MtrlIdx = mat_count - 3;
			}
			msh_defs[index].HighlightedGrps[grp_idx] = true;
		}
		else {
			msh_defs[index].HighlightedGrps[grp_idx] = false;
			mg_inline->MtrlIdx = mg->MtrlIdx;					//if not highlight just restore with the template index
		}
	}
	
	return true;
}

void MeshManager::VisualCreated(VISHANDLE vis, int refcount) {
	for (UINT i = 0; i < msh_defs.size(); i++) {
		msh_defs[i].devmsh_h = VB1->GetDevMesh(vis, msh_defs[i].msh_idx);
	}
}
void MeshManager::VisualDestroyed(VISHANDLE vis, int refcount) {
	for (UINT i = 0; i < msh_defs.size(); i++) {
		msh_defs[i].devmsh_h = NULL;
	}
}
UINT MeshManager::GetMeshCount() {
	return nMeshes;
}
void MeshManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, ";<-------------------------MESHES DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	char cbuf[256] = { '\0' };
	char cbuf2[256] = { '\0' };
	for (UINT i = 0; i < msh_defs.size(); i++) {
		sprintf(cbuf, "MESH_%i_NAME", i);
		sprintf(cbuf2, "%s", msh_defs[i].meshname.c_str());
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "MESH_%i_POS", i);
		oapiWriteItem_vec(fh, cbuf, msh_defs[i].ofs);
		sprintf(cbuf, "MESH_%i_DIR", i);
		oapiWriteItem_vec(fh, cbuf, msh_defs[i].dir);
		sprintf(cbuf, "MESH_%i_ROT", i);
		oapiWriteItem_vec(fh, cbuf, msh_defs[i].rot);
		sprintf(cbuf, "MESH_%i_VIS", i);
		oapiWriteItem_int(fh, cbuf, msh_defs[i].visibility);
		oapiWriteLine(fh, " ");
	}
	return;
}
vector<MSH_DEF> MeshManager::GetAllDefs() {
	return msh_defs;
}

string MeshManager::GetMeshName(def_idx def_idx) {
	return msh_defs[def_idx].meshname;
}

msh_idx MeshManager::IdxDef2Msh(def_idx def_idx) {
	return msh_defs[def_idx].msh_idx;
}

def_idx MeshManager::IdxMsh2Def(msh_idx msh_idx) {
	return GetMeshDefIndexfromMsh_idx(msh_idx);
}
MATRIX3 MeshManager::GetMeshDefRM(def_idx def_idx) {
	return msh_defs[def_idx].rm;
}

void MeshManager::SetMeshDefRM(def_idx def_idx, MATRIX3 rm) {
	MATRIX3 offset = mul(rm, VB1->Inverse(msh_defs[def_idx].rm));
	VB1->FindDirRot(rm, msh_defs[def_idx].dir, msh_defs[def_idx].rot);
	RotateMesh(msh_defs[def_idx].msh_idx, offset);
	msh_defs[def_idx].rm = rm;
	//RotateMeshFromTemplate(msh_defs[def_idx].msh_idx, rm);
	return;
}
MESHHANDLE MeshManager::GetMeshDefMH(def_idx def_idx) {
	return msh_defs[def_idx].msh_h;
}
DEVMESHHANDLE MeshManager::GetMeshDefDMH(def_idx def_idx) {
	return msh_defs[def_idx].devmsh_h;
}
string MeshManager::GetMeshDefName(def_idx def_idx) {
	return msh_defs[def_idx].meshname;
}
VECTOR3 MeshManager::GetMeshDefPos(def_idx def_idx) {
	return msh_defs[def_idx].ofs;
}
VECTOR3 MeshManager::GetMeshDefDir(def_idx def_idx) {
	return msh_defs[def_idx].dir;
}
VECTOR3 MeshManager::GetMEshDefRot(def_idx def_idx) {
	return msh_defs[def_idx].rot;
}
void MeshManager::MoveMeshDef(def_idx def_idx, VECTOR3 delta_pos) {
	msh_defs[def_idx].ofs += delta_pos;
	VB1->ShiftMesh(IdxDef2Msh(def_idx), delta_pos);
	return;
}
void MeshManager::SetMeshDefDirRot(def_idx def_idx, VECTOR3 dir, VECTOR3 rot) {
	MATRIX3 rm = VB1->FindRM(dir, rot);
	return SetMeshDefRM(def_idx, rm);
}
UINT MeshManager::GetMeshDefNGrps(def_idx def_idx) {
	return msh_defs[def_idx].ngrps;
}
bool MeshManager::UsingD3D9() {
	return VB1->UsingD3D9();
}
bool MeshManager::MeshGroupIsHighlighted(msh_idx msh_idx, UINT grp_idx) {
	def_idx idx = IdxMsh2Def(msh_idx);
	return msh_defs[idx].HighlightedGrps[grp_idx];
}
void MeshManager::ResetMeshGroupHighlights(msh_idx msh_idx) {
	def_idx idx = IdxMsh2Def(msh_idx);
	for (UINT i = 0; i < msh_defs[idx].ngrps; i++) {
		if (msh_defs[idx].HighlightedGrps[i]) {
			HighlightMeshGroup(msh_idx, i, false);
		}
	}
}

msh_idx MeshManager::GetMeshIdx(def_idx d_idx) {
	return msh_defs[d_idx].msh_idx;
}
WORD MeshManager::GetMeshVisibility(def_idx d_idx) {
	return msh_defs[d_idx].visibility;
}
void MeshManager::SetMeshVisibility(def_idx d_idx, WORD visibility) {
	msh_defs[d_idx].visibility = visibility;
	VB1->SetMeshVisibilityMode(msh_defs[d_idx].msh_idx, visibility);
	return;
}
void MeshManager::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing Mesh Section");
	UINT mesh_counter = 0;
	char cbuf[256] = { '\0' };
	char item[256] = { '\0' };
	sprintf_s(cbuf, "MESH_%i_NAME", mesh_counter);
	while (oapiReadItem_string(fh, cbuf, item)) {
		string mn(item);
		VECTOR3 pos = _V(0, 0, 0);
		VECTOR3 dir = _V(0, 0, 1);
		VECTOR3 rot = _V(0, 1, 0);
		WORD visibility;
		sprintf_s(cbuf, "MESH_%i_POS", mesh_counter);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf_s(cbuf, "MESH_%i_DIR", mesh_counter);
		oapiReadItem_vec(fh, cbuf, dir);
		sprintf_s(cbuf, "MESH_%i_ROT", mesh_counter);
		oapiReadItem_vec(fh, cbuf, rot);
		sprintf(cbuf, "MESH_%i_VIS", mesh_counter);
		int vis;
		if (!oapiReadItem_int(fh, cbuf, vis)) { vis = 1; }
		visibility = (WORD)vis;
		AddMeshDef(mn, pos, dir, rot,visibility);
		mesh_counter++;
		sprintf_s(cbuf, "MESH_%i_NAME", mesh_counter);
	}
	LogV("Found %i Mesh Definitions", mesh_counter);
	//SBLog("Found %i Mesh Definitions", GetMeshCount());
	return;
}

void MeshManager::Clear() {
	LogV("Clearing Mesh Section");
	VB1->ClearMeshes();
	msh_defs.clear();
	nMeshes = 0;
	LogV("Clearing Mesh Section Completed");
}