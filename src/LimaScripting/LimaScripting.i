// ATTENTION : selon la plate-forme cible, il faut modifier SWIG_FLAGS de 
// maniere a redefinir IN_STD.

%title "Interface Python pour Lima", before
%text %{
  Du a un bug du generateur de doc de SWIG, les vrais noms des constructeurs
    sont pour les polygones Triangle et Quadrangle et pour les polyedres, 
    Tetraedre, Prisme, Pyramide, Hexaedre, Heptaedre et Octoedre. 
%}
%module Lima
%{
//#undef __sun
#include <string>
//#define __sun 1 
#include <Lima/lima++.h>
#include <Lima/erreur.h>
#ifndef __hpux
using namespace Lima; 
#endif	// __hpux
%}


%include "exception.i"
%include Lima/lima.h

%exception{	// v 6.36.0
  try{
    $function
  }
  catch(erreur& e){
    char buf [4096];
	const unsigned long	length	= e.length ( ) < 4096 ? e.length ( ) : 4095;
	memset (buf, '\0', 4095);
    strncpy (buf, e.what ( ), length - 1);
    buf [4095]	= '\0';
    SWIG_exception_fail (SWIG_RuntimeError, buf);
  } 
}

%typemap(in) const string& {
  if(!PyString_Check($1)) {
    PyErr_SetString(PyExc_TypeError, "not a string");
    return NULL;
  }
  $1 = new IN_STD string(PyString_AsString($1));
}

%typemap(freearg) const IN_STD string& {
  delete $1;
}

%typemap(out) const IN_STD string& {
  $result = PyString_FromString($1->c_str());
}

%typemap(out) IN_STD string {
  $result = PyString_FromString($1.c_str());
}

//%include stddef.h
%include Lima/size.h
%include Lima/lima_types.h
%include Lima/enum.h
%include Lima/vect2d.h 
%include Lima/vect3d.h 
%include Lima/attribut.h
%include Lima/liste_attribut.h
%include Lima/noeud.h
%include Lima/bras.h
%include Lima/polygone.h
%include Lima/polyedre.h
%include Lima/nuage.h
%include Lima/ligne.h
%include Lima/surface.h
%include Lima/volume.h
%include Lima/maillage.h

%exception; 	// v 6.36.0


%extend Noeud {
  // affiche numero du noeud et coordonnees
  void afficher() {
    printf("Noeud %d, pos [%g, %g, %g]\n", 
	   self->id(), self->x(), self->y(), self->z());
  };
};

%extend Bras {
  // affiche numero du bras et id des noeuds
  void afficher() {
    printf("Bras %d, noeuds [%d, %d]\n", 
	   self->id(), self->noeud(0).id(), self->noeud(1).id());
  };
};

%extend Polygone {
  // affiche numero du polygone et id des noeuds
  void afficher() {
    printf("Polygone %d, noeuds [%d", self->id(), self->noeud(0).id());
    for(int i=1; i<self->nb_noeuds(); ++i)
      printf(", %d", self->noeud(i).id());
    printf("]\n");
  };
};

%extend Polyedre {
  // affiche numero du polyedre et id des noeuds
  void afficher() {
    printf("Polyedre %d, noeuds [%d", self->id(), self->noeud(0).id());
    for(int i=1; i<self->nb_noeuds(); ++i)
      printf(", %d", self->noeud(i).id());
    printf("]\n");
  };
};

%extend Maillage {
  // Retourne une chaine de caracteres indiquant la geometrie du maillage
  IN_STD string str_type_geometrie ( ) 
  {
	IN_STD	string	result	= "NON DEFINI";
	switch (self->type_geometrie ( ))
	{
		case NORMAL			: result	= "NORMAL";			break;
		case AXISYMETRIQUE	: result	= "AXISYMETRIQUE";	break;
		case PERIODIQUE		: result	= "PERIODIQUE";		break;
	}
	return result;
  };
};

