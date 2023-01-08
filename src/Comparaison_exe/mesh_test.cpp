//
// Tools for checking Lima meshes.
//

#include "mesh_test.h"
#include "Lima/lima++.h"
#include "Lima/erreur.h"
#include "Lima/config.h"
#include "io.h"

extern "C" double fabs (double);
extern "C" double sqrt (double);

#if !defined(_IBMR2) && !defined(CRAY) && !defined(__hpux)
using namespace std;
#else
#if __IBMCPP__ >= 500
using namespace std;
#endif  // #if __IBMCPP__ >= 500
#endif  // #if !defined(_IBMR2) && ...



template <class T> string type2str (T t)
{
	ostrstream	stream;
	stream << setprecision (20) << t << ends;
	return stream.str ( );
}


bool isEqual (double d1, double d2)
{
	if (d1 == d2)
		return true;

	// Rem pour les versions à venir : ne serait-il pas plus judicieux de
	// diviser par sup (fabs (d1), fabs (d2)) ?
	double percent = fabs ((d1 - d2) / (d1 + d2));
	return (percent < 1E-6 ? true : false);
}


ostream& operator << (ostream& o, const Vect2d& v)
{
  return o << '[' << v.x() << ' ' << v.y() << ']';
}

ostream& operator << (ostream& o, const Vect3d& v)
{
  return o << '[' << v.x() << ' ' << v.y() <<
    ' ' << v.z() << ']';
}

void print_attribut(const Attribut& a)
{
  cout << a.nom() << " : " <<
    a.type() << endl;

  if(a.type() == Attribut::ENTIER){
    int val;
    for(int i=0; i<a.nb_elements(); ++i){
      a.get_val(i, val);
      cout << val << " ";
      if(!((i+1)%15)) cout << endl;
    }
    cout << endl;
  }
  if(a.type() == Attribut::CHAINE){
    string buf;
    for(int i=0; i<a.nb_elements(); ++i){
      a.get_val(i, buf);
      cout << buf << " ";
    }
    cout << endl;
  }
  if(a.type() == Attribut::REEL){
    double val;
    for(int i=0; i<a.nb_elements(); ++i){
      a.get_val(i, val);
      cout << val << " ";
      if(!((i+1)%8)) cout << endl;
    }
    cout << endl;
  }
  if(a.type() == Attribut::VECT2D){
    Vect2d val;
    for(int i=0; i<a.nb_elements(); ++i){
      a.get_val(i, val);
      cout << val << " ";
      if(!((i+1)%3)) cout << endl;
    }
    cout << endl;
  }
  if(a.type() == Attribut::VECT3D){
    Vect3d val;
    for(int i=0; i<a.nb_elements(); ++i){
      a.get_val(i, val);
      cout << val << " ";
      if(!((i+1)%2)) cout << endl;
    }
    cout << endl;
  }
}

void print_attributs(const Liste_attribut& a)
{
  for(int i=0; i<a.nb_elements(); ++i)
    print_attribut(a.element(i));
  cout << endl;
}

void compare_attribut(const Attribut& a, const Attribut& b)
{
  if(a.nom() != b.nom())
    throw erreur ("Nom attribut different : " + a.nom() + " - " + b.nom());
  if(a.type() != b.type())
    throw erreur ("Type attribut different : " + type2str (a.type()) + " - " +
	               type2str (b.type()));
  if(a.nb_elements() != b.nb_elements())
    throw erreur ("Nombre elements attribut different : " +
	               type2str (a.nb_elements()) + " - " +
	               type2str (b.nb_elements()));

  if(a.type() == Attribut::ENTIER){
    int valc, vald;
    for(int i=0; i<a.nb_elements(); ++i){
      a.get_val(i, valc);
      b.get_val(i, vald);
      if(valc != vald)
	throw erreur ("Valeur attribut different : " + type2str (valc) + " - " +
	               type2str (vald));
    }
  }
  if(a.type() == Attribut::CHAINE){
    string bufc, bufd;
    for(int i=0; i<a.nb_elements(); ++i){
      a.get_val(i, bufc);
      b.get_val(i, bufd);
      if(bufc != bufd)
	throw erreur ("Valeur attribut different : " + bufc + " - " + bufd);
    }
  }
  if(a.type() == Attribut::REEL){
    double valc, vald;
    for(int i=0; i<a.nb_elements(); ++i){
      a.get_val(i, valc);
      b.get_val(i, vald);
//      if(valc != vald)
		if (false == isEqual (valc, vald))
	throw erreur ("Valeur attribut different : " + type2str (valc) + " - " +
	               type2str (vald));
    }
  }
  if(a.type() == Attribut::VECT2D){
    Vect2d valc, vald;
    for(int i=0; i<a.nb_elements(); ++i){
      a.get_val(i, valc);
      b.get_val(i, vald);
//      if(valc.x() != vald.x() || valc.y() != vald.y() )
      if ((false == isEqual (valc.x(), vald.x())) ||
		    (false == isEqual (valc.y(), vald.y())))
	throw erreur ("Valeur attribut different : (" + type2str (valc.x()) +
	               ", " + type2str (valc.y()) + ") - (" + type2str (vald.x()) +
	               ", " + type2str (vald.y()) + ")");
    }
  }
  if(a.type() == Attribut::VECT3D){
    Vect3d valc, vald;
    for(int i=0; i<a.nb_elements(); ++i){
      a.get_val(i, valc);
      b.get_val(i, vald);
//      if(valc.x() != vald.x() || valc.y() != vald.y() || valc.z() != vald.z())
	if ((false == isEqual (valc.x(), vald.x())) ||
	    (false == isEqual (valc.y(), vald.y())) ||
	    (false == isEqual (valc.z(), vald.z())))
	throw erreur ("Valeur attribut different : (" + type2str (valc.x()) +
	               ", " + type2str (valc.y()) + ", " + type2str (valc.z()) +
	               ") - (" + type2str (vald.x()) + ", " + type2str (vald.y()) +
	               ", " + type2str (vald.z()) + ")");
    }
  }
}

