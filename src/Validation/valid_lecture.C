//#include <vector>

#include <malloc.h>
#include <unistd.h>

#include "lima++.h"
#include "vect3d.h"
#include "erreur.h"

#if defined(_IBMR2) && (__IBMCPP__ < 500)
#include <iostream.h>
#elif defined(CRAY)
#include <iostream.h>
#elif defined(__GNUC__)
#include <iostream.h>
using namespace Lima;
#include <vector>
#include <string>
using std::vector;
using std::string;
#elif defined(__sgi)
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using namespace Lima;
std::ostream& operator << (std::ostream& o, const string& s)
{
  return o << s.c_str();
}
#else
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <vector>
#include <string>
using std::vector;
using std::string;
using namespace Lima;
#endif

int main(int argc, char** argv)
{


  if (argc == 0) {
    cout << "donner un nom de fichier\n" << endl;
    return 1;
  }
  try{
    Maillage mai;

    format_t  form = SUFFIXE;
    size_type num  = 1;
    int iter;

    // Ouverture et lecture du fichier
//    mai.fichier(argv[1],form,num);
    mai.lire(argv[1],form,num);

    cout << endl << " RELECTURE DU MAILLAGE " << endl << endl;

    // Dimension du maillage
    dim_t dim = mai.dimension();
    cout << "Le maillage est " << dim << "D" << endl;

    // Unite de longueur
    double unit_long = mai.unite_longueur();
    cout << "L'unite de longueur est egal a " << unit_long << endl;

    // Systeme de coordonnees
    coordonnee_t syst_coo = mai.type_coordonnee();
    cout << "Le systeme de coordonnees est : " << syst_coo << endl;

    // Geometrie
    geometrie_t geom = mai.type_geometrie();
    cout << "La geometrie est de type " << geom << endl;

    cout << endl << "*********************************************" << endl << endl;

    // Lecture du nombre de noeuds du maillage
    int nbnoe = mai.nb_noeuds();
    cout << "Nombre de noeuds du maillage = " << nbnoe << endl << endl;
    cout << " Noeuds          X           Y           Z" << endl;
    for (iter = 0 ; iter < nbnoe ; iter ++)
      {
	Noeud noe(mai.noeud(iter));
	cout << "    " << iter+1 << "            " << noe.x() << "           " << noe.y() << "           " << noe.z() << endl;
      }
    cout << endl;

    // Lecture des attributs associes aux noeuds



    vector<string> noe_att_list = mai.att_noeuds().liste();
    vector<string>::iterator ater = noe_att_list.begin();
    int long_noe_att_list = noe_att_list.size();

    if (long_noe_att_list == 1)
      {
	cout << "Il y a un unique attribut associe aux noeuds." << endl;
	char* tmp_nom = (char*)ater->c_str();
	cout << "Cet attribut est : '" << tmp_nom << "'" << endl;
      }
    else if (long_noe_att_list > 1)
      {
	cout << "Il y a " << long_noe_att_list << " attributs associes aux noeuds." << endl;
	cout << "Ces attributs sont : " << endl;
	
	int index = 1;
	for (vector<string>::iterator bter = ater ; bter != noe_att_list.end() ; bter++ , index++)
	  {
	    char* tmp_nom = (char*)bter->c_str();
	    cout << "--> L'attribut " << index << " est : '" << tmp_nom << "'" << endl;
	  }
      }
    // Recherche des attributs
    cout << "--> L'attribut de type chaine de caracteres : 'numero'";

    int long_list;
    long_list  = mai.att_noeuds().element("numero").nb_elements();

    if (long_list == 1)
      {
        string tmp_val;
        mai.att_noeuds().element("numero").get_val(tmp_val);
        cout << " est a valeur constante : " << tmp_val << endl;
      }
    else if (long_list > 1)
      {
        cout << " possede " << long_list << " valeurs qui sont : " << endl;

        string tmp_val;
        for (int jter = 0 ; jter < long_list ; jter++)
          {
            mai.att_noeuds().element("numero").get_val(jter, tmp_val);
            cout << tmp_val << " - ";
          }
        cout << endl;
      }
    //
    cout << "--> L'attribut de type vecteur3d : 'coordonnees'";

    int nb_elem;

    nb_elem  = mai.att_noeuds().element("coordonnees").nb_elements();
    if (nb_elem == 1)
      {
	Vect3d tmp_val;
	mai.att_noeuds().element("coordonnees").get_val(tmp_val);
	cout << " est a valeur constante : " << endl;
	cout << "[" << tmp_val.x() << " ; " << tmp_val.y() << " ; " << tmp_val.z() << "]" << endl;
      }
    else if (nb_elem > 1)
      {
	cout << " possede " << nb_elem << " valeurs qui sont : " << endl;
	Vect3d tmp_val;
	//for (int cter = 0 ; cter < nb_elem ; cter++)
	for (int cter = 0 ; cter < nb_elem ; cter++)
	  {
	    mai.att_noeuds().element("coordonnees").get_val(cter, tmp_val);
	    cout << "[" << tmp_val.x() << " ; " << tmp_val.y() << " ; " << tmp_val.z() << "]";
	    cout << endl;
	  }
      }
    //
    cout << endl << "*********************************************" << endl << endl;

    // Lecture du nombre de bras du maillage
    int nbbras = mai.nb_bras();
    cout << "Nombre de bras du maillage = " << nbbras << endl << endl;
    cout << " Bras          Noeud1        Noeud2" << endl;
    for (iter = 0 ; iter < nbbras ; iter ++)
      {
	Noeud noe1(mai.bras(iter).noeud(0));
	Noeud noe2(mai.bras(iter).noeud(1));
	cout << "   " << iter+1 << "             " << noe1.id() << "            " << noe2.id() << endl;
      }

    cout << endl << "*********************************************" << endl << endl;

    // Lecture du nombre de polygones
    int nbpolygo =  mai.nb_polygones();
    cout << "Nombre de polygones du maillage = " << nbpolygo << endl << endl;
    cout << " Polygones     Noeud1     Noeud2     Noeud3     Noeud4" << endl;
    for (iter = 0 ; iter < nbpolygo ; iter++)
      {
	int nombre_noe = mai.polygone(iter).nb_noeuds();
	// Ici on teste les quadrangles
	if (nombre_noe == 4)
	  {
	    Noeud noe1(mai.polygone(iter).noeud(0));
	    Noeud noe2(mai.polygone(iter).noeud(1));
	    Noeud noe3(mai.polygone(iter).noeud(2));
	    Noeud noe4(mai.polygone(iter).noeud(3));
	    cout << "     " << iter+1 << "           " << noe1.id() << "          " << noe2.id() << "          " << noe3.id() << "          " << noe4.id() <<endl; 
	  } 
      }

    cout << endl << "*********************************************" << endl << endl;

    // Lecture du nombre de polyedres
    int nbpolyed =  mai.nb_polyedres();
    cout << "Nombre de polyedres du maillage = " << nbpolyed << endl << endl;
    cout << " Polyedres     Noeud1     Noeud2     Noeud3     Noeud4     Noeud5     Noeud6     Noeud7     Noeud8" << endl;
    for (iter = 0 ; iter < nbpolyed ; iter++)
      {
	int nombre_noe = mai.polyedre(iter).nb_noeuds();
	// Ici on teste les hexadres
	if (nombre_noe == 8)
	  {
	    Noeud noe1(mai.polyedre(iter).noeud(0));
	    Noeud noe2(mai.polyedre(iter).noeud(1));
	    Noeud noe3(mai.polyedre(iter).noeud(2));
	    Noeud noe4(mai.polyedre(iter).noeud(3));
	    Noeud noe5(mai.polyedre(iter).noeud(4));
	    Noeud noe6(mai.polyedre(iter).noeud(5));
	    Noeud noe7(mai.polyedre(iter).noeud(6));
	    Noeud noe8(mai.polyedre(iter).noeud(7));
	    cout << "     " << iter+1 << "           " << noe1.id() << "          " << noe2.id() << "          " << noe3.id() << "          " << noe4.id() << "          " << noe5.id() << "          " << noe6.id() << "          " << noe7.id() << "          " << noe8.id() << endl; 
	  } 
      }

    cout << endl << "*********************************************" << endl << endl;

    // Lecture du nombre de nuages
    int nbnua =  mai.nb_nuages();
    cout << "Nombre de nuages du maillage = " << nbnua << endl << endl;

    for (iter = 0 ; iter <nbnua ; iter++)
      {
	Nuage nu(mai.nuage(iter));
	string nom_nuage = nu.nom();
	cout << "Le nuage '" << nom_nuage.c_str() << "' est constitue des noeuds :" << endl;
	
	int knoe = nu.nb_noeuds();
	for (int jter = 0 ; jter < knoe ; jter++)
	  {
	    Noeud noe(mai.nuage(iter).noeud(jter));
	    cout << noe.id() << "  ";
	  }
	cout << endl;
      }

    cout << endl << "*********************************************" << endl << endl;

    // Lecture du nombre de lignes
    int nbli =  mai.nb_lignes();
    cout << "Nombre de lignes du maillage = " << nbli << endl << endl;

    for (iter = 0 ; iter <nbli ; iter++)
      {
	Ligne li(mai.ligne(iter));
	string nom_ligne = li.nom();
	cout << "La ligne '" << nom_ligne.c_str() << "' est constituee des bras :" << endl;
	
	int kbr = li.nb_bras();
	for (int jter = 0 ; jter < kbr ; jter++)
	  {
	    Bras br(mai.ligne(iter).bras(jter));
	    cout << br.id() << " constitue des noeuds : ";
	    
	    int knoe = br.nb_noeuds();
	    for (int kter = 0 ; kter < knoe ; kter++)
	      {
		Noeud noe(br.noeud(kter));
		cout << noe.id() << "  ";
	      }
	    cout << endl;
	  }
      }

    cout << endl << "*********************************************" << endl << endl;

    // Lecture du nombre de surfaces
    int nbsu =  mai.nb_surfaces();
    cout << "Nombre de surfaces du maillage = " << nbsu << endl << endl;

    for (iter = 0 ; iter <nbsu ; iter++)
      {
	Surface su(mai.surface(iter));
	string nom_surf = su.nom();
	cout << "La surface '" << nom_surf.c_str() << "' est constituee des polygones :" << endl;
	
	int kpg = su.nb_polygones();
	for (int jter = 0 ; jter < kpg ; jter++)
	  {
	    Polygone pg(mai.surface(iter).polygone(jter));
	    cout << pg.id() << " constitue des noeuds : ";
	    
	    int knoe = pg.nb_noeuds();
	    for (int kter = 0 ; kter < knoe ; kter++)
	      {
		Noeud noe(pg.noeud(kter));
		cout << noe.id() << "  ";
	      }
	    cout << endl;
	  }
      }

    cout << endl << "*********************************************" << endl << endl;

    // Lecture du nombre de volumes
    int nbvo =  mai.nb_volumes();
    cout << "Nombre de volumes du maillage = " << nbvo << endl << endl;

    for (iter = 0 ; iter < nbvo ; iter++)
      {
	Volume vo(mai.volume(iter));
	string nom_vol = vo.nom();
	cout << "Le volume '" << nom_vol.c_str() << "' est constitue des polyedres :" << endl;
	
	int kpd = vo.nb_polyedres();
	for (int jter = 0 ; jter < kpd ; jter++)
	  {
	    Polyedre pd(mai.volume(iter).polyedre(jter));
	    cout << pd.id() << " constitue des noeuds : ";
	    
	    int knoe = pd.nb_noeuds();
	    for (int kter = 0 ; kter < knoe ; kter++)
	      {
		Noeud noe(pd.noeud(kter));
		cout << noe.id() << "  ";
	      }
	    cout << endl;
	  }
	cout << endl;
      }


    // Lecture des attributs attaches aux polyedres des volumes
    string nom_groupe = "VOLUME_TOTAL";
    vector<string> polvol_att_list = mai.volume(nom_groupe).att_polyedres().liste();
    vector<string>::iterator pter = polvol_att_list.begin();
    int long_polvol_att_list = polvol_att_list.size();

    if (long_polvol_att_list == 1)
      {
	cout << "Il y a un unique attribut associe aux polyedres du volume." << endl;
	char* tmp_nom = (char*)pter->c_str();
	cout << "Cet attribut est : '" << tmp_nom << "'" << endl;
      }
    else if (long_polvol_att_list > 1)
      {
	cout << "Il y a " << long_polvol_att_list << " attributs associes aux polyedres du volume. " << endl;
	cout << "Ces attributs sont : " << endl;
	
	int index = 1;
	for (vector<string>::iterator qter = pter ; qter != polvol_att_list.end() ; qter++ , index++)
	  {
	    char* tmp_nom = (char*)qter->c_str();
	    cout << "--> L'attribut " << index << " est : '" << tmp_nom << "'" << endl;
	  }
	cout << endl;
      }
    // Recherche des attributs
    cout << "--> L'attribut de type entier : 'numero'";

    nb_elem  = mai.volume(nom_groupe).att_polyedres().element("numero").nb_elements();
    if (nb_elem == 1)
      {
	int tmp_val;
	mai.volume(nom_groupe).att_polyedres().element("numero").get_val(tmp_val);
	cout << " est a valeur constante : " << tmp_val << endl;
      }
    else if (nb_elem > 1)
      {
	cout << " possede " << nb_elem << " valeurs qui sont : " << endl;
	int tmp_val;
	for (int jter = 0 ; jter < nb_elem ; jter++)
	  {
	    mai.volume(nom_groupe).att_polyedres().element("numero").get_val(jter, tmp_val);
	    cout << tmp_val << " - ";
	  }
	cout << endl;
      }
    //
    cout << "--> L'attribut de type reel : 'volume'";
    bool exist = mai.volume(nom_groupe).att_polyedres().contenir("volume");
    if (exist == false)
      {
	cout << " n'existe pas ! " << endl;
      }
    else
      {
	nb_elem = mai.volume(nom_groupe).att_polyedres().element("volume").nb_elements();
	if (nb_elem == 1)
	  {
	    double tmp_val;
	    mai.volume(nom_groupe).att_polyedres().element("volume").get_val(tmp_val);
	    cout << " est a valeur constante : " << tmp_val << endl;
	  }
	else if (nb_elem > 1)
	  {
	    cout << " possede " << nb_elem << " valeurs qui sont : " << endl;
	    double tmp_val;
	    for (int jter = 0 ; jter < nb_elem ; jter++)
	      {
		mai.volume(nom_groupe).att_polyedres().element("volume").get_val(jter, tmp_val);
		cout << tmp_val << " - ";
	      }
	    cout << endl;
	  }
      }
    //
    cout << "--> L'attribut de type chaine de caracteres : 'type_elem'";

    int long_list2;
    long_list2  = mai.volume(nom_groupe).att_polyedres().element("type_elem").nb_elements();

    if (long_list2 == 1)
      {
        string tmp_val2;
        mai.volume(nom_groupe).att_polyedres().element("type_elem").get_val(tmp_val2);
        cout << " est a valeur constante : " << tmp_val2 << endl;
      }
    else if (long_list2 > 1)
      {
        cout << " possede " << long_list2 << " valeurs qui sont : " << endl;

        string tmp_val2;
        for (int jter = 0 ; jter < long_list2 ; jter++)
          {
            mai.volume(nom_groupe).att_polyedres().element("type_elem").get_val(jter, tmp_val2);
            cout << tmp_val2 << " - ";
          }
        cout << endl;
      }

    cout << endl << "*********************************************" << endl << endl;

  }
  catch(erreur& err){
    cerr << err.what() << endl;
  }

  return 0;
}
