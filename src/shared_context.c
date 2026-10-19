#include "shared_context.h"

SharedContext *shared_context(){
    static SharedContext inst;
    return &inst;
}
