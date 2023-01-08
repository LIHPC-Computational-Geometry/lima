#!/usr/bin/env python

import sys
sys.path.append("")
from LimaScripting import *

print(lima_version())

# lecture
m = Maillage()
m.lire("puits2.mli")
m.preparer(CODE_T)
# informations
print(m.nb_noeuds())
print(m.nb_polyedres())
print(m.nb_polygones())
print(m.nb_volumes())
# un noeud
n =  m.noeud(0)
print(n.x())
print(n.y())
print(m.noeud(0).z())
n.afficher()

v =  m.volume_id(0)
print(v.nom())
v.nommer("VOL0")

for i in range(0, m.nb_volumes()):
    v = m.volume_id(i)
    print(v.nom(), v.nb_polyedres())
    lst = v.att_polyedres()
    for j in range(0, lst.nb_elements()):
        att = lst.element(j)
        print(att.nom(), att.nb_elements())

        for j in range(0, att.nb_elements()):
            att.get_val(j, v)
            print(v)

# sauvegarde
m.ecrire("puits.mli")
