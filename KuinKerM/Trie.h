#ifndef TRIEH
#define TRIEH

#include "../kuinkerm.h"

/* 字典树
 */
namespace kuinkerm {

  Trie::Trie(int maxStrLen, int maxCharNum)
      : maxStrLen(maxStrLen), maxCharNum(maxCharNum) {
    Sum.resize((maxStrLen + 1) * (maxCharNum + 1));
    preSum.resize((maxStrLen + 1) * (maxCharNum + 1));
    Try.resize(maxStrLen + 1);
    for (int i = 0; i <= maxStrLen; i++) Try[i].resize(maxCharNum + 1);
  }
  void Trie::Insert(const std::string& s) {
    int rt = 0;
    for (int i = 0; i < (int)s.size(); i++) {
      int id = strHash.str2Int(std::string(1, s[i]));
      if (!Try[rt][id]) Try[rt][id] = ++n;
      rt = Try[rt][id];
      preSum[rt]++;
      if (i == (int)(s.size() - 1)) Sum[rt]++;
    }
  }

  // 返回true为删除成功，否则为删除失败
  bool Trie::Delete(const std::string& s) {
    int rt = 0;
    for (int i = 0; i < (int)s.size(); i++) {
      int id = strHash.str2Int(std::string(1, s[i]));
      rt = Try[rt][id];
      if (i == (int)(s.size() - 1) && !Sum[rt]) return false;
      if (i == (int)(s.size() - 1)) Sum[rt]--;
    }
    rt = 0;
    for (int i = 0; i < (int)s.size(); i++) {
      int id = strHash.str2Int(std::string(1, s[i]));
      rt = Try[rt][id];
      assert(preSum[rt]);
      preSum[rt]--;
    }
    return true;
  }
  // 字符串的存在次数
  int Trie::existTimes(const std::string& s) {
    int rt = 0;
    for (int i = 0; i < (int)s.size(); i++) {
      int id = strHash.str2Int(std::string(1, s[i]));
      if (!Try[rt][id]) return 0;
      rt = Try[rt][id];
      if (i == (int)(s.size() - 1)) return Sum[rt];
    }
    return 0;
  }

  // 计算拥有某前缀的字符串的数量
  int Trie::preExistTimes(const std::string& s) {
    int rt = 0;
    for (int i = 0; i < (int)s.size(); i++) {
      int id = strHash.str2Int(std::string(1, s[i]));
      if (!Try[rt][id]) return 0;
      rt = Try[rt][id];
      if (i == (int)(s.size() - 1)) return preSum[rt];
    }
    return 0;
  }
}  // namespace kuinkerm
#endif