#include "Lima/lima++.h"
#include <Lima/malipp.h>

int main(){
    double lenght_unit = 1.;
    Lima::MaliPPWriter* writer=new Lima::MaliPPWriter("toto", 1);
    writer->unite_longueur(lenght_unit);
    writer->dimension(Lima::D2);
    writer->close();
    return 0;
}