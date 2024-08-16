#include "anonymizationalgorithm.h"

void print_alg(const int meas_choice, const int variant, char end){
    if(meas_choice == ALG_RAND) std::cout<<"Random"<<end;
    else if(meas_choice == ALG_UNIQUE) std::cout<<"Unique"<<end;
    else if(meas_choice == ALG_DEGREE) std::cout<<"Degree"<<end;
    else if(meas_choice == ALG_AFF_TOTAL) std::cout<<"Affected total"<<end;
    else if(meas_choice == ALG_AFF_UNIQUE) std::cout<<"Affected unique"<<end;
    else if(meas_choice == ALG_AFF_UNIQUE_ONLY) std::cout<<"Affected unique only"<<end;
    else std::cout<<"none"<<end;

    std::cout<<"Variant: "<<variant<<std::endl;
}
