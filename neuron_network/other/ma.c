#include "ma.h"
#include "predict.h"

int process_NOE(int set)
{
    if(set == 1){
        pro_grid();
        //pro_word();
        return 1;
    }
    else if (set == 2) {
        pro_solv();
        return 1;
    }
    else errx(EXIT_FAILURE, "Invalid arg!");   
}