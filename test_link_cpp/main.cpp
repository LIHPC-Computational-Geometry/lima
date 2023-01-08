#include <Lima/malipp.h>
#include <Lima/erreur.h>
#include <Lima/polyedre.h>
#include "Lima/lima++.h"
#include <Lima/malipp2.h>

int main(){
    double lenght_unit = 1.;
    Lima::MaliPPWriter2* writer=new Lima::MaliPPWriter2("toto", 1);

    writer->unite_longueur(lenght_unit);
    writer->dimension(Lima::D2);
    writer->close();
    return 0;
}