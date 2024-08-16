#include "measure.h"

void print_measure(const int meas_choice, char end){
    if(meas_choice == MEAS_DEGREE) std::cout<<"Degree"<<end;
    else if(meas_choice == MEAS_DEGDIST) std::cout<<"Degree distribution"<<end;
    else if(meas_choice == MEAS_HAY) std::cout<<"Hay"<<end;
    else if(meas_choice == MEAS_COUNT) std::cout<<"Count"<<end;
    else if(meas_choice == MEAS_DKANON) std::cout<<"d-k-Anonymity"<<end;
    else if(meas_choice == MEAS_HYBRID) std::cout<<"Hybrid"<<end;
    else if(meas_choice == MEAS_EDGECOUNT) std::cout<<"Edgecount"<<end;
    else std::cout<<"none"<<end;
}