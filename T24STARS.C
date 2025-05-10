/* Ivan Pashkov, 10-5, 01.11.2023, IP5 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <time.h>
#include <commondf.h>

#include "GFX.H"
#include "STARS.H"

HWND hWnd;
DOUBLE SyncTime, FPS, DeltaTime;
static INT IsPause = 0, TwistX = 0, TwistY = 0, TwistZ = 0, MotionState = 0; 
INT Zoom = 1;

VOID Reshape( INT NewW, INT NewH );

VOID Timer( VOID )
{
  LONG t;
  static LONG StartTime = -1, FPSTime, FrameCount = 0, PauseTime = 0, OldTime;
 
  t = clock();
  if (StartTime == -1)
    StartTime = FPSTime = OldTime = t;

  //SyncTime = (t - StartTime) / (double)CLOCKS_PER_SEC;

  if (IsPause)
  {
    DeltaTime = 0;
    PauseTime += t - OldTime;
  }

  else
  {
    DeltaTime = (t - OldTime) / (DOUBLE)CLOCKS_PER_SEC;
    SyncTime = (t - PauseTime - StartTime) / (DOUBLE)CLOCKS_PER_SEC;
  }

  FrameCount++;
  if (t - FPSTime > 0.3 * CLOCKS_PER_SEC)
  {
    FPS = FrameCount / ((t - FPSTime) / (DOUBLE)CLOCKS_PER_SEC);
    FPSTime = t;
    FrameCount = 0;
  }
  OldTime = t;
}

VOID Display( VOID )
{
  CHAR Buf[102];

  if (MotionState == 0)
  {
    TwistX = 0;
    TwistY = 0;
    TwistZ = 0;
  }

  MotionState = 0;

  Timer();
  sprintf(Buf, "CGSG IP5 :: Super FPS: %.3f", FPS);
  SetWindowText(hWnd, Buf);

  GFX_ClearBlack();
  StarsMove();

  if ((GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0)
    TwistZ--;
  if ((GetAsyncKeyState(VK_RCONTROL) & 0x8000) != 0)
    TwistZ++;

  if ((GetAsyncKeyState(VK_UP) & 0x8000) != 0)
    TwistX--;
  if ((GetAsyncKeyState(VK_DOWN) & 0x8000) != 0)
    TwistX++;

  if ((GetAsyncKeyState(VK_LEFT) & 0x8000) != 0)
    TwistY++;
  if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0)
    TwistY--;

  StarsRotate(TwistZ, TwistX, TwistY);
  StarsDraw();
  //GFX_PrintTime();
  GFX_DrawFrame();

  glFinish();
  glutSwapBuffers();
  glutPostRedisplay();
}

VOID Keyboard( BYTE Key, INT x, INT y )
{
  /* EXIT FROM PROGRAMM IF KEY = ESC */
  if (Key == 27)
    exit(0);

  if (Key == 'p' || Key == 'P')
    IsPause = ((IsPause == 1) ? 0 : 1);
}

VOID Reshape( INT NewW, INT NewH )
{
  INT WinW = NewW;
  INT WinH = NewH;

  glViewport(0, 0, NewW, NewH);
  GFX_FrameSetSize(WinW, WinH);
  ProjSetSize(GFX_W, GFX_H);
}

VOID Mouse( INT X, INT Y )
{
  static OldX, OldY, flag = 1;
    
  if (flag)
  {    
    OldX = X;
    OldY = Y;
    flag = 0;
  }

  TwistY = (X - OldX) / 10;
  TwistX = (OldY - Y) / 10;
  OldX = X;
  OldY = Y;

  MotionState = 1;
}

VOID main( INT argc, CHAR *argv[] )
{
  FILE *F;

  if ((F = fopen("MyFont.fnt", "rb")) == NULL)
    ;

  /* Initialization */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  /* Create Window */
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(800, 800);
  glutCreateWindow("CGSG STARS");
  hWnd = FindWindowA(NULL, "CGSG STARS");

  /* Base Draw */
  GFX_Init();
  ProjSetSize(GFX_W / Zoom, GFX_H / Zoom);
  StarsInit();
  
  GFX_LoadFont();

  /* Set callback functions */
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  glutReshapeFunc(Reshape);
  glutMotionFunc(Mouse);

  /* Cycle */
  glClearColor(0.30, 0.47, 0.8, 1);
  glutMainLoop();
}