#include <malloc.h>
#include <unistd.h>
#include <time.h>

#include "lima++.h"
#include "erreur.h"
#include "config.h"
#include "../tests/io.h"


int main(int argc, char** argv)
{


  if (argc == 1) {
    cout << "donner un nom de fichier\n" << endl;
    return 1;
  }
  try{
    Maillage mai;

    // on associe le fichier au maillage
    format_t  form=SUFFIXE;
    size_type num=1;
//    mai.fichier(argv[1],form,num);
    mai.lire(argv[1],form,num);
   
    cout << "Nombre de noeuds du maillage " << mai.nb_noeuds() << endl;
    cout << "Nombre de bras du maillage " << mai.nb_bras() << endl;
    cout << "Nombre de polygones du maillage " << mai.nb_polygones() << endl;
    cout << "Nombre de polyedres du maillage " << mai.nb_polyedres() << endl;
    cout << "Nombre de nuages du maillage " << mai.nb_nuages() << endl;
    cout << "Nombre de lignes du maillage " << mai.nb_lignes() << endl;
    cout << "Nombre de surfaces du maillage " << mai.nb_surfaces() << endl;
    cout << "Nombre de volumes du maillage " << mai.nb_volumes() << endl;
    
    cout << endl << "Version lima : " << endl ;
    cout << lima_version() << endl;   

    cout << endl << "Formats de lecture : " << endl ;
    int i=0;
    while ( liste_format_lecture()[i] != NULL ) {
      cout << liste_format_lecture()[i] << endl;
      i++;
    }
    cout << endl << "Formats d'ecriture : " << endl;
    i=0;
    while ( liste_format_ecriture()[i] != NULL ) {
      cout << liste_format_ecriture()[i] << endl;
      i++;
    }
    cout << endl;

    if (argc > 2)
      mai.ecrire(argv[2],form,num);

  }
  catch(erreur& err){
    cerr << err.what() << endl;
  }

  return 0;
}

