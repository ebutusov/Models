// ModelsView.h : interface of the CModelsView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "msModel.h"
#include "Camera.h"
#include "MySimulation.h"
#include "GLRenderer.h"
#include "IControlListener.h"
#include "TextureLoader.h"
#include <vector>

class CModelsView : public CWindowImpl<CModelsView>, COpenGL<CModelsView>
{
private:
	std::vector<msModel*> m_models;
	typedef std::vector<msModel *>::iterator miter_t;
	msModel m_model;
	DWORD m_last_frame, m_start_frame;
	bool m_drag;
	char m_keys[256];
	bool m_LPM;
	float m_camera_speed;

	GLuint m_floor_texture;

	CGLRenderer m_renderer;
	IControlListener *m_sink;
	CSimulationGL *m_xsim;

public:
	DECLARE_WND_CLASS(NULL)
	BOOL PreTranslateMessage(MSG* pMsg);

	CModelsView();

	BEGIN_MSG_MAP(CModelsView)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDown) 
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP(COpenGL<CModelsView>)
	END_MSG_MAP()

	// XXX shoud this be public?
	CCamera m_cam;

	void OnInit();
	void OnRender();
	void Frame(DWORD time);
	void FrameNow();
	void OnResize(int cx, int cy);
	void ProcessMouseEvents();

	void CaptureReleaseMouse();

	void OnActivate(bool activate);

	// TEST
	void SetSimulation(CSimulationGL *sim);
	void SetControlListener(IControlListener *l) { m_sink = l; }
	// TEST

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	//LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);



};
