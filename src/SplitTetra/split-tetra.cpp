/*
 * Decomposition d'un maillage  en  maillage tetraedrique
 *
 */

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <Lima/lima++.h>

#include "Cell3d.h"



int
main (int argc, char *argv[])
{


  if (argc != 3)
    {
      std::cerr << "SplitTetra V" << Lima::lima_version ( ) << std::endl;

      std::
	cerr << "Transforme un fichier de maillage composé uniquement de polyèdres de type tétraèdres, pyramides, hexaèdres et prismes à base triangulaires"
	     << " en un fichier de maillage composé uniquement de tétraèdres."
	<< std::endl;
      std::
	cerr << "Usage " << argv[0] << " fichier_origine fichier_dest" <<
	std::endl;
      exit (-1);
    }

  try
  {

    Lima::Maillage origine;
    origine.lire (argv[1]);
    if (0 != origine.nb_polygones ())
    {
		std::cerr << "Le maillage doit être dépourvu de mailles polygonales pour être décomposé en tétraèdres." << std::endl;
		exit (-1);
	}
    origine.preparation_parametrable(LM_ORIENTATION|LM_NOEUDS_POLYGONES|LM_POLYEDRES_POLYGONES|LM_POLYGONES_POLYEDRES|LM_NOEUDS_POLYEDRES|LM_COMPACTE);

    int m_nb_vtx = origine.nb_noeuds ();
    int m_nb_elts = origine.nb_polyedres ();
    int m_nb_faces = origine.nb_polygones ();

    Lima::Maillage final;

    Lima::Noeud invalid_node;	// Noeud qui n'appartient pas au maillage


    std::vector < Lima::Noeud > vtx_loc;
    std::vector < Lima::Noeud > face_cg;

    // Copie des Noeuds
    for (int vtxi = 0; vtxi < m_nb_vtx; ++vtxi)
      {
	Lima::Noeud n = origine.noeud (vtxi);
	Lima::Noeud n1;

	if (n.nb_polyedres () != 0)
	  {
	    n1 = Lima::Noeud (n.x (), n.y (), n.z ());
	    final.ajouter (n1);
	  }
	else
	  n1 = invalid_node;


	vtx_loc.push_back (n1);
      }

    std::cout << "Copie des Noeuds .... " << final.nb_noeuds ()
      << " Noeuds " << std::endl;

    // Copie des Barycentres des faces
    for (int facei = 0; facei < m_nb_faces; ++facei)
      {
	Lima::Polygone p = origine.polygone (facei);
        // On ne regarde que les polygones associes a un polyedres

	double xg, yg, zg;
	xg = yg = zg = 0.0;
	Lima::Noeud n (invalid_node);
	unsigned int nb_nodes = p.nb_noeuds ();
	if (p.nb_polyedres() > 0 && nb_nodes > 3)
	  {
	    //  Pour les triangles on retourne un invalid_node
	    for (int vtxi = nb_nodes - 1; vtxi >= 0; --vtxi)
	      {
		Lima::Noeud n1 = p.noeud (vtxi);
		xg += n1.x ();
		yg += n1.y ();
		zg += n1.z ();
	      }
	    double inv = 1.0 / double (nb_nodes);
	    n = Lima::Noeud (xg * inv, yg * inv, zg * inv);
	    final.ajouter (n);

	  }

	face_cg.push_back (n);
      }
    std::cout << "Copie des centres des faces .... " << final.nb_noeuds()
      << " Noeuds " << std::endl;

    // Decomposition en Tetraedres
    // Garde en plus un tableau de type connectivite de Mailles vers Tetra

    std::vector < Lima::Polyedre > tetras;
    std::vector < size_t > ref_tetras (m_nb_elts + 1);
    ref_tetras[0] = 0;

    for (int elti = 0; elti < m_nb_elts; ++elti)
      {
	Lima::Polyedre h = origine.polyedre (elti);
	unsigned int nb_nodes = h.nb_noeuds ();


	ref_tetras[elti + 1] = ref_tetras[elti];
	if (nb_nodes == 4)
	  {
	    Lima::Polyedre tetra (vtx_loc[h.noeud (0).id () - 1],
				  vtx_loc[h.noeud (1).id () - 1],
				  vtx_loc[h.noeud (2).id () - 1],
				  vtx_loc[h.noeud (3).id () - 1]);
	    final.ajouter (tetra);
	    ref_tetras[elti + 1]++;
             tetras.push_back (tetra);

	  }
	else
	  {

	    Cell3d *pCell3d = factoryCell3d (h);
	    Real3 centroid = pCell3d->centroid ();

#ifdef TEST_VOLUME_CENTROID
            Real vol;
            Real3 centr2 = pCell3d->centroidVolume(vol);
            if ((centr2 - centroid).abs() > centroid.abs() * 1e-8)
                std::cout << "Compare " << centroid << " et " << centr2 << std::endl;
#endif
	    delete pCell3d;

	    Lima::Noeud cg (centroid.x, centroid.y, centroid.z);

	    final.ajouter (cg);


	    for (unsigned int facei = 0; facei < h.nb_polygones (); facei++)
	      {

		Lima::Polygone p = h.polygone (facei);
		bool reverse = false;
		if (p.polyedre(0).id() != h.id())
			{
				reverse = true;
			}
		int p_id = p.id () - 1;
		assert (p_id < m_nb_faces);

		if (p.nb_noeuds () == 3)
		  {

		    Lima::Polyedre tetra;
		    if ( ! reverse)
			    tetra = Lima::Polyedre(cg, vtx_loc[p.noeud (0).id () - 1],
					  vtx_loc[p.noeud (1).id () - 1],
					  vtx_loc[p.noeud (2).id () - 1]);
			else
				tetra =  Lima::Polyedre(cg, vtx_loc[p.noeud (0).id () - 1],
					  vtx_loc[p.noeud (2).id () - 1],
					  vtx_loc[p.noeud (1).id () - 1]);
		    final.ajouter (tetra);
		    ref_tetras[elti + 1]++;
		    tetras.push_back (tetra);
		  }
		else
		  {
		    Lima::Noeud fg = face_cg[p_id];
		    for (unsigned int i = 0; i < p.nb_noeuds (); ++i)
		      {
			int next_i = (i + 1) % p.nb_noeuds ();

			Lima::Polyedre tetra;
			if (!reverse )
				tetra = Lima::Polyedre(fg,
					      vtx_loc[p.noeud (next_i).id () - 1],
					      vtx_loc[p.noeud (i).id () - 1], cg);

			else
				tetra = Lima::Polyedre(cg,
				 vtx_loc[p.noeud (next_i).id () - 1],
					      vtx_loc[p.noeud (i).id () - 1], fg);


			final.ajouter (tetra);
			ref_tetras[elti + 1]++;
			tetras.push_back (tetra);
		      }
		  }
	      }
	  }
      }

    std::cout << "Transformation des mailles en  "
      << tetras.size () << " Tetraedres" << std::endl;

    // copie des surfaces
    for (unsigned int surfi = 0; surfi < origine.nb_surfaces (); ++surfi)
      {
	Lima::Surface surf = origine.surface (surfi);
	Lima::Surface n_surf (surf.nom ());
	final.ajouter (n_surf);
	for (unsigned int facei = 0; facei < surf.nb_polygones (); facei++)
	  {
	    Lima::Polygone p = surf.polygone (facei);
	    if (p.nb_noeuds () == 3)
	      {
		Lima::Polygone tri (vtx_loc[p.noeud (0).id () - 1],
				    vtx_loc[p.noeud (1).id () - 1],
				    vtx_loc[p.noeud (2).id () - 1]);
		final.ajouter (tri);
		n_surf.ajouter (tri);
	      }
	    else
	      {
		Lima::Noeud fg = face_cg[p.id () - 1];
		for (unsigned int i = 0; i < p.nb_noeuds (); ++i)
		  {
		    int next_i = (i + 1) % p.nb_noeuds ();
		    Lima::Polygone tri (vtx_loc[p.noeud (i).id () - 1],
					vtx_loc[p.noeud (next_i).id () - 1],
					fg);
		    final.ajouter (tri);
		    n_surf.ajouter (tri);
		  }
	      }
	  }
      }

    // Copie des volumes
    for (unsigned int voli = 0; voli < origine.nb_volumes (); ++voli)
      {
	Lima::Volume vol = origine.volume (voli);
	Lima::Volume nouv_vol (vol.nom ());
	final.ajouter (nouv_vol);
	for (unsigned int poly_i = 0; poly_i < vol.nb_polyedres (); poly_i++)
	  {
	    Lima::Polyedre maille = vol.polyedre (poly_i);

	    for (unsigned int i = ref_tetras[maille.id()-1];
		 i < ref_tetras[maille.id() - 1 + 1]; ++i)
	      {
		Lima::Polyedre nouv = tetras[i];
//		final.ajouter (nouv); Errare JCW est ! Il y est déjà dans le maillage !
		nouv_vol.ajouter (nouv);
	      }
	  }

      }
    final.ecrire (argv[2]);

    // DIagnostiques finaux
    for (unsigned int poly_i = 0; poly_i < origine.nb_polyedres(); ++poly_i)
    {

      for (unsigned int i = ref_tetras[poly_i]; i < ref_tetras[poly_i+1]; i++)
       {
       		Lima::Polyedre nouv = tetras[i];
                if (nouv.volume() < 0.0)
                       std::cout << "Polyedre " << nouv.id() << "  (init " << poly_i << " ) volume negatif? " <<nouv.volume() << std::endl;
       }
    }
    final.preparation_parametrable(LM_NOEUDS_POLYEDRES);

    // Recherche des noeuds orphelins
    for (unsigned int vtxi = 0; vtxi < final.nb_noeuds (); ++vtxi)
      {
	if (final.noeud (vtxi).nb_polyedres () == 0)
	  {
	    std::cout << " Noeud " << final.noeud (vtxi).
	      id () << " sans polyedre" << std::endl;
	  }
      }
  }



  catch (Lima::erreur & exc)
  {
    std::cerr << "Lima reporte l'erreur suivante : "
      << exc.what () << std::endl;
    exit (-1);
  }

}
