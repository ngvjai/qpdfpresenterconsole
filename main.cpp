/* vim: set et ts=4 sw=4: */

#include "presenter.h"

int main(int argc, char *argv[])
{
    int retval = 0;
    Presenter app(argc, argv);
    if (app.isReady()) {
        retval = app.exec();
    } else {
        retval = EXIT_FAILURE;
    }
    return retval;
}
