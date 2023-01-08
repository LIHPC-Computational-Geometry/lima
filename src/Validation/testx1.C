#include <malloc.h>
#include <unistd.h>

#include "size.h"
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
    Maillage mai1, mai2 ;

    TypeMasque prepa_test = LM_ORIENTATION|LM_SUPPRIMER_DOUBLONS_POLYGONES|
      LM_POLYGONES_POLYEDRES|LM_POLYEDRES_POLYGONES|LM_NOEUDS_POLYGONES|
      LM_NOEUDS_POLYEDRES|LM_SURFACES_NOEUDS|LM_VOLUMES_NOEUDS|LM_COMPACTE ;
    preparation_t prepa_ref = CODE_T ;
    
    cout << "L'entier 64 bits associe a prepa_test est : " << prepa_test << endl ;

    // On associe le fichier au maillage
    format_t  form=SUFFIXE ;
    size_type num=1 ;

//    mai1.fichier(argv[1], form, num) ;
    mai1.lire(argv[1], form, num) ;
//    mai2.fichier(argv[1], form, num) ;
    mai2.lire(argv[1], form, num) ;
   
    // Execution des preparations
    mai1.preparation_parametrable(prepa_test) ;
    mai2.preparer(prepa_ref)  ;
    

    // Quelques valeurs relatives a la preparation 1
    cout << "PREPARATION 1 " << endl ;
    cout << " Nombre de noeuds du maillage    : " << mai1.nb_noeuds()    << endl ;
    cout << " Nombre de bras du maillage      : " << mai1.nb_bras()      << endl ;
    cout << " Nombre de polygones du maillage : " << mai1.nb_polygones() << endl ;
    
    if (mai1.dimension() == D3) {
      cout << " Nombre de polyedres du maillage : " << mai1.nb_polyedres() << endl ;
    }
    cout << endl ;


    // Quelques valeurs relatives a la preparation 2
    cout << "PREPARATION 2 " << endl ;
    cout << " Nombre de noeuds du maillage    : " << mai2.nb_noeuds()    << endl ;
    cout << " Nombre de bras du maillage      : " << mai2.nb_bras()      << endl ;
    cout << " Nombre de polygones du maillage : " << mai2.nb_polygones() << endl ;
    if (mai1.dimension() == D3) {
      cout << " Nombre de polyedres du maillage : " << mai2.nb_polyedres() << endl ;  
    }
    cout << endl ;

    
    // Test de conformite des preparations 1 & 2
    cout << "Test de conformite des preparations 1 & 2 ... " << endl ;
    Polygone pg1(mai1.polygone(0)) ;
    Polygone pg2(mai2.polygone(0)) ;
    
    for (int iter = 0 ; iter < mai1.nb_polygones() ; iter++) {
      pg1 = mai1.polygone(iter) ;
      pg2 = mai2.polygone(iter) ;
      
      for (int jter = 0 ; jter < pg1.nb_noeuds() ; jter++) {
        if (pg1.noeud(jter).id() != pg2.noeud(jter).id()) {
          cout << " POLYGONE->NOEUD_ERREUR !!! " << endl ;
          return -1 ;
        }
      }
    }
    
    if (mai1.dimension() == D3) {
      Polyedre pd1(mai1.polyedre(0)) ;
      Polyedre pd2(mai2.polyedre(0)) ;
      
      for (int kter = 0 ; kter < mai1.nb_polyedres() ; kter++) {
        pd1 = mai1.polyedre(kter) ;
        pd2 = mai2.polyedre(kter) ;
        
        for (int pter = 0 ; pter < pd1.nb_noeuds() ; pter++) {
          if (pd1.noeud(pter).id() != pd2.noeud(pter).id()) {
            cout << " POLYEDRE->NOEUD_ERREUR !!! " << endl ;
            return -1 ;
          }
        }
      }
    }
    
    cout << "Test de conformite reussi ;-) " << endl ;
    
  }
  catch(erreur& err){
    cerr << err.what() << endl ;
  }
  
  return 0 ;
}
