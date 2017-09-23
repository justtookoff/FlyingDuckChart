//FlyingDuckChartApp.cpp
#include "FlyingDuckChartApp.h"
#include "FlyingDuckChart.h"

BOOL FlyingDuckChartApp::InitInstance() {
	FlyingDuckChart *flyingDuckChart = new FlyingDuckChart;
	flyingDuckChart->Create(NULL,"FlyingDuckChart");
	flyingDuckChart->ShowWindow(this->m_nCmdShow);
	flyingDuckChart->ShowWindow(SW_SHOWMAXIMIZED);
	flyingDuckChart->UpdateWindow();
	this->m_pMainWnd = flyingDuckChart;

	return TRUE;
}

FlyingDuckChartApp flyingDuckChartApp;
