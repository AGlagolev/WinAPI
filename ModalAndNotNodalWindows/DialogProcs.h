#pragma once
#include"resource.h"
#include <Windows.h>

BOOL CALLBACK DlgMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgModal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgNotModal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);