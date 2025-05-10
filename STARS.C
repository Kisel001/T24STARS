/* Ivan Pashkov, 10-5, 01.11.2023, IP5 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "STARS.H"
#include "GFX.H"

#define NUM_STARS  1000
#define INFINITY_1 100000.0
#define ProjDist   100.0
#define ProjSize   100.0
#define SIZESTARS  3

#define STR_RGB(R, G, B) (((BYTE)(R) << 16) | ((BYTE)(G) << 8) | (BYTE)(B))

VEC Stars[NUM_STARS];
DBL Speed = 40000;
extern DBL DeltaTime;

INT Ws, Hs;
INT Wp, Hp;
DBL Twist = 1.3;

VOID Swap( VEC *A, VEC *B )
{
  VEC tmp = *A;

  *A = *B;
  *B = tmp;
}

VOID QuickSort( VEC *A, INT N )
{
  INT b = 0, e = N - 1; 
  DBL x = A[N / 2].Z;

  if (N < 2)
    return;

  while (b <= e)
  {
    while (A[b].Z < x)
      b++;
    while (A[e].Z > x)
      e--;

    if (b <= e)
    {
      if (b != e)
        Swap(&A[b], &A[e]);
      b++;
      e--;
    }
  }

  QuickSort(A, e + 1);
  QuickSort(A + b, N - b);
}

static DBL R1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1.0;
}

static DBL R0( VOID )
{
  return (DBL)rand() / RAND_MAX;
}

static VEC StarInit( VOID )
{
  VEC S;
  UCHAR R = rand(), G = rand(), B = rand();

  S.X = R1() * INFINITY_1;
  S.Y = R1() * INFINITY_1;
  S.Z = R0() * INFINITY_1;
  S.RGB = STR_RGB(R, G, B);

  return S;
}

VOID StarsInit( VOID )
{
  INT i;

  for (i = 0; i < NUM_STARS; i++)
    Stars[i] = StarInit();
}

static VOID StarDraw( VEC S )
{
  DBL k;

  DBL xp, yp;
  INT xs, ys;

  xp = S.X * ProjDist / S.Z;
  yp = S.Y * ProjDist / S.Z;

  xs = (INT)(xp * Ws / Wp + Ws / 2);
  ys = (INT)(-yp * Hs / Hp + Hs / 2);

  k = (INFINITY_1 - S.Z) / INFINITY_1;
  
  //GFX_PutPixel(xs, ys, S.RGB);
  GFX_Circle(xs, ys, (INT)(k * SIZESTARS), S.RGB);
}

VOID StarsDraw( VOID )
{
  INT i;

  QuickSort(Stars, NUM_STARS);

  for (i = NUM_STARS - 1; i >= 0; i--)
    StarDraw(Stars[i]);
}

VOID StarsMove( VOID )
{
  INT i;

  for (i = 0; i < NUM_STARS; i++)
  {
    Stars[i].Z -= Speed * DeltaTime;
    if (Stars[i].Z <= 0)
      Stars[i].Z += INFINITY_1;
  }
}

static VOID ProjSet( VOID )
{
  if (Ws >= Hs)
  {
    Wp = ProjSize * Ws / Hs;
    Hp = ProjSize;
  }
  else
  {
    Wp = ProjSize;
    Hp = ProjSize * Hs / Ws;
  }
}

VOID ProjSetSize( INT W, INT H )
{
  Ws = W;
  Hs = H;
  ProjSet();
}

VOID StarsRotate( INT TwistZ, INT TwistX, INT TwistY )
{
  INT i;
  DBL P1 = DeltaTime * Twist * TwistZ;
  DBL P2 = DeltaTime * Twist * TwistX;
  DBL P3 = DeltaTime * Twist * TwistY;
  INT Xtmp, Ytmp, Ztmp;

  for (i = 0; i < NUM_STARS; i++)
  {
    /* Z */
    Xtmp = Stars[i].X;
    Ytmp = Stars[i].Y;

    Stars[i].X = Xtmp * cos(P1) - Ytmp * sin(P1);
    Stars[i].Y = Xtmp * sin(P1) + Ytmp * cos(P1);
    /* X */
    Ytmp = Stars[i].Y;
    Ztmp = Stars[i].Z;

    Stars[i].Z = Ztmp * cos(P2) - Ytmp * sin(P2);
    Stars[i].Y = Ztmp * sin(P2) + Ytmp * cos(P2);
    /* Y */
    Ztmp = Stars[i].Z;
    Xtmp = Stars[i].X;

    Stars[i].Z = Ztmp * cos(P3) - Xtmp * sin(P3);
    Stars[i].X = Ztmp * sin(P3) + Xtmp * cos(P3);
  }
}