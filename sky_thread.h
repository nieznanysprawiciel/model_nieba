#ifndef SKY_THREAD_H
#define SKY_THREAD_H

#include <QThread>
#include "glm/gtx/quaternion.hpp"


class CompleteSkyModel;

class sky_thread : public QThread
{
    Q_OBJECT
private:
    CompleteSkyModel* sky_model;
    unsigned int offset;
    unsigned int max;
public:
    sky_thread(CompleteSkyModel *sky, unsigned int off, unsigned int maximum);

protected:
     void run();
};

#endif // SKY_THREAD_H
