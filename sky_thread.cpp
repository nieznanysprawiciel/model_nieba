#include "sky_thread.h"
#include "completeskymodel.h"

sky_thread::sky_thread(CompleteSkyModel* sky, unsigned int off, unsigned int maximum)
{
    sky_model = sky;
    offset = off;
    max = maximum;
}

void sky_thread::run()
{
    sky_model->execute(offset, max);
}
