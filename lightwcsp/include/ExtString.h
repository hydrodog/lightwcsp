/*
 * Represent a string whose contents is in an externally-managed Buffer.
 * The assumption is that the store lasts longer than this object.
 * This is a high-performance method for representing strings in a config file
 * for example
 * @author: Dov Kruger
 *
 */
#include <Buffer.h>

class ExtString {
private:
  Buffer& buf;
  int pos;
  int len;
public:
  ExtString(Buffer& buf_i, int pos_i, int len_i) :
    buf(buf_i), pos(pos_i), len(len_i) {}
  
};
