#ifndef STRINGDELH
#define STRINGDELH

#include "kuinkerm.h"

/* 字符串处理
 */
namespace kuinkerm {

  std::vector<std::string> stringDel::splitByDel(std::string s, char Del) {
    std::stringstream Ss(s);
    std::string strEle;
    std::vector<std::string> Rnt;
    while (getline(Ss, strEle, Del)) Rnt.emplace_back(strEle);
    return Rnt;
  }

    std::string stringDel::stringHash::int2Str(int x) {
      if (x <= (int)V.size())
        return V[x - 1];
      else {
        throw std::runtime_error(
            "stringDel::stringHash, int2Str out_of_range, if you think that's "
            "no problem, then just delete this exception.");
        return "";
      }
    }
    int stringDel::stringHash::str2Int(const std::string& s) {
      if (HT.Find(s) == HT.notFound()) {
        V.emplace_back(s);
        return (HT[s] = (int)V.size());
      } else
        return HT[s];
    }

}  // namespace kuinkerm
#endif