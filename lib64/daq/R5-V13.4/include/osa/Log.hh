#ifndef OSA_LOG
#define OSA_LOG

namespace OSA {

class Log {
public:
  enum {DEBUG=0, INFO=1, NOTICE=2, WARNING=3, ERROR=4};
public:
  static void init();
  static void write(const char* format, ...);
  static void write(int priority, const char* format, ...);
};

}
#endif
