// WinTestApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WinTestApp.h"
#include "TwoDimPlane.h"
#include "AirObjects.h"
#include "Node.h"
#include "GenerateMovesCombinations.h"
#include <sstream>
#include <fstream>
#include <iostream>

using namespace Gdiplus;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
ULONG_PTR m_gdiplusToken;
Gdiplus::GdiplusStartupInput gdiplusStartupInput;
int levelsInOneStep = 20;
Children  *generations;
Node *root;
AllPossibleMoves *moves; 
AllPossibleMoves *optionalMoves;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINTESTAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINTESTAPP));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINTESTAPP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINTESTAPP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   generations = new Children[100];
   NodeAllocator* instance = NodeAllocator::Instance();
   NodeAllocator* instance2 = NodeAllocator::Instance();

   hInst = hInstance; // Store instance handle in our global variable

   //Plane *gPlane = new Plane(500000, 500000);
   Plane *gPlane = instance->getPlane()->Init( 500000, 500000 );

   hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, gPlane->getMaxPoint().x , gPlane->getMaxPoint().y , NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   Gdiplus::GdiplusStartup( &m_gdiplusToken, &gdiplusStartupInput, NULL);
   //PPoint p1(600, 600), p2(120000, 20000), p3( 550, 800), p4( 1600, 800), p5( 1000, 800);
   //PPoint p1(9000, 20000), p2(90000, 15000), p3( 20000, 24000), p4( 1600, 800), p5( 1000, 800);
   //PPoint p1(60000, 10000), p2(80000, 90000), p3( 65000, 90000), p4( 90000, 10000), p5( 1000, 800);

   //Scenario 1, Position of Blue in more South
   /*PPoint p1( 40000, 100000), p2( 100000, 80000), p3( 12000, 50000), p4( 90000, 55000), p5( 1000, 800), targetPoint(60000, 200000);
   Aircraft *aircraft1 = instance->getAircraft()->Init(  p1, 200, East, Blue, 2000, Defender);
   Aircraft *aircraft2 = instance->getAircraft()->Init(  p2, 200, West, Red, 2000, Defender);
   gPlane->AddObjectOnPlane( aircraft1, FlyingObjectType::TypeAircraft);
   gPlane->AddObjectOnPlane( aircraft2, FlyingObjectType::TypeAircraft);*/

   //Scenario 2, Position of Blue in more North
   /*PPoint p1(30000, 100000), p2(90000, 140000), p3( 12000, 50000), p4( 90000, 55000), p5( 1000, 800);
   Aircraft *aircraft1 = instance->getAircraft()->Init(  p1, 200, South, Blue, 2000, Attacker);
   Aircraft *aircraft2 = instance->getAircraft()->Init(  p2, 200, North, Red, 2000, Defender);
   gPlane->AddObjectOnPlane( aircraft1, FlyingObjectType::TypeAircraft);
   gPlane->AddObjectOnPlane( aircraft2, FlyingObjectType::TypeAircraft);*/

   //Scenario 3, Blue behind red
   PPoint p1(10000, 90000), p2(70000, 80000), p3( 12000, 50000), p4( 90000, 55000), p5( 1000, 800);
   Aircraft *aircraft1 = instance->getAircraft()->Init(  p1, 200, East, Blue, 2000, Defender);
   Aircraft *aircraft2 = instance->getAircraft()->Init(  p2, 600, East, Red, 2000, Defender);
   gPlane->AddObjectOnPlane( aircraft1, FlyingObjectType::TypeAircraft);
   gPlane->AddObjectOnPlane( aircraft2, FlyingObjectType::TypeAircraft);

   //Scenario 4, Three aircrafts
   /*PPoint p1(10000, 100000), p2(70000, 80000), p3( 10000, 70000), p4( 90000, 90000), p5( 1000, 800);
   Aircraft *aircraft1 = instance->getAircraft()->Init(  p1, 200, NorthEast, Blue, 2000, Defender);
   Aircraft *aircraft2 = instance->getAircraft()->Init(  p2, 200, NorthWest, Red, 2000, Attacker);
   Aircraft *aircraft3 = instance->getAircraft()->Init(  p3, 200, NorthEast, Blue, 2000, Defender);
   gPlane->AddObjectOnPlane( aircraft1, FlyingObjectType::TypeAircraft);
   gPlane->AddObjectOnPlane( aircraft2, FlyingObjectType::TypeAircraft);
   gPlane->AddObjectOnPlane( aircraft3, FlyingObjectType::TypeAircraft);*/

   //Scenario 5, Four aircrafts
   /*PPoint p1(10000, 100000), p2(70000, 80000), p3( 10000, 80000), p4( 70000, 100000), p5( 1000, 800);
   Aircraft *aircraft1 = instance->getAircraft()->Init(  p1, 200, NorthEast, Blue, 2000, Attacker);
   Aircraft *aircraft2 = instance->getAircraft()->Init(  p2, 200, NorthWest, Red, 2000, Defender);
   Aircraft *aircraft3 = instance->getAircraft()->Init(  p3, 200, East, Blue, 2000, Defender);
   Aircraft *aircraft4 = instance->getAircraft()->Init(  p4, 200, West, Red, 2000, Defender);
   gPlane->AddObjectOnPlane( aircraft1, FlyingObjectType::TypeAircraft);
   gPlane->AddObjectOnPlane( aircraft2, FlyingObjectType::TypeAircraft);
   gPlane->AddObjectOnPlane( aircraft3, FlyingObjectType::TypeAircraft);
   gPlane->AddObjectOnPlane( aircraft4, FlyingObjectType::TypeAircraft);*/

   //PPoint p1(10000, 40000), p2(80000, 50000), p3( 65000, 90000), p4( 90000, 10000), p5( 1000, 800);
   
   /*Aircraft *aircraft1 = instance->getAircraft()->Init(  p1, 200, East, Blue, 2000, Attacker);
   Aircraft *aircraft2 = instance->getAircraft()->Init(  p2, 200, East, Red, 2000, Defender);*/
   //Aircraft *aircraft2 = instance->getAircraft()->Init(  p3, 200, NorthEast, Blue, 2000, Defender);
   //Aircraft *aircraft4 = instance->getAircraft()->Init(  p4, 200, North, Red, 2000, Defender);
   //Aircraft *aircraft1 = new Aircraft(  p1, 200, East, Blue, 2000, Attacker);
   //Aircraft *aircraft2 = new Aircraft(  p2, 200, West, Red, 2000, Defender);
   //Aircraft *aircraft3 = new Aircraft(  p3, 200, North, Red, 2000, Defender);
   //Aircraft *aircraft4 = new Aircraft(  p4, 200, South, Blue, 2000, Defender);
   //Aircraft aircraft3(  p3, 10000, South, Blue) ;
   //Aircraft aircraft4(  p4, 10000, South, Red) ;
   //Aircraft aircraft5(  p5, 10000, South, Red) ;

   /*gPlane->AddObjectOnPlane( aircraft1, FlyingObjectType::TypeAircraft);
   gPlane->AddObjectOnPlane( aircraft2, FlyingObjectType::TypeAircraft);*/
   //gPlane->AddObjectOnPlane( aircraft3, FlyingObjectType::TypeAircraft);
   //gPlane->AddObjectOnPlane( aircraft4, FlyingObjectType::TypeAircraft);
   //xyPlane->AddObjectOnPlane( aircraft4, FlyingObjectType::TypeAircraft);
   //xyPlane->AddObjectOnPlane( aircraft5, FlyingObjectType::TypeAircraft);

   //((Aircraft*)xyPlane->getObjectsInPlane()[0].pObject)->fireOrShoot( ((Aircraft*)xyPlane->getObjectsInPlane()[0].pObject)->getHeadingDirection(),
   //																	5000,
   //																		p3);

   //gPlane.getObjectsInPlane()[0].pObject->fireOrShoot( ((Aircraft*)gPlane.getObjectsInPlane()[0].pObject)->getHeadingDirection(),
   //																	5000,
   //																	p3);

   root = instance->getNode()->Init( NULL, gPlane, 0);
   //root->expandNode();

   GenerateCombinations combinations( gPlane->getObjectsInPlane()->size(), MaxAction);
   moves = combinations.generate();

   GenerateCombinations optionalCombinations( gPlane->getObjectsInPlane()->size(), MaxOptionalAction);
   optionalMoves = optionalCombinations.generate();

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   instance->ReleasePlane( gPlane);

   return TRUE;
}

