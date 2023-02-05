#include "thread.h"

int main()
{
    p_parameters params=(p_parameters)calloc(1,sizeof(parameters));
    assert(params);
    init_parameters(params);
    while(params->status)
    {
        usleep(1000);
    }
    free_parameters(params);
}
