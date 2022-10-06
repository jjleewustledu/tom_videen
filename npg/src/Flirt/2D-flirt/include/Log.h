#ifndef _LOG_H_
#define _LOG_H_

#include <iostream>
namespace flirt {

namespace Log {
  typedef unsigned int logmask_t;
  const logmask_t none = 0x00000000;
  const logmask_t all  = 0xffffffff;

  void setOutput( std::streambuf *sb);
  void setLogmask( logmask_t mask);
  void printf( logmask_t mask, const char *fmt, ...);
}

}
#endif /* _LOG_H_ */
