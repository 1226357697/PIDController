#include <Windows.h>
#include <Windowsx.h>
#include <iostream>
#include "PIDController.h"
#include <thread>
#include "bezier.h"
#define ISPID  0

POINT ball_pos;
POINT mouse_pos;

#if ISPID
PIDController pidContorl;
PIDController pidContorl2;
#else
bezier pidContorl;
bezier pidContorl2;
#endif
bool is_run = false;

void mouse_abs_move(int x, int y)
{
  mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x, y, 0, NULL);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_CREATE:
    {
      is_run = true;
      ball_pos.x = 0;
      ball_pos.y = 0;

#if ISPID
      pidContorl.setProportion(0.5f);
      pidContorl.setIntegral(0.3f);
      pidContorl.setDifferential(0.2f);

#else
      pidContorl.setTarget(100);
      pidContorl.setMethod(3);

#endif

#if ISPID
      pidContorl2.setProportion(0.5f);
      pidContorl2.setIntegral(0.3f);
      pidContorl2.setDifferential(0.2f);

#else
      pidContorl2.setTarget(100);
      pidContorl2.setMethod(3);

#endif
      mouse_pos.x = 100;
      mouse_pos.y = 100;
      std::thread([hwnd]() {
        char buffer[0x1000];
          while (is_run)
          {
            ball_pos.x = pidContorl.run(ball_pos.x);
            ball_pos.y = pidContorl2.run(ball_pos.y);
            Sleep(10);

            RECT rc;
            GetClientRect(hwnd, &rc);
            InvalidateRect(hwnd, &rc, TRUE);

            std::sprintf(buffer, "x: %d y: %d(x:%d, y:%d)", ball_pos.x, ball_pos.y, mouse_pos.x, mouse_pos.y);
            SetWindowTextA(hwnd, buffer);

          }
        }).detach();

      return 0;
    }
  case WM_DESTROY:
    is_run = false;
    PostQuitMessage(0);
    return 0;
  
  case WM_KEYDOWN: 
  {
    int VKEY = wParam;
    if (VKEY == VK_DOWN)
    {
      ball_pos.y ++;
    }
    else if (VKEY == VK_UP)
    {
      ball_pos.y--;
    }
    else if (VKEY == VK_LEFT)
    {
      ball_pos.x--;
    }
    else if (VKEY == VK_RIGHT)
    {
      ball_pos.x++;
    }
    RECT rc;
    GetClientRect(hwnd, &rc);
    InvalidateRect(hwnd, &rc, TRUE);
    return 0;
    break;
  }
  case WM_MOUSEMOVE: {
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);
    mouse_pos.x = xPos;
    mouse_pos.y = yPos;

    pidContorl.setTarget(xPos);
    pidContorl2.setTarget(yPos);

    //pidContorl.run(ball_pos.x);
    //pidContorl2.run(ball_pos.y);

    return 0;
  }
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(hwnd, &ps);

    Ellipse(dc, ball_pos.x - 8, ball_pos.y - 8, ball_pos.x + 8, ball_pos.y + 8);

    EndPaint(hwnd, &ps);
    return 0;
  }
  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  // ���崰����
  const char CLASS_NAME[] = "PID Wnd class";
  WNDCLASS wc = {};

  wc.lpfnWndProc = WindowProc;            // ���ô��ڹ��̺���
  wc.hInstance = hInstance;               // ��ǰʵ�����
  wc.lpszClassName = CLASS_NAME;          // ����������
  wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ����Ĭ�Ϲ��
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // ���ñ�����ɫ

  // ע�ᴰ����
  RegisterClass(&wc);

  // ��������
  HWND hwnd = CreateWindowEx(
    0,                              // ��չ������ʽ
    CLASS_NAME,                     // ����������
    "My First Window",              // ���ڱ���
    WS_OVERLAPPEDWINDOW,            // ������ʽ
    CW_USEDEFAULT, CW_USEDEFAULT,   // ���ڳ�ʼλ��
    1248, 768,                       // ���ڳߴ�
    NULL,                           // �����ھ��
    NULL,                           // �˵����
    hInstance,                      // ��ǰʵ�����
    NULL                            // �������
  );

  // ��鴰���Ƿ񴴽��ɹ�
  if (hwnd == NULL) {
    return 0;
  }

  // ��ʾ����
  ShowWindow(hwnd, nCmdShow);

  // ������Ϣѭ��
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}