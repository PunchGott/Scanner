#include "error.h"

Error::Error()
{
   QDialog error;
   error.exec();
}
