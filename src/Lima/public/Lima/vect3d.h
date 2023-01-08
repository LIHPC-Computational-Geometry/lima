#ifndef _VECT3D_H
#define _VECT3D_H


#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

// Vecteur 3D 
class Vect3d
{
public:
  // Constructeur.
  Vect3d(double x=0, double y=0, double z=0);
  
  // Lecture de la coordonnée X du vecteur.
  double        x() const;
  // Lecture de la coordonnée Y du vecteur. 
  double        y() const;
  // Lecture de la coordonnée Z du vecteur. 
  double        z() const;
  // Lecture de la ieme composante du vecteur
#ifdef SWIG
  %name(get_coo)
#endif
  double        coo(size_type i) const;
    
  // Mise a jour de la coordonnée X du vecteur.
  void          set_x(double d);
  // Mise a jour de la coordonnée Y du vecteur. 
  void          set_y(double d);
  // Mise a jour de la coordonnée Z du vecteur. 
  void          set_z(double d);
  // Mise a jour de la ieme composante du vecteur
#ifdef SWIG
  %name(set_coo)
#endif
  void          coo(size_type i, double d);

private:
  double        m_coo[3];
};

#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
