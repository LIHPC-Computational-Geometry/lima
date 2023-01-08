//
// Compare two Mesh Files using Lima


#include "Lima/lima++.h"
#include "Lima/erreur.h"
#include "Lima/config.h"
#include "io.h"
#include "mesh_test.h"

#include <stdlib.h>


#if !defined(_IBMR2) && !defined(CRAY) && !defined(__hpux)
using namespace Lima;
#else
#if __IBMCPP__ >= 500
using namespace Lima;
#endif  // #if __IBMCPP__ >= 500
#endif  // #if !defined(_IBMR2) && ...


int main (int argc, char * argv[])
{
  int cell;
  int meshNum	= 1;

  if (argc < 3)
    {
      cerr << "Usage " << argv[0] << " File1 File2" << endl;
      exit(-1);
    }
  if (argc >= 4)
     meshNum	= atoi (argv [3]);

  try {
    Maillage lima1;
    lima1.lire(argv[1], SUFFIXE, meshNum);
    lima1.preparer(CODE_A);

    Maillage lima2;
    lima2.lire(argv[2], SUFFIXE, meshNum);
    lima2.preparer(CODE_A);

	compareMeshes (lima1, lima2);

    return 0;
  }
  catch(const erreur& exc)
	{
	  cerr << "Lima reporte l'erreur suivante : " << exc.what() << endl;
//	  exit(-1);
	}
/* Commente pour les tests de nr.
  catch (const exception& e)
  {
	cerr << "Exception : " << e.what ( ) << endl;
  }
*/
  catch(...)
    {
      cerr << "Exception inconnue " << endl;
      exit(-1);
    }

  return 0;
}
