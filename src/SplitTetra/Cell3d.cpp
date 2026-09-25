//
// Les fonctions non triviales de Cell3D
//

#include "Cell3d.h"

//
void
Tetra::splitTetra ()
{
  if (m_tetras.size () != 0)
    return;
  m_tetras.push_back (*this);
}


// Toutes les informations sur les connectivites sont ici

static
std::vector < int >
face_triangle (int a0, int a1, int a2)
{
  std::vector < int >temp;
  temp.push_back (a0);
  temp.push_back (a1);
  temp.push_back (a2);
  return temp;
}


static
std::vector < int >
face_quadrangle (int a0, int a1, int a2, int a3)
{
  std::vector < int >temp;
  temp.push_back (a0);
  temp.push_back (a1);
  temp.push_back (a2);
  temp.push_back (a3);
  return temp;
}

void
Tetra::computeFaces ()
{
  m_faces_id.resize (0);
  m_faces_id.push_back (face_triangle (0, 1, 3));
  m_faces_id.push_back (face_triangle (0, 3, 2));
  m_faces_id.push_back (face_triangle (0, 2, 1));
  m_faces_id.push_back (face_triangle (1, 2, 3));
}

void
Pyramid::computeFaces ()
{
  m_faces_id.resize (0);

  m_faces_id.push_back (face_triangle (0, 1, 4));
  m_faces_id.push_back (face_triangle (0, 4, 3));
  m_faces_id.push_back (face_triangle (3, 4, 2));
  m_faces_id.push_back (face_quadrangle (0, 3, 2, 1));
}

void
Wedge::computeFaces ()
{
  m_faces_id.resize (0);
  m_faces_id.push_back (face_triangle (0, 2, 1));
  m_faces_id.push_back (face_triangle (3, 4, 5));
  m_faces_id.push_back (face_quadrangle (0, 3, 5, 2));
  m_faces_id.push_back (face_quadrangle (0, 1, 4, 3));
  m_faces_id.push_back (face_quadrangle (1, 2, 5, 4));
}

void
Hexa::computeFaces ()
{
  m_faces_id.resize (0);
  m_faces_id.push_back (face_quadrangle (0, 1, 5, 4));
  m_faces_id.push_back (face_quadrangle (0, 3, 2, 1));
  m_faces_id.push_back (face_quadrangle (0, 4, 7, 3));
  m_faces_id.push_back (face_quadrangle (1, 2, 6, 5));
  m_faces_id.push_back (face_quadrangle (3, 7, 6, 2));
  m_faces_id.push_back (face_quadrangle (4, 5, 6, 7));
}


// Decomposition d'une maille 3D en tetraedres

void
Cell3d::splitTetra ()
{

  // On ne le fait que si ce n'est pas déjà fait
 
  if (m_tetras.size () != 0)
    return;

  // On calcule un centre de maille fictif
  Real3 cell_center = Real3::null ();
  for (unsigned int i = 0; i < m_pos.size (); ++i)
    cell_center += m_pos[i];
  cell_center = cell_center / double (m_pos.size ());
  
  if (m_faces_id.size() == 0)
        computeFaces();

  for (std::vector < std::vector < int > >::const_iterator it =
       m_faces_id.begin (); it != m_faces_id.end (); it++)
    {
      const std::vector < int >face_id = *it;
      // Une face triangulaire donne
      // Un tetraedre avec le "centre de la maille"
      if (face_id.size () == 3)
	{
	  m_tetras.push_back (Tetra (cell_center, m_pos[face_id[0]],
				     m_pos[face_id[1]], m_pos[face_id[2]]));
	}
      else
	{
	  // Une face polygonale donne 
	  // n tetraedres bases sur les cotes, le centre de la face et
	  // le centre de la maille

	  Real3 face_center = Real3::null ();
	  for (unsigned int i = 0; i < face_id.size (); ++i)
	    face_center += m_pos[face_id[i]];
	  face_center = 0.25 * face_center;
	  for (unsigned int i = 0; i < face_id.size (); i++)
	    {
	      unsigned int next_i = (i + 1) % face_id.size ();
	     	Tetra t ( m_pos[face_id[next_i]],m_pos[face_id[i]],
					 face_center, cell_center);
		 m_tetras.push_back (t);
	    }
	}
    }
}


