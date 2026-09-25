//
// Classe Real3 Compatible avec celle du code T
//

#ifndef Real3_H
#define Real3_H

#include <cmath>
#include <iostream>
#include <Lima/lima++.h>

typedef double Real;

  class Real3
    {
      public:

        Real3 (Real x0, Real y0, Real z0):x (x0), y (y0), z (z0)
      {
      }
      Real3 ():x (0.0), y (0.0), z (0.0)
      {
      }
      Real3 (const Real3 & f):x (f.x), y (f.y), z (f.z)
      {
      }

      Real3 (const Lima::Noeud n):x (n.x ()), y (n.y ()), z (n.z ())
      {
      }
      Real x, y, z;

      static Real3 null ()
      {
	return Real3 (0.0, 0.0, 0.0);
      }

      Real3  operator = (Real3 f)
      {
	x = f.x, y = f.y, z = f.z;
	return *this;
      }
     Real3  operator = (Real v)
     {
       x = y = z = v;
       return *this;
     }

     Real abs2 () const
     {
       return x * x + y * y + z * z;
     }

     Real abs () const
     {
       return sqrt (abs2 ());
     }

      Real3 & operator += (Real3  b)
      {
	x += b.x;
	y += b.y;
	z += b.z;
	return *this;
      }
      Real3 & operator -= (Real3  b)
      {
	x -= b.x;
	y -= b.y;
	z -= b.z;
	return *this;
      }

      Real3 operator + (Real3  b) const
      {
	return Real3 (x + b.x, y + b.y, z + b.z);
      }


     Real3 operator - (Real3  b) const
     {
       return Real3 (x - b.x, y - b.y, z - b.z);
     }

     Real3 operator - () const
     {
       return Real3 (-x, -y, -z);
     }
     bool operator == (Real3 b) const
     {
       return x == b.x && y == b.y && z == b.z;
     }
     bool operator != (Real3 b) const
     {
       return !operator == (b);
     }

     Real3 operator * (Real b) const
     {
       return Real3 (x * b, y * b, z * b);
     }

     Real3 operator / (Real b) const
     {
       return Real3 (x * (1 / b), y * (1 / b), z * (1 / b));
     }


     inline bool lexicographicLessThan (Real3  other, Real EPS) const;
    };




     inline bool
       Real3::lexicographicLessThan (Real3  other, Real EPS) const
     {
       if (x < other.x - EPS)
	 return 1;
       if (x > other.x + EPS)
	 return 0;
       if (y < other.y - EPS)
	 return 1;
       if (y > other.y + EPS)
	 return 0;
       if (z < other.z - EPS)
	 return 1;
       if (z > other.z + EPS)
	 return 0;
       return 0;
     }

inline   std::ostream & operator << (std::ostream & o, Real3  v)
  {
    o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return o;

  }

  inline Real3 operator * (Real c, Real3  b)
  {
    return Real3 (c * b.x, c * b.y, c * b.z);
  }



     class VertexLexicographicLessThan
     {
       Real m_eps;
       public:
         VertexLexicographicLessThan (Real eps = 1.e-6):m_eps (eps)
       {};
       bool operator () (Real3 * ent1, Real3 * ent2) const
       {
	 return ent1->lexicographicLessThan (*ent2, m_eps);
       }

     };

     class Real2
       {
	 public:

	   Real2 (Real x0, Real y0):x (x0), y (y0)
	 {
	 }
       Real2 ():x (0.0), y (0.0)
	 {
	 }
	 Real2 (const Real2 & f):x (f.x), y (f.y)
	 {
	 }


	 Real x, y;

	 static Real2 null ()
	 {
	   return Real2 (0.0, 0.0);
	 }

	 Real2  operator = (Real2 f)
	 {
	   x = f.x, y = f.y;
	   return *this;
	 }
	 Real2  operator = (Real v)
	 {
	   x = y = v;
	   return *this;
	 }

	 Real abs2 ()const
	 {
	   return x * x + y * y;
	 }

	 Real abs ()const
	 {
	   return sqrt (abs2 ());
	 }

	 Real2 & operator += (Real2  b)
	 {
	   x += b.x;
	   y += b.y;
	   return *this;
	 }
	 Real2 & operator -= (Real2  b)
	 {
	   x -= b.x;
	   y -= b.y;
	   return *this;
	 }

	 Real2 operator + (Real2  b) const
	 {
	   return Real2 (x + b.x, y + b.y);
	 }


	 Real2 operator - (Real2  b) const
	 {
	   return Real2 (x - b.x, y - b.y);
	 }

	 Real2 operator - () const
	 {
	   return Real2 (-x, -y);
	 }
	 bool operator == (Real2 b) const
	 {
	   return x == b.x && y == b.y;
	 }
	 bool operator != (Real2 b) const
	 {
	   return !operator == (b);
	 }

     Real2 operator * (Real b) const
     {
       return Real2 (x * b, y * b);
     }

	 Real2 operator / (Real b) const
	 {
	   return Real2 (x * (1 / b), y * (1 / b));
	 }


       };





inline  std::ostream & operator << (std::ostream & o, Real2  v)
  {
    o << "(" << v.x << ", " << v.y << ")";
    return o;

  }

  inline Real2 operator * (Real c, Real2  b)
  {
    return Real2 (c * b.x, c * b.y);
  }


  inline Real abs (const Real & u)
  {
    return fabs (u);
  }

  inline Real3 vecMul (Real3  v1, Real3  v2)
  {
    return Real3 (v1.y * v2.z - v1.z * v2.y, -v1.x * v2.z + v1.z * v2.x,
		  v1.x * v2.y - v1.y * v2.x);
  }
  
  inline Real scaMul (Real3  u, Real3  v)
  {
    return (u.x * v.x + u.y * v.y + u.z * v.z);
  }

  inline Real scaMul (Real2  u, Real2  v)
  {
    return (u.x * v.x + u.y * v.y);
  }
  
  inline Real mixteMul (Real3  v1, Real3  v2, Real3 v3)
  {
    return v1.x * v2.y * v3.z + v2.x * v3.y * v1.z + v3.x * v1.y * v2.z
    - v1.z * v2.y * v3.x - v2.z * v3.y * v1.x - v3.z * v1.y * v2.x;
  }



#endif
