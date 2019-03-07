#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "LightsManager.h"

#pragma comment(lib, "comctl32.lib")
void DialogControl::UpdateColorExamples(HWND hWnd, VECTOR3 col) {
	//PAINTSTRUCT ps;
	HDC hDC = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	SelectObject(hDC, GetStockObject(DC_BRUSH));
	int r, g, b;
	VECTOR3 color = col * 255;
	r = (int)color.x;
	g = (int)color.y;
	b = (int)color.z;
	SetDCBrushColor(hDC, RGB(r, g, b));
	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	ReleaseDC(hWnd, hDC);
	return;
}
BOOL DialogControl::LightCreationDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		int EditDlgs[9] = { IDC_EDIT_DIFFR,IDC_EDIT_DIFFG,IDC_EDIT_DIFFB,IDC_EDIT_SPECR,IDC_EDIT_SPECG,IDC_EDIT_SPECB,IDC_EDIT_AMBR,IDC_EDIT_AMBG,IDC_EDIT_AMBB };
		for (UINT i = 0; i < 9; i++) {
			SetDlgItemText(hWnd, EditDlgs[i], "1.0");
		}
		break;
	}
	case WM_PAINT:
	{
		InvalidateRect(GetDlgItem(hWnd, IDC_STATIC_DIFF), NULL, FALSE);
		UpdateWindow(GetDlgItem(hWnd, IDC_STATIC_DIFF));
		VECTOR3 col_d = GetDlgItemsVector3(hWnd, IDC_EDIT_DIFFR, IDC_EDIT_DIFFG, IDC_EDIT_DIFFB);
		UpdateColorExamples(GetDlgItem(hWnd, IDC_STATIC_DIFF),col_d);

		InvalidateRect(GetDlgItem(hWnd, IDC_STATIC_SPEC), NULL, FALSE);
		UpdateWindow(GetDlgItem(hWnd, IDC_STATIC_SPEC));
		VECTOR3 col_s = GetDlgItemsVector3(hWnd, IDC_EDIT_SPECR, IDC_EDIT_SPECG, IDC_EDIT_SPECB);
		UpdateColorExamples(GetDlgItem(hWnd, IDC_STATIC_SPEC), col_s);

		InvalidateRect(GetDlgItem(hWnd, IDC_STATIC_AMB), NULL, FALSE);
		UpdateWindow(GetDlgItem(hWnd, IDC_STATIC_AMB));
		VECTOR3 col_a = GetDlgItemsVector3(hWnd, IDC_EDIT_AMBR, IDC_EDIT_AMBG, IDC_EDIT_AMBB);
		UpdateColorExamples(GetDlgItem(hWnd, IDC_STATIC_AMB), col_a);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDC_EDIT_DIFFR:
		case IDC_EDIT_DIFFG:
		case IDC_EDIT_DIFFB:
		{
			if (HIWORD(wParam) != EN_SETFOCUS && (HIWORD(wParam) != EN_KILLFOCUS)) {
				InvalidateRect(GetDlgItem(hWnd, IDC_STATIC_DIFF), NULL, FALSE);
				UpdateWindow(GetDlgItem(hWnd, IDC_STATIC_DIFF));
				VECTOR3 col_d = GetDlgItemsVector3(hWnd, IDC_EDIT_DIFFR, IDC_EDIT_DIFFG, IDC_EDIT_DIFFB);
				UpdateColorExamples(GetDlgItem(hWnd, IDC_STATIC_DIFF), col_d);
			}
			break;
		}
		case IDC_EDIT_SPECR:
		case IDC_EDIT_SPECG:
		case IDC_EDIT_SPECB:
		{
			if (HIWORD(wParam) != EN_SETFOCUS && (HIWORD(wParam) != EN_KILLFOCUS)) {
				InvalidateRect(GetDlgItem(hWnd, IDC_STATIC_SPEC), NULL, FALSE);
				UpdateWindow(GetDlgItem(hWnd, IDC_STATIC_SPEC));
				VECTOR3 col_s = GetDlgItemsVector3(hWnd, IDC_EDIT_SPECR, IDC_EDIT_SPECG, IDC_EDIT_SPECB);
				UpdateColorExamples(GetDlgItem(hWnd, IDC_STATIC_SPEC), col_s);

			}
			break;
		}
		case IDC_EDIT_AMBR:
		case IDC_EDIT_AMBG:
		case IDC_EDIT_AMBB:
		{
			InvalidateRect(GetDlgItem(hWnd, IDC_STATIC_AMB), NULL, FALSE);
			UpdateWindow(GetDlgItem(hWnd, IDC_STATIC_AMB));
			VECTOR3 col_a = GetDlgItemsVector3(hWnd, IDC_EDIT_AMBR, IDC_EDIT_AMBG, IDC_EDIT_AMBB);
			UpdateColorExamples(GetDlgItem(hWnd, IDC_STATIC_AMB), col_a);
			break;
		}
		case IDC_BUTTON_ADDPOINT:
		{
			VECTOR3 col_d = GetDlgItemsVector3(hWnd, IDC_EDIT_DIFFR, IDC_EDIT_DIFFG, IDC_EDIT_DIFFB);
			VECTOR3 col_s = GetDlgItemsVector3(hWnd, IDC_EDIT_SPECR, IDC_EDIT_SPECG, IDC_EDIT_SPECB);
			VECTOR3 col_a = GetDlgItemsVector3(hWnd, IDC_EDIT_AMBR, IDC_EDIT_AMBG, IDC_EDIT_AMBB);
			COLOUR4 diff, spec, amb;
			diff.r = col_d.x;
			diff.g = col_d.y;
			diff.b = col_d.z;
			diff.a = 0;
			spec.r = col_s.x;
			spec.g = col_s.y;
			spec.b = col_s.z;
			spec.a = 0;
			amb.r = col_a.x;
			amb.g = col_a.y;
			amb.b = col_a.z;
			amb.a = 0;
			LightsMng->AddLightDef(LightEmitter::TYPE::LT_POINT,diff,spec,amb);
			UpdateTree(hDlg, LIGHTS, 0);
			break;
		}
		case IDC_BUTTON_ADDSPOT:
		{
			VECTOR3 col_d = GetDlgItemsVector3(hWnd, IDC_EDIT_DIFFR, IDC_EDIT_DIFFG, IDC_EDIT_DIFFB);
			VECTOR3 col_s = GetDlgItemsVector3(hWnd, IDC_EDIT_SPECR, IDC_EDIT_SPECG, IDC_EDIT_SPECB);
			VECTOR3 col_a = GetDlgItemsVector3(hWnd, IDC_EDIT_AMBR, IDC_EDIT_AMBG, IDC_EDIT_AMBB);
			COLOUR4 diff, spec, amb;
			diff.r = col_d.x;
			diff.g = col_d.y;
			diff.b = col_d.z;
			diff.a = 0;
			spec.r = col_s.x;
			spec.g = col_s.y;
			spec.b = col_s.z;
			spec.a = 0;
			amb.r = col_a.x;
			amb.g = col_a.y;
			amb.b = col_a.z;
			amb.a = 0;
			LightsMng->AddLightDef(LightEmitter::TYPE::LT_SPOT,diff,spec,amb);
			UpdateTree(hDlg, LIGHTS, 0);
			break;
		}
		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}