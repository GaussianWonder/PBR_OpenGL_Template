#ifndef __FILE_UTILS_H__
#define __FILE_UTILS_H__

#include <string>

namespace glt {

class FileUtils {
public:
  static std::string readFile(const std::string &fileName);
};

} // namespace glt

#endif // __FILE_UTILS_H__
