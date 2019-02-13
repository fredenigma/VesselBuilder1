#include "StationBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "MeshManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::DlgRotateMesh(UINT msh_idx, VECTOR3 axis, double angle) {

	UINT index = SB1->MshMng->IdxMsh2Def(msh_idx);
	VECTOR3 rotated_axis = mul(SB1->MshMng->GetMeshDefRM(index), axis);


	MATRIX3 rm = rotm(rotated_axis, angle);

	//SB1->MshMng->RotateMesh(SB1->MshMng->GetMeshDefMH(index), SB1->MshMng->GetMeshDefDMH(index), rm);
	MATRIX3 oldrm = SB1->MshMng->GetMeshDefRM(index);
	MATRIX3 newrm = mul(rm, oldrm);
	//SB1->MshMng->msh_defs[index].rm = mul(rm, oldrm);
	SB1->MshMng->SetMeshDefRM(index, newrm);
	
	return;
}
void DialogControl::UpdateMeshDialog(HWND hWnd) {
	if (CurrentSelection.Type != MESH) { return; }
	UINT idx = CurrentSelection.idx;
	SetDlgItemText(hwnd_Mesh, IDC_EDIT_MESHNAME, (LPCSTR)SB1->MshMng->GetMeshDefName(idx).c_str());
	SetDlgItemsTextVector3(hwnd_Mesh, IDC_EDIT_POSX, IDC_EDIT_POSY, IDC_EDIT_POSZ, SB1->MshMng->GetMeshDefPos(idx));
	SetDlgItemsTextVector3(hwnd_Mesh, IDC_EDIT_DIRX, IDC_EDIT_DIRY, IDC_EDIT_DIRZ, SB1->MshMng->GetMeshDefDir(idx));
	SetDlgItemsTextVector3(hwnd_Mesh, IDC_EDIT_ROTX, IDC_EDIT_ROTY, IDC_EDIT_ROTZ, SB1->MshMng->GetMEshDefRot(idx));
	char cbuf[128] = { '\0' };
	sprintf_s(cbuf, "%.1f", speedrotation*DEG);
	SetDlgItemText(hwnd_Mesh, IDC_EDIT_GETANGLE, (LPCSTR)cbuf);
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
			DlgRotateMesh(SB1->MshMng->IdxDef2Msh(idx), axis, angle);
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
			DlgRotateMesh(SB1->MshMng->IdxDef2Msh(idx), axis, angle);
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
			DlgRotateMesh(SB1->MshMng->IdxDef2Msh(idx), axis, angle);
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
			SB1->MshMng->MoveMeshDef(idx, delta_pos);
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
			SB1->MshMng->MoveMeshDef(idx, delta_pos);
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
			SB1->MshMng->MoveMeshDef(idx, delta_pos);
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
			SB1->MshMng->MoveMeshDef(idx, delta_pos);
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
			SB1->MshMng->MoveMeshDef(idx, delta_pos);
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
			SB1->MshMng->MoveMeshDef(idx, delta_pos);
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
			if (SB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_POSX, IDC_EDIT_POSY, IDC_EDIT_POSZ, SB1->vclip.pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_DIRX, IDC_EDIT_DIRY, IDC_EDIT_DIRZ, SB1->vclip.dir);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ROTX, IDC_EDIT_ROTY, IDC_EDIT_ROTZ, SB1->vclip.rot);
			}
			break;
		}
		case IDC_BUTTON_POS_SET:
		{
			VECTOR3 newpos = GetDlgItemsVector3(hWnd, IDC_EDIT_POSX, IDC_EDIT_POSY, IDC_EDIT_POSZ);
			VECTOR3 offset = newpos - SB1->MshMng->GetMeshDefPos(CurrentSelection.idx);
			SB1->MshMng->MoveMeshDef(CurrentSelection.idx, offset);
			UpdateMeshDialog(hWnd);

			break;
		}
		case IDC_CHECK_HIGHLIGHT_MSH:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				if (CurrentSelection.Type != MESH) { break; }

				LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_MSH, BM_GETCHECK, 0, 0);
				if (getcheck == BST_CHECKED) {
					SB1->MshMng->HighlightMesh(SB1->MshMng->IdxDef2Msh(CurrentSelection.idx), true);
				}
				else {
					SB1->MshMng->HighlightMesh(SB1->MshMng->IdxDef2Msh(CurrentSelection.idx), false);
				}
			}
			break;
		}
		case IDC_BUTTON_DIR_SET:
		{
			VECTOR3 dir = GetDlgItemsVector3(hWnd, IDC_EDIT_DIRX, IDC_EDIT_DIRY, IDC_EDIT_DIRZ);
			normalise(dir);
			SB1->MshMng->SetMeshDefDirRot(CurrentSelection.idx, dir, SB1->MshMng->GetMEshDefRot(CurrentSelection.idx));
			UpdateMeshDialog(hWnd);
			break;
		}
		case IDC_BUTTON_ROT_SET:
		{
			VECTOR3 rot = GetDlgItemsVector3(hWnd, IDC_EDIT_ROTX, IDC_EDIT_ROTY, IDC_EDIT_ROTZ);
			normalise(rot);
			SB1->MshMng->SetMeshDefDirRot(CurrentSelection.idx, SB1->MshMng->GetMeshDefDir(CurrentSelection.idx), rot); 
			UpdateMeshDialog(hWnd);
			break;
		}
		case IDC_BUTTON_DELETEMESHDEF:
		{
			bool test = SB1->MshMng->DeleteMeshDef(CurrentSelection.idx);
			UpdateTree(hDlg, MESH,hrootMeshes);
			//sprintf(oapiDebugString(), "%i", test);
			break;
		}
		case IDC_BUTTON_LOADMESH:
		{

			string meshdir = SB1->OrbiterRoot;
			meshdir += "\\Meshes\\";

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
				string filename = result.substr(meshdir.size(), ofn.nFileExtension - 1 - meshdir.size());//result.size()-meshdir.size()-

				SetWindowText(GetDlgItem(hWnd, IDC_EDIT_MESHNAME), filename.c_str());
				bool changed = SB1->MshMng->ChangeMeshFile(CurrentSelection.idx, filename);
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