%extend Maillage 
{
	// Retourne le nombre de connexions bras -> noeuds
	size_type nb_connexions_bras_noeuds ( ) const
	{
		size_type	nb_connexions	= 0;

		for(size_type b = 0 ; b < self->nb_bras ( ); b++)
		{
			Bras	bras		= self->bras (b);
			nb_connexions		+= bras.nb_noeuds ( );
		}	// for(size_type b = 0 ; b < self->nb_bras ( ); b++)++)

		return nb_connexions;
	};

	// Retourne le nombre de connexions polygones -> noeuds
	size_type nb_connexions_polygones_noeuds ( ) const
	{
		size_type	nb_connexions	= 0;

		for(size_type p = 0 ; p < self->nb_polygones ( ); p++)
		{
			Polygone	polygone	= self->polygone (p);
			nb_connexions		+= polygone.nb_noeuds ( );
		}	// for(size_type p = 0 ; p < self->nb_polygones ( ); p++)

		return nb_connexions;
	};

	// Retourne le nombre de connexions polyedres -> noeuds
	size_type nb_connexions_polyedres_noeuds ( ) const
	{
		size_type	nb_connexions	= 0;

		for(size_type p = 0 ; p < self->nb_polyedres ( ); p++)
		{
			Polyedre	polyedre	= self->polyedre (p);
			nb_connexions		+= polyedre.nb_noeuds ( );
		}	// for(size_type p = 0 ; p < self->nb_polyedres ( ); p++)

		return nb_connexions;
	};

	// Retourne le nombre de connexions nuages -> noeuds
	size_type nb_connexions_nuages_noeuds ( ) const
	{
		size_type	nb_connexions	= 0;

		for(size_type n = 0 ; n < self->nb_nuages ( ); n++)
		{
			Nuage		nuage	= self->nuage (n);
			nb_connexions		+= nuage.nb_noeuds ( );
		}	// for(size_type n = 0 ; n < self->nb_nuages ( ); n++)

		return nb_connexions;
	};

	// Retourne le nombre de connexions lignes -> bras
	size_type nb_connexions_lignes_bras ( ) const
	{
		size_type	nb_connexions	= 0;

		for(size_type l = 0 ; l < self->nb_lignes ( ); l++)
		{
			Ligne		ligne	= self->ligne (l);
			nb_connexions		+= ligne.nb_bras ( );
		}	// for(size_type l = 0 ; l < self->nb_lignes ( ); l++)

		return nb_connexions;
	};

	// Retourne le nombre de connexions surfaces -> polygones
	size_type nb_connexions_surfaces_polygones ( ) const
	{
		size_type	nb_connexions	= 0;

		for (size_type s = 0 ; s < self->nb_surfaces ( ); s++)
		{
			Surface		surface	= self->surface (s);
			nb_connexions		+= surface.nb_polygones ( );
		}	// for(size_type s = 0 ; s < self->nb_surfaces ( ); s++)

		return nb_connexions;
	};

	// Retourne le nombre de connexions volumes -> polyedres
	size_type nb_connexions_volumes_polyedres ( ) const
	{
		size_type	nb_connexions	= 0;

		for (size_type v = 0 ; v < self->nb_volumes ( ); v++)
		{
			Volume		volume	= self->volume (v);
			nb_connexions		+= volume.nb_polyedres ( );
		}	// for (size_type v = 0 ; v < self->nb_volumes ( ); v++)

		return nb_connexions;
	};
};

%extend Maillage {
  // affiche le noms des nuages
  void lister_nuages(){
    for(int i=0; i<self->nb_nuages(); ++i)
      printf("%s\n", self->nuage(i).nom().c_str());    
  };
};

%extend Maillage {
  // affiche le noms des lignes
  void lister_lignes(){
    for(int i=0; i<self->nb_lignes(); ++i)
      printf("%s\n", self->ligne(i).nom().c_str());    
  };
};

%extend Maillage {
  // affiche le noms des surfaces
  void lister_surfaces(){
    for(int i=0; i<self->nb_surfaces(); ++i)
      printf("%s\n", self->surface(i).nom().c_str());    
  };
};

%extend Maillage {
  // affiche le noms des volumes
  void lister_volumes(){
    for(int i=0; i<self->nb_volumes(); ++i)
      printf("%s\n", self->volume(i).nom().c_str());    
  };
};

// NOUVEAU :
%extend Maillage {
	void preparation_parametrable (long high, long low)
	{
		TypeMasque	prepa	= (TypeMasque)high;
		prepa <<= 32;
		prepa				|= (TypeMasque)low;
		self->preparation_parametrable (prepa);
	};	// preparation_parametrable
};

%section "Exemples", pre
%text %{
  from Lima import *
  # lecture
  m = Maillage()
  m.lire("puits.unv")
  m.preparer(CODE_T)
  # informations
  print m.nb_noeuds()
  print m.nb_polyedres()
  print m.nb_polygones()
  print m.nb_volumes()
  # un noeud
  n =  m.noeud(0)
  print n.x()
  print n.y()
  print m.noeud(0).z()
  n.afficher()

  v =  m.volume_id(0)
  print v.nom()
  v.nommer("VOL0")

  for i in range(0, m.nb_volumes()):
    v = m.volume_id(i)
    print v.nom(), v.nb_polyedres()

  # sauvegarde
  m.ecrire("puits.mli")
%}
