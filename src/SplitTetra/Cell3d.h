//
// Une maille 3D prête à la décomposition
// Calcul des volumes et centres de masses
//

#ifndef CELL3D_H
#define CELL3D_H

#include "Lima/lima++.h"
#include "Real3.h"
#include <cassert>

class Tetra;
class Hexa;
class Wedge;
class Pyramid;

class Cell3d {

protected:
  Real3 m_xg;
  std::vector<Real3> m_pos;

  // Les definitions des faces de la maille
  std::vector<std::vector<int> > m_faces_id;

  // Une decomposition de la maille en Tetraedre
  std::vector<Tetra> m_tetras;

  Real m_vol;

  virtual void splitTetra();
  virtual void computeFaces() = 0;

  Cell3d(void);

public:
  virtual ~Cell3d(void);

  virtual Real3 centroidVolume(Real &);
  virtual Real3 centroid(void);
  virtual Real volume(void);

};

Cell3d * factoryCell3d(const Lima::Polyedre &);

class Tetra: public Cell3d {

  void computeFaces();
  void splitTetra();

public:

  Tetra(Real3 p0, Real3 p1, Real3 p2, Real3 p3) {
    m_pos.resize(0);
    m_pos.push_back(p0);
    m_pos.push_back(p1);
    m_pos.push_back(p2);
    m_pos.push_back(p3);
  }

  Tetra(const Tetra & tet) {
    m_pos = tet.m_pos;
  }

  Tetra() {
  }

  Tetra(std::vector<Real3> t) {
    assert(t.size() == 4);
    m_pos = t;
  }

  Real volume(void) const {
    return mixteMul(m_pos[1] - m_pos[0], m_pos[2] - m_pos[0], m_pos[3]
	- m_pos[0]);
  }

  virtual Real volume(void) {
    return mixteMul(m_pos[1] - m_pos[0], m_pos[2] - m_pos[0], m_pos[3]
	- m_pos[0]);
  }
  Real3 centroid(void) const {
    return 0.25 * (m_pos[0] + m_pos[1] + m_pos[2] + m_pos[3]);
  }
  virtual Real3 centroid(void)  {
    return 0.25 * (m_pos[0] + m_pos[1] + m_pos[2] + m_pos[3]);
  }
  ~Tetra() {
  }

};

class Hexa: public Cell3d {
  void computeFaces();

public:
  Hexa(std::vector<Real3> t) {
    assert(t.size() == 8);
    m_pos = t;
  }
  ~Hexa() {
  }
};

class Wedge: public Cell3d {
  void computeFaces();

public:
  Wedge(std::vector<Real3> t) {
    assert(t.size() == 6);
    m_pos = t;
  }
  ~Wedge() {
  }
};

class Pyramid: public Cell3d {
  void computeFaces();

public:
  Pyramid(std::vector<Real3> t) {
    assert(t.size() == 5);
    m_pos = t;
  }
  ~Pyramid() {
  }
};

#endif
