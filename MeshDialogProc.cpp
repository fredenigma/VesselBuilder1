#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "MeshManager.h"
#pragma comment(lib, "comctl32.lib")


void DialogControl::DlgRotateMesh(UINT msh_idx, VECTOR3 axis, double angle) {

	UINT index = MshMng->IdxMsh2Def(msh_idx);
	VECTOR3 rotated_axis = mul(MshMng->GetMeshDefRM(index), axis);


	MATRIX3 rm = rotm(rotated_axis, angle);

	//SB1->MshMng->RotateMesh(SB1->MshMng->GetMeshDefMH(index), SB1->MshMng->GetMeshDefDMH(index), rm);
	MATRIX3 oldrm = MshMng->GetMeshDefRM(index);
	MATRIX3 newrm = mul(rm, oldrm);
	//SB1->MshMng->msh_defs[index].rm = mul(rm, oldrm);
	MshMng->SetMeshDefRM(index, newrm);
	
	return;
}
void DialogControl::UpdateMeshDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	
	if (idx >= MshMng->GetMeshCount()) { return; }
	
	SetDlgItemText(hwnd_Mesh, IDC_EDIT_MESHNAME, (LPCSTR)MshMng->GetMeshName(idx).c_str());
	SetDlgItemsTextVector3(hwnd_Mesh, IDC_EDIT_POSX, IDC_EDIT_POSY, IDC_EDIT_POSZ, MshMng->GetMeshDefPos(idx));
	SetDlgItemsTextVector3(hwnd_Mesh, IDC_EDIT_DIRX, IDC_EDIT_DIRY, IDC_EDIT_DIRZ, MshMng->GetMeshDefDir(idx));
	SetDlgItemsTextVector3(hwnd_Mesh, IDC_EDIT_ROTX, IDC_EDIT_ROTY, IDC_EDIT_ROTZ, MshMng->GetMEshDefRot(idx));
	char cbuf[128] = { '\0' };
	sprintf_s(cbuf, "%.1f", speedrotation*DEG);
	SetDlgItemText(hwnd_Mesh, IDC_EDIT_GETANGLE, (LPCSTR)cbuf);
	WORD vis = MshMng->GetMeshVisibility(idx);

	SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXTPASS, BM_SETCHECK, BST_UNCHECKED, 0);

	if (vis & MESHVIS_EXTERNAL) {
	SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_SETCHECK, BST_CHECKED, 0);
	}
	if (vis & MESHVIS_COCKPIT) {
	SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_SETCHECK, BST_CHECKED, 0);
	}
	if (vis & MESHVIS_VC) {
	SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_SETCHECK, BST_CHECKED, 0);
	}
	if (vis & MESHVIS_EXTPASS) {
	SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXTPASS, BM_SETCHECK, BST_CHECKED, 0);
	}
	/*if (idx >= MshMng->GetMeshCount()) { return; }
	SetDlgItemText(hwnd_Mesh, IDC_EDIT_MESHNAME, (LPCSTR)MshMng->GetMeshDefName(idx).c_str());
	SetDlgItemsTextVector3(hwnd_Mesh, IDC_EDIT_POSX, IDC_EDIT_POSY, IDC_EDIT_POSZ, MshMng->GetMeshDefPos(idx));
	SetDlgItemsTextVector3(hwnd_Mesh, IDC_EDIT_DIRX, IDC_EDIT_DIRY, IDC_EDIT_DIRZ, MshMng->GetMeshDefDir(idx));
	SetDlgItemsTextVector3(hwnd_Mesh, IDC_EDIT_ROTX, IDC_EDIT_ROTY, IDC_EDIT_ROTZ, MshMng->GetMEshDefRot(idx));
	char cbuf[128] = { '\0' };
	sprintf_s(cbuf, "%.1f", speedrotation*DEG);
	SetDlgItemText(hwnd_Mesh, IDC_EDIT_GETANGLE, (LPCSTR)cbuf);
	WORD vis = MshMng->GetMeshVisibility(idx);

	SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXTPASS, BM_SETCHECK, BST_UNCHECKED, 0);
	
	if (vis & MESHVIS_EXTERNAL) {
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_SETCHECK, BST_CHECKED, 0);
	}
	if (vis & MESHVIS_COCKPIT) {
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_SETCHECK, BST_CHECKED, 0);
	}
	if (vis & MESHVIS_VC) {
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_SETCHECK, BST_CHECKED, 0);
	}
	if (vis & MESHVIS_EXTPASS) {
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXTPASS, BM_SETCHECK, BST_CHECKED, 0);
	}*/
	/*switch (vis) {
	case MESHVIS_NEVER:
	{
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_SETCHECK, BST_UNCHECKED, 0);
		break;
	}
	case MESHVIS_EXTERNAL:
	{
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_SETCHECK, BST_UNCHECKED, 0);
		break;
	}
	case MESHVIS_COCKPIT:
	{
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_SETCHECK, BST_UNCHECKED, 0);
		break;
	}
	case MESHVIS_ALWAYS:
	{
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_SETCHECK, BST_UNCHECKED, 0);
		break;
	}
	case MESHVIS_VC:
	{
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_SETCHECK, BST_CHECKED, 0);
		break;
	}
	case MESHVIS_VC|MESHVIS_EXTERNAL:
	{
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_SETCHECK, BST_CHECKED, 0);
		break;
	}
	case MESHVIS_VC|MESHVIS_COCKPIT:
	{
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_SETCHECK, BST_CHECKED, 0);
		break;
	}
	case MESHVIS_VC|MESHVIS_ALWAYS:
	{
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_SETCHECK, BST_CHECKED, 0);
		break;
	}
	}*/
	return;
}
void DialogControl::MeshNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	UINT idx = CurrentSelection.idx;
	switch (LOWORD(wParam))
	{
	case IDC_SPIN_ROLL:
	{
		if (CurrentSelection.Type != MESH) { break; }
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			char cbuf[128] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_GETANGLE, (LPSTR)cbuf, 128);
			speedrotation = atof(cbuf) *RAD;
			VECTOR3 axis = _V(0, 0, -1);
			double angle = speedrotation*(double)lpd->iDelta;
			DlgRotateMesh(MshMng->IdxDef2Msh(idx), axis, angle);
			UpdateMeshDialog(hWnd);
			SendDlgItemMessage(hWnd, IDC_SPIN_ROLL, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_PITCH:
	{
		if (CurrentSelection.Type != MESH) { break; }
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			char cbuf[128] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_GETANGLE, (LPSTR)cbuf, 128);
			speedrotation = atof(cbuf) *RAD;
			VECTOR3 axis = _V(-1, 0, 0);
			double angle = speedrotation*(double)lpd->iDelta;
			DlgRotateMesh(MshMng->IdxDef2Msh(idx), axis, angle);
			UpdateMeshDialog(hWnd);
			SendDlgItemMessage(hWnd, IDC_SPIN_PITCH, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_YAW:
	{
		if (CurrentSelection.Type != MESH) { break; }
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			char cbuf[128] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_GETANGLE, (LPSTR)cbuf, 128);
			speedrotation = atof(cbuf) *RAD;
			VECTOR3 axis = _V(0, 1, 0);
			double angle = speedrotation*(double)lpd->iDelta;
			DlgRotateMesh(MshMng->IdxDef2Msh(idx), axis, angle);
			UpdateMeshDialog(hWnd);
			SendDlgItemMessage(hWnd, IDC_SPIN_YAW, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_POSX_L:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double speed = 0.5;
			double delta = (double)lpd->iDelta*speed;
			VECTOR3 delta_pos = _V(delta, 0, 0);
			MshMng->MoveMeshDef(idx, delta_pos);
			UpdateMeshDialog(hWnd);

			SendDlgItemMessage(hWnd, IDC_SPIN_POSX_L, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_POSX_S:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double speed = 0.1;
			double delta = (double)lpd->iDelta*speed;
			VECTOR3 delta_pos = _V(delta, 0, 0);
			MshMng->MoveMeshDef(idx, delta_pos);
			UpdateMeshDialog(hWnd);

			SendDlgItemMessage(hWnd, IDC_SPIN_POSX_S, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_POSY_L:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double speed = 0.5;
			double delta = (double)lpd->iDelta*speed;
			VECTOR3 delta_pos = _V(0, delta, 0);
			MshMng->MoveMeshDef(idx, delta_pos);
			UpdateMeshDialog(hWnd);

			SendDlgItemMessage(hWnd, IDC_SPIN_POSY_L, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_POSY_S:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double speed = 0.1;
			double delta = (double)lpd->iDelta*speed;
			VECTOR3 delta_pos = _V(0, delta, 0);
			MshMng->MoveMeshDef(idx, delta_pos);
			UpdateMeshDialog(hWnd);

			SendDlgItemMessage(hWnd, IDC_SPIN_POSY_S, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_POSZ_L:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double speed = 0.5;
			double delta = (double)lpd->iDelta*speed;
			VECTOR3 delta_pos = _V(0, 0, delta);
			MshMng->MoveMeshDef(idx, delta_pos);
			UpdateMeshDialog(hWnd);

			SendDlgItemMessage(hWnd, IDC_SPIN_POSZ_L, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_POSZ_S:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double speed = 0.1;
			double delta = (double)lpd->iDelta*speed;
			VECTOR3 delta_pos = _V(0, 0, delta);
			MshMng->MoveMeshDef(idx, delta_pos);
			UpdateMeshDialog(hWnd);

			SendDlgItemMessage(hWnd, IDC_SPIN_POSZ_S, UDM_SETPOS, 0, 0);
		}
		break;
	}

	}
	return;
}

BOOL DialogControl::MeshDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_SPIN_POSX_L, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_POSX_S, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_POSY_L, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_POSY_S, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_POSZ_L, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_POSZ_S, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_YAW, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_ROLL, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_PITCH, UDM_SETRANGE32, -10000, 10000);
		break;
	}
	case WM_NOTIFY:
	{
		//oapiWriteLogV("IDX:%i", CurrentSelection.idx);
		MeshNotify(hWnd, wParam, lParam);
		return true;
	}
	case WM_COMMAND:
	{
		
		switch (LOWORD(wParam)) {
		case IDC_BTN_MESHPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_POSX, IDC_EDIT_POSY, IDC_EDIT_POSZ, VB1->vclip.pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_DIRX, IDC_EDIT_DIRY, IDC_EDIT_DIRZ, VB1->vclip.dir);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ROTX, IDC_EDIT_ROTY, IDC_EDIT_ROTZ, VB1->vclip.rot);
			}
			break;
		}
		case IDC_BUTTON_POS_SET:
		{
			VECTOR3 newpos = GetDlgItemsVector3(hWnd, IDC_EDIT_POSX, IDC_EDIT_POSY, IDC_EDIT_POSZ);
			VECTOR3 offset = newpos - MshMng->GetMeshDefPos(CurrentSelection.idx);
			MshMng->MoveMeshDef(CurrentSelection.idx, offset);
			UpdateMeshDialog(hWnd);

			break;
		}
		case IDC_CHECK_HIGHLIGHT_MSH:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				if (CurrentSelection.Type != MESH) { break; }

				LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_MSH, BM_GETCHECK, 0, 0);
				if (getcheck == BST_CHECKED) {
					MshMng->HighlightMesh(MshMng->IdxDef2Msh(CurrentSelection.idx), true);
				}
				else {
					MshMng->HighlightMesh(MshMng->IdxDef2Msh(CurrentSelection.idx), false);
				}
			}
			break;
		}
		case IDC_CHECK_MSHVISEXT:
		case IDC_CHECK_MSHVISCOC:
		case IDC_CHECK_MSHVISVC:
		case IDC_CHECK_MSHVISEXTPASS:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				LRESULT getcheck_ext = SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXT, BM_GETCHECK, 0, 0);
				LRESULT getcheck_coc = SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISCOC, BM_GETCHECK, 0, 0);
				LRESULT getcheck_vc = SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISVC, BM_GETCHECK, 0, 0);
				LRESULT getcheck_extpass = SendDlgItemMessage(hWnd, IDC_CHECK_MSHVISEXTPASS, BM_GETCHECK, 0, 0);
				WORD visibility = MESHVIS_NEVER;
				if (getcheck_ext == BST_CHECKED) {
					visibility += MESHVIS_EXTERNAL;
				}
				if (getcheck_coc == BST_CHECKED) {
					visibility += MESHVIS_COCKPIT;
				}
				if (getcheck_vc == BST_CHECKED) {
					visibility += MESHVIS_VC;
				}
				if (getcheck_extpass == BST_CHECKED) {
					visibility += MESHVIS_EXTPASS;
				}
				MshMng->SetMeshVisibility(CurrentSelection.idx, visibility);
				

			}
			break;
		}
		
		
		case IDC_BUTTON_DIR_SET:
		{
			VECTOR3 dir = GetDlgItemsVector3(hWnd, IDC_EDIT_DIRX, IDC_EDIT_DIRY, IDC_EDIT_DIRZ);
			normalise(dir);
			MshMng->SetMeshDefDirRot(CurrentSelection.idx, dir, MshMng->GetMEshDefRot(CurrentSelection.idx));
			UpdateMeshDialog(hWnd);
			break;
		}
		case IDC_BUTTON_ROT_SET:
		{
			VECTOR3 rot = GetDlgItemsVector3(hWnd, IDC_EDIT_ROTX, IDC_EDIT_ROTY, IDC_EDIT_ROTZ);
			normalise(rot);
			MshMng->SetMeshDefDirRot(CurrentSelection.idx, MshMng->GetMeshDefDir(CurrentSelection.idx), rot); 
			UpdateMeshDialog(hWnd);
			break;
		}
		case IDC_BUTTON_DELETEMESHDEF:
		{
			bool test = MshMng->DeleteMeshDef(CurrentSelection.idx);
			UpdateTree(hDlg, MESH, CurrentSelection.config);
			break;
		}
		case IDC_BUTTON_LOADMESH:
		{

			//string meshdir = VB1->OrbiterRoot;
			//meshdir += "\\Meshes\\";
			string meshdir = VB1->meshdir;
			/*char absmeshdir_c[MAX_PATH];
			char currentdir1[MAX_PATH];
			GetCurrentDirectory(sizeof(currentdir1) / sizeof(char), currentdir1);
			SetCurrentDirectory(meshdir.c_str());
			GetCurrentDirectory(sizeof(absmeshdir_c) / sizeof(char), absmeshdir_c);
			string absmeshdir(absmeshdir_c);
			SetCurrentDirectory(currentdir1);
			*/
			OPENFILENAME ofn;       // common dialog box structure
			char szFile[260] = { '\0' };       // buffer for file name
											   //HWND hwnd;              // owner window
											   //			HANDLE hf;              // file handle

											   // Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "Mesh Files\0*.msh\0\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = meshdir.c_str();// meshdir.c_str();
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			char currentdir[MAX_PATH];

			GetCurrentDirectory(sizeof(currentdir) / sizeof(char), currentdir);

			if (GetOpenFileName(&ofn) == TRUE)
			{
				//	hf = CreateFile(ofn.lpstrFile,
				//		GENERIC_READ,
				//		0,
				//		(LPSECURITY_ATTRIBUTES)NULL,
				//		OPEN_EXISTING,
				//		FILE_ATTRIBUTE_NORMAL,
				//		(HANDLE)NULL);
				SetCurrentDirectory(currentdir);

				/*char cbuf[128] = { '\0' };
				for (UINT i = 0; i < ofn.nFileExtension-1-meshdir.size(); i++) {
				cbuf[i] = szFile[i + meshdir.size()];
				}*/

				//sprintf(test, szFile);
				string result(szFile);
				string filename = result.substr(meshdir.size()+1, ofn.nFileExtension - 2 - meshdir.size());//result.size()-meshdir.size()-
				//oapiWriteLogV("filename:%s", filename.c_str());
				SetWindowText(GetDlgItem(hWnd, IDC_EDIT_MESHNAME), filename.c_str());
				bool changed = MshMng->ChangeMeshFile(CurrentSelection.idx, filename);
				if (changed) {
					TVITEM tvi;
					tvi.mask = TVIF_TEXT;
					tvi.pszText = (LPSTR)filename.c_str();
					tvi.cchTextMax = filename.size();
					tvi.hItem = CurrentSelection.hitem;
					SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
					//HTREEITEM hitem = TreeView_GetItem(GetDlgItem(hDlg,IDC_TREE1),)
				}
			}




			break;
		}
		//case IDCANCEL:
		}
		//sprintf(oapiDebugString(), "%i", LOWORD(wParam));
		break;
	}
	}


	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}