Real
Cell3d::volume (void)
{
  Real vol = 0.0;
  if (m_tetras.size () == 0)
    splitTetra ();
  for (std::vector < Tetra >::const_iterator tet = m_tetras.begin ();
       tet != m_tetras.end (); ++tet)
    {
      vol += (*tet).volume ();	// tet->volume() does not work with all compilers
    }
  return vol;
}

Real3
Cell3d::centroid (void)
{
  Real3 xg (Real3::null ());
  Real vol = 0.0;
  if (m_tetras.size () == 0)
    splitTetra ();
  for (std::vector < Tetra >::const_iterator tet = m_tetras.begin ();
       tet != m_tetras.end (); ++tet)
    {
      Real vol0 = (*tet).volume ();
      Real3 local_cent = (*tet).centroid ();
      xg += vol0 * local_cent;
      vol += vol0;
    }
  if (vol != 0.0)
    xg = xg * (1.0 / vol);

  return xg;
}

//
// Following Polyhedral Mass Properties (Revisited)
// David Eberly, Magic Software Inc
//

inline void
SubExpression (Real w0, Real w1, Real w2, 
                      Real & f1, Real & f2)
{
   Real temp0 = w0 + w1;
   f1 = temp0 + w2;
   
   f2 = w0 * w0  + w1 * temp0 + w2 * f1;
}
 
inline void
triangleContribution(Real3 pt0, Real3 pt1, Real3 pt2,
                     Real  & intg0, Real & intg1, Real & intg2, Real & intg3)
{
      Real x0 = pt0.x, y0 = pt0.y, z0 = pt0.z;
      Real x1 = pt1.x, y1 = pt1.y, z1 = pt1.z;
      Real x2 = pt2.x, y2 = pt2.y, z2 = pt2.z;
      
      Real f1x,f2x,f1y,f2y,f1z,f2z;
       
      Real3 normale;
      normale =  (vecMul( pt1 - pt0, pt2 - pt0));
      
      SubExpression(x0,x1,x2,f1x,f2x);
      SubExpression(y0,y1,y2,f1y,f2y);
      SubExpression(z0,z1,z2,f1z,f2z);
      
      intg0 += normale.x * f1x;
      
      intg1 += normale.x * f2x;
      intg2 += normale.y * f2y;
      intg3 += normale.z * f2z;
      
}                     

Real3
Cell3d::centroidVolume(Real& vol)
{
   Real intg0 = 0.0, intg1 = 0.0, intg2 = 0.0, intg3 = 0.0;   
   vol = 0.0;
   if (m_tetras.size() == 0)
      splitTetra();
    for (std::vector < std::vector < int > >::const_iterator it =
       m_faces_id.begin (); it != m_faces_id.end (); it++)
    {
      const std::vector < int >face_id = *it;
      if (face_id.size() == 3) {
        triangleContribution(m_pos[face_id[0]], m_pos[face_id[1]], m_pos[face_id[2]],
                intg0, intg1, intg2, intg3);
      }
      else {    
          Real3 face_center = Real3::null ();
            for (unsigned int i = 0; i < face_id.size (); ++i)
               face_center += m_pos[face_id[i]];
            face_center = (1.0 / face_id.size() ) * face_center;
            for (unsigned int i = 0; i < face_id.size (); i++)
            {
              unsigned int next_i = (i + 1) % face_id.size ();
              triangleContribution(m_pos[face_id[i]], m_pos[face_id[next_i]], face_center,
                              intg0, intg1, intg2, intg3);
            }
    }
    }
    vol = intg0 * (1.0 / 6.0);
    return Real3(intg1, intg2, intg3) * (1.0 / 24.0) / vol;
}
//
// Une petite usine a maille qui transforme le type Lima++ dans un type utilisable
//

Cell3d *
factoryCell3d (const Lima::Polyedre & p)
{
  std::vector < Real3 > pos;
  unsigned int nb_node = p.nb_noeuds ();
  for (unsigned int i = 0; i < nb_node; ++i)
    {
      Lima::Noeud n = p.noeud (i);
      pos.push_back (Real3 (n.x (), n.y (), n.z ()));
    }
  switch (nb_node)
    {
    case 4:
      return new Tetra (pos);
    case 5:
      return new Pyramid (pos);
    case 6:
      return new Wedge (pos);
    case 8:
      return new Hexa (pos);
    default:
		throw Lima::erreur ("split_tetra ne supporte que les tétraèdres, pyramides, hexaèdres et prismes à base triangulaire.");
    }
  return 0;
}

Cell3d::Cell3d(void)
{}

Cell3d::~Cell3d(void)
{}