void drawArc(HDC hdc)
{
	Graphics *graphics = new Graphics(hdc);
	Gdiplus::Pen pen( Color(255, 0, 0, 255));
	
	root->m_xyPlane->drawPlane( graphics, true, true);

	WCHAR message[64]; 

	RECT rect;
	SetBkMode(hdc,TRANSPARENT);

	std::wstringstream strStream;
	std::ofstream	   probFiles;

	probFiles.open("probfile.csv", std::ios::binary | std::ios::app);

	strStream<<root->m_level<<",";

	for (int i = 0; i < root->m_xyPlane->getObjectsInPlane()->size() ; i++)
	{

		swprintf( message, 
					L"     Speed: %d, TimeStep: %d", 
					(*root->m_xyPlane->getObjectsInPlane())[i].pObject->getSpeed(),
					root->m_level
					);

		rect.left= (*root->m_xyPlane->getObjectsInPlane())[i].pObject->getCurrentPosition().x/viewScale;
		rect.top= (*root->m_xyPlane->getObjectsInPlane())[i].pObject->getCurrentPosition().y/viewScale;
		DrawText( hdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP  ) ;


		strStream<<(*root->m_xyPlane->getObjectsInPlane())[i].pObject->getDefenceHeuristicValue()<<",";
		probFiles<< root->m_level<<" , "<<
			(*root->m_xyPlane->getObjectsInPlane())[i].pObject->getDefenceHeuristicValue()
			<<", "<<(*root->m_xyPlane->getObjectsInPlane())[i].pObject->getAttackHeuristicValue()<<", ";

		(*root->m_xyPlane->getObjectsInPlane())[i].pObject->setDefenceHeuristicValue(0);
		(*root->m_xyPlane->getObjectsInPlane())[i].pObject->setAttackHeuristicValue(0);
	}

	std::wstring wStr(strStream.str());
	OutputDebugString( wStr.c_str());
	root->m_collectiveHeuristic = 1.0;
	probFiles<<std::endl;
	probFiles.close();
	
	//graphics.Flush();

	delete graphics;

}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...

			//SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

			drawArc( hdc);

			//SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_KEYDOWN:
		{
			char kKey = 'n';
			UINT intKey = kKey - 32;
			NodeAllocator* instance = NodeAllocator::Instance();

			if ( wParam == intKey)
			{
				int level = root->m_level;

				for (int i = 0 ; i < root->m_xyPlane->getObjectsInPlane()->size(); i++)
				{
					CalculateACRelation rel( root->m_xyPlane,
											(*root->m_xyPlane->getObjectsInPlane())[i].pObject);
					rel.execute();
				}

				LookAhead lookAhead;
				lookAhead.Run( root, generations, levelsInOneStep, moves, optionalMoves);
				Node* tPlane = lookAhead.getBestPlane( generations);
				
				
				instance->ReleaseNode( root);


				root = instance->getNode()->Init( NULL, tPlane->m_xyPlane, level + 1);
				

				root->m_collectiveHeuristic = tPlane->m_collectiveHeuristic;
				//instance->ReleasePlane( tPlane );

				/*
					Empty the generations
				*/
				int count = 0;

				for (int i = 0 ; i < 100; i++ )
				{
					for (int j = 0 ; j < generations[i].size() ; j++)
					{
						//delete generations[i][j];
						instance->ReleaseNode( generations[i][j] );
						count++;
					}

					generations[i].clear();
					generations[i].resize(0);
				}
			}
			else if ( wParam == 107 || wParam == 187 )
			{
				viewScale = viewScale - 20;
			}
			else if ( wParam == 109 || wParam == 189 )
			{
				viewScale = viewScale + 20;
			}
			InvalidateRect( hWnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONDOWN:
		{
             PPoint pt;

			 wchar_t header[64];

			 pt.x = (int)LOWORD(lParam);
             pt.y = (int)HIWORD(lParam);

			 swprintf( header, L"X: %d, Y: %d", pt.x, pt.y);
			 
			 if ( (*root->m_xyPlane->getObjectsInPlane())[0].pObject->isInBVR( pt) )
			 {
				 MessageBox( NULL, L"Inside range.", header, MB_OK);
			 }
			 else
			 {
				 MessageBox( NULL, L"Outside range.", header, MB_OK);
			 }
			 InvalidateRect( hWnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