void compare_attributs(const Liste_attribut& a, const Liste_attribut& b)
{
  if (a.nb_elements() != b.nb_elements())
    throw erreur("Nombres d'attributs differents");

  for(int i=0; i<a.nb_elements(); ++i)
    compare_attribut(a.element(i), b.element(i));
}

void compareMeshes (Maillage& mesh1, Maillage& mesh2)
{
  int cell;

    // Check wether the number of nodes are equals
    if (mesh1.nb_noeuds() != mesh2.nb_noeuds())
      throw erreur("Nombres de noeuds differents");

    if (mesh1.nb_bras() != mesh2.nb_bras())
      throw erreur("Nombres de bras differents");

    if (mesh1.nb_polygones() != mesh2.nb_polygones())
      throw erreur("Nombres de faces differents");

    if (mesh1.nb_polyedres() != mesh2.nb_polyedres())
      throw erreur("Nombres de mailles differents");

    if (mesh1.nb_nuages() != mesh2.nb_nuages())
      throw erreur("Nombres de nuages differents");

   if (mesh1.nb_lignes() != mesh2.nb_lignes())
      throw erreur("Nombres de lignes differents");

    if (mesh1.nb_surfaces() != mesh2.nb_surfaces())
      throw erreur("Nombres de surfaces differents");

    if (mesh1.nb_volumes() != mesh2.nb_volumes())
      throw erreur("Nombres de volumes differents");

    if (mesh1.dimension() != mesh2.dimension())
      cout << "Dimensions differentes" << endl;


    //print_attributs(mesh1.att_maillage());
    compare_attributs(mesh1.att_maillage(), mesh2.att_maillage());

    double max_err = 0.0, max_norm = 0.0;
    for (int vtx = 0; vtx < mesh1.nb_noeuds(); vtx++)
      {
	Noeud  n1 = mesh1.noeud(vtx), n2 = mesh2.noeud(vtx);

	double norm = n1.x() * n1.x() + n1.y() * n1.y() + n1.z() * n1.z();

	if (norm > max_norm)
	  max_norm = norm;

	double err = (n1.x() - n2.x())*(n1.x()-n2.x())
	  + (n1.y() - n2.y())*(n1.y()-n2.y())
	  + (n1.z() - n2.z()) * (n1.z() - n2.z());

	if (err > max_err)
	  max_err = err;
      }

    max_err = sqrt(max_err);

    //print_attributs(mesh1.att_noeuds());
    compare_attributs(mesh1.att_noeuds(), mesh2.att_noeuds());

    // Comparaison des Bras
    for (cell = 0; cell < mesh1.nb_bras(); ++cell)
      {
	Bras  poly1 = mesh1.bras(cell),
	   poly2 = mesh2.bras(cell);

	if (poly1.nb_noeuds() != poly2.nb_noeuds())
	  throw erreur ("Nombres de noeuds d'un bras differents");

	for (int vtx = 0; vtx < poly1.nb_noeuds(); ++vtx)
	  if (poly1.noeud(vtx).id () != poly2.noeud(vtx).id ())
	    throw erreur ("Bras different");
      }

    //print_attributs(mesh1.att_bras());
    compare_attributs(mesh1.att_bras(), mesh2.att_bras());

	// Comparaison des Polygones
	for (cell = 0; cell < mesh1.nb_polygones(); ++cell)
	{
		Polygone  poly1 = mesh1.polygone(cell),
		poly2 = mesh2.polygone(cell);

		if (poly1.nb_noeuds() != poly2.nb_noeuds())
			throw erreur ("Nombres de noeuds d'un polygone differents");

		for (int vtx = 0; vtx < poly1.nb_noeuds(); ++vtx)
			if (poly1.noeud (vtx).id ( ) != poly2.noeud (vtx).id ( ))
			{
				ostrstream	msg;
				msg << "Noeuds d'indices differents ("
				    << poly1.noeud (vtx).id ( ) << "-"
				    <<  poly2.noeud (vtx).id ( ) << ") pour les polygones "
				    << poly1.id ( ) << " et " << poly2.id ( ) << ends;
				throw erreur (msg.str ( ));
			}
	}

    //print_attributs(mesh1.att_polygones());
    compare_attributs(mesh1.att_polygones(), mesh2.att_polygones());

	// Comparaison des POlyedres
	for (cell = 0; cell < mesh1.nb_polyedres(); ++cell)
	{
		Polyedre  poly1 = mesh1.polyedre(cell),
		poly2 = mesh2.polyedre(cell);

		if (poly1.nb_noeuds() != poly2.nb_noeuds())
			throw erreur ("Nombres de noeuds d'un polyedre differents");

		for (int vtx = 0; vtx < poly1.nb_noeuds(); ++vtx)
		{
			if (poly1.noeud (vtx).id ( ) != poly2.noeud (vtx).id ( ))
			{
				ostrstream	msg;
				msg << "Noeuds d'indices differents ("
				    << poly1.noeud (vtx).id ( ) << "-"
				    <<  poly2.noeud (vtx).id ( ) << ") pour les polyedres "
				    << poly1.id ( ) << " et " << poly2.id ( ) << ends;
				throw erreur (msg.str ( ));
			}
		}
	}

    //print_attributs(mesh1.att_polyedres());
    compare_attributs(mesh1.att_polyedres(), mesh2.att_polyedres());

    // Comparaison des VOlumes
    for (int vol = 0; vol < mesh1.nb_volumes(); ++vol)
      {
	Volume  vol1 = mesh1.volume(vol),
	   vol2 = mesh2.volume(vol);

	//print_attributs(vol1.att_polyedres());
	compare_attributs(vol1.att_polyedres(), vol2.att_polyedres());

	if (vol1.nb_polyedres() != vol2.nb_polyedres())
	  throw erreur ("Nombres de mailles d'un volume");

	for (int poly = 0; poly < vol1.nb_polyedres(); ++poly)
	  if (vol1.polyedre(poly).id() != vol2.polyedre(poly).id ())
	    throw erreur ("Volume different");
      }

    //print_attributs(mesh1.att_volumes());
    compare_attributs(mesh1.att_volumes(), mesh2.att_volumes());

    // Comparaison des surfaces

    for (int surf = 0; surf < mesh1.nb_surfaces(); ++surf)
      {
	Surface  surf1 = mesh1.surface(surf),
	   surf2 = mesh2.surface(surf);

	//print_attributs(surf1.att_polygones());
	compare_attributs(surf1.att_polygones(), surf2.att_polygones());

	if (surf1.nb_polygones () != surf2.nb_polygones())
	  throw erreur ("Nombres de faces d'une surface differents");

	for (int poly = 0; poly < surf1.nb_polygones(); ++poly)
	  if (surf1.polygone(poly).id() != surf2.polygone(poly).id ())
	    throw erreur ("Surface different");
      }
    //print_attributs(mesh1.att_surfaces());
    compare_attributs(mesh1.att_surfaces(), mesh2.att_surfaces());

    // Comparaison des lignes

    for (int lig = 0; lig < mesh1.nb_lignes(); ++lig)
      {
	Ligne  lig1 = mesh1.ligne(lig),
	   lig2 = mesh2.ligne(lig);

	//print_attributs(lig1.att_bras());
	compare_attributs(lig1.att_bras(), lig2.att_bras());

	if (lig1.nb_bras () != lig2.nb_bras())
	  throw erreur ("Nombres de bras d'une ligne differents");

	for (int poly = 0; poly < lig1.nb_bras(); ++poly)
	  if (lig1.bras(poly).id() != lig2.bras(poly).id ())
	    throw erreur ("Ligne different");
      }
    //print_attributs(mesh1.att_lignes());
    compare_attributs(mesh1.att_lignes(), mesh2.att_lignes());

    // Comparaison des nuages

    for (int nu = 0; nu < mesh1.nb_nuages(); ++nu)
      {
	Nuage  nu1 = mesh1.nuage(nu),
	   nu2 = mesh2.nuage(nu);

	//print_attributs(nu1.att_noeuds());
	compare_attributs(nu1.att_noeuds(), nu2.att_noeuds());

	if (nu1.nb_noeuds () != nu2.nb_noeuds())
	  throw erreur ("Nombres de noeud d'une nuage differents");

	for (int poly = 0; poly < nu1.nb_noeuds(); ++poly)
	  if (nu1.noeud(poly).id() != nu2.noeud(poly).id ())
	    throw erreur ("Nuage different");
      }
    //print_attributs(mesh1.att_nuages());
    compare_attributs(mesh1.att_nuages(), mesh2.att_nuages());

    if (max_err <= (1.0e-4*max_norm))
      cout << "Test reussi" << endl;
    else
      cout << "Max err positionnement noeuds " << max_err/max_norm << endl;
}	// compareMeshes
