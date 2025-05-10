/* Ivan Pashkov, 10-5, 01.11.2023, IP5 */
#include <commondf.h>

typedef double DBL;

typedef struct tagVEC
{
  DBL X, Y, Z;
  DWORD RGB;
} VEC;

VOID StarsInit( VOID );
VOID ProjSetSize( INT W, INT H );
VOID StarDraw( VEC S );
VOID StarsMove( VOID );
VOID ProjSetSize( INT W, INT H );
VOID StarsRotate( INT TwistZ, INT TwistX, INT TwistY );