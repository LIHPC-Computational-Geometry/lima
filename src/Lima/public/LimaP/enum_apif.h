#ifndef _ENUM_APIF_H
#define _ENUM_APIF_H


/* Les points d'attache possibles d'un attribut */

enum att_loc {
  MAILLAGE,           //  0
  NOEUD,              //  1
  BRAS,               //  2
  POLYGONE,           //  3
  POLYEDRE,           //  4
  NUAGE,              //  5
  LIGNE,              //  6
  SURFACE,            //  7
  VOLUME,             //  8
  NOEUD_NUAGE,        //  9
  NOEUD_LIGNE,        // 10
  NOEUD_SURFACE,      // 11 
  NOEUD_VOLUME,       // 12
  BRAS_LIGNE,         // 13
  BRAS_SURFACE,       // 14
  BRAS_VOLUME,        // 15
  POLYGONE_SURFACE,   // 16
  POLYGONE_VOLUME,    // 17
  POLYEDRE_VOLUME     // 18
};

typedef enum att_loc att_loc;

#endif
