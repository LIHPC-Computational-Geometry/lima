#include <malloc.h>
#include <unistd.h>

//#include "size.h"
#include "lima++.h"
#include "erreur.h"

#if defined(_IBMR2) && (__IBMCPP__ < 500)
#include <iostream.h>
#elif defined(CRAY)
#include <iostream.h>
#elif defined(__GNUC__)
#include <iostream.h>
using namespace Lima ;
#elif defined(__sgi)
#include <iostream.h>
using namespace Lima ;
ostream& operator << (ostream& o, const std::string& s)
{
  return o << s.c_str() ;
}
#else
#include <iostream>
using std::cout ;
using std::cerr ;
using std::endl ;
using namespace Lima ;
#endif


int main(int argc, char** argv)
{


  if (argc == 0) {
    cout << "donner un nom de fichier\n" << endl ;
    return 1 ;
  }
  
  try{
    Maillage mai ;

    TypeMasque prepa = LM_POLYGONES_BRAS | LM_SUPPRIMER_DOUBLONS_BRAS | LM_COMPACTE ;
    
    cout << "L'entier 64 bits associe a prepa est : " << prepa << endl ;

    // On associe le fichier au maillage
    format_t  form=SUFFIXE ;
    size_type num=1 ;

//    mai.fichier(argv[1], form, num) ;
    mai.lire(argv[1], form, num) ;
   

    // Quelques valeurs relatives au maillage avant preparation
    cout << "MAILLAGE AVANT PREPARATION " << endl ;
    cout << " Nombre de noeuds du maillage    : " << mai.nb_noeuds()    << endl ;
    cout << " Nombre de bras du maillage      : " << mai.nb_bras()      << endl ;
    cout << " Nombre de polygones du maillage : " << mai.nb_polygones() << endl ;
    
    if (mai.dimension() == D3) {
      cout << " Nombre de polyedres du maillage : " << mai.nb_polyedres() << endl ;
    }
    cout << endl ;

    // Execution de la preparation
    mai.preparation_parametrable(prepa) ;

    // Quelques valeurs relatives au maillage apres preparation
    cout << "MAILLAGE APRES PREPARATION " << endl ;
    cout << " Nombre de noeuds du maillage    : " << mai.nb_noeuds()    << endl ;
    cout << " Nombre de bras du maillage      : " << mai.nb_bras()      << endl ;
    cout << " Nombre de polygones du maillage : " << mai.nb_polygones() << endl ;
    if (mai.dimension() == D3) {
      cout << " Nombre de polyedres du maillage : " << mai.nb_polyedres() << endl ;  
    }
    cout << endl ;
 
  }
  catch(erreur& err){
    cerr << err.what() << endl ;
  }
  
  return 0 ;
}
