#ifndef LOGIC_H
#define LOGIC_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

namespace logic {

  using symbol = std::pair<bool, std::string>;
  using part   = std::vector<symbol>;

  enum form_set {
    SINGLE,
    AND,
    OR,
  };

  struct Sentence {
    std::vector<part> sentence;
    std::string form;
    form_set type;

    Sentence() {}

    Sentence(std::string name) :
    sentence({ part({ {0, name} }) }),
    form(name),
    type(SINGLE)
    {
    }

    int size() {
      return sentence.size();
    }

    bool empty() {
      return sentence.empty();
    }

    auto begin() {
      return sentence.begin();
    }

    auto end() {
      return sentence.end();
    }

    void push_back(part A) {
      sentence.push_back(A);
    }

    void check_form(form_set to) {
      if (type != SINGLE && type != to) {
        type = SINGLE;
        form = "(" + form + ")";
      }
    }

    void add(Sentence B) {
      for (part b : B) {
        sentence.push_back(b);
      }

      check_form(AND);
      B.check_form(AND);
      form += " && " + B.form;
      type = AND;
    }

    part& operator[] (int index) {
      return sentence[index];
    }

  };

  Sentence Symbol(std::string name) {
    return Sentence(name);
  }

  std::ostream& operator << (std::ostream& out, const Sentence &knowledge) {
    out << knowledge.form;
    return out;
  }

  std::ostream& operator << (std::ostream &out, part A) {
    for (int i = 0; i < A.size(); i++) {
      out << A[i].first << ":" << A[i].second;
      if (i + 1 != A.size())
        out << " || ";
    }
    return out;
  }

  Sentence And() {
    return Sentence();
  }

  template<typename ... U>
  Sentence And(Sentence A, U ... others) {

    if ((sizeof ...(others)) == 0) {
      A.check_form(AND);
      return A;
    }

    Sentence result = And(others...);

    for (part a : A) {
      result.push_back(a);
    }

    A.check_form(AND);
    result.form = A.form + " && " + result.form;
    result.type = AND;

    return result;
  }

  part part_or(part A, part B) {
    for (symbol &a : A) {
      B.push_back(a);
    }
    std::sort(B.begin(), B.end());

    return B;
  }

  Sentence Or() {
    return Sentence();
  }

  template<typename ... U>
  Sentence Or(Sentence A, U ... others) {

    if ((sizeof ...(others)) == 0) {
      A.check_form(OR);
      return A;
    }

    Sentence B = Or(others...);
    Sentence result;

    if ((A.empty() && !B.empty()) || (!A.empty() && B.empty())) {
      return A.empty() ? B : A;
    }

    for (part a : A) {
      for (part b : B) {
        result.push_back(part_or(a, b));
      }
    }

    if (!A.empty() && !B.empty()) {
      A.check_form(OR);
      result.form = A.form + " || " + B.form;
      result.type = OR;
    }

    return result;
  }

  Sentence part_not(part A) {
    Sentence result;

    for (symbol a : A) {
      a.first ^= 1;
      result.push_back(part({ a }));
    }

    return result;
  }

  Sentence Not(Sentence A) {
    Sentence result;

    for (part a : A) {
      result = Or(result, part_not(a));
    }

    A.check_form(SINGLE);
    result.form = "!" + A.form;
    result.type = SINGLE;

    return result;
  }

  Sentence Implies(Sentence A, Sentence B) {
    Sentence result = Or(Not(A), B);
    A.check_form(SINGLE);
    B.check_form(SINGLE);
    result.form = "(" + A.form + " => " + B.form + ")";
    result.type = SINGLE;
    return result;
  }

  Sentence Equivalent_to(Sentence A, Sentence B) {
    Sentence result = And(Implies(A, B), Implies(B, A));
    A.check_form(SINGLE);
    B.check_form(SINGLE);
    result.form = "(" + A.form + " <=> " + B.form + ")";
    result.type = SINGLE;
    return result;
  }

  symbol part_check(part &A, part &B) {
    for (symbol &a : A) {
      for (symbol &b : B) {
        if (a.first != b.first && a.second == b.second)
          return a;
      }
    }
    return symbol();
  }

  part part_add(part &A, part &B, symbol &x) {
    part result;
    for (symbol &a : A)
      if (a != x)
        result.push_back(a);

    x.first ^= 1;
    for (symbol &b : B)
      if (b != x)
        result.push_back(b);

    sort(result.begin(), result.end());
    return result;
  }

  bool check(Sentence A) {
    std::map<part, bool> visit;
    for (int i = 0; i < A.size(); i++) {
      visit[A[i]] = true;
      for (int j = 0; j < i; j++) {
        symbol x = part_check(A[i], A[j]);
        if (!x.second.empty()) {
          part new_part = part_add(A[i], A[j], x);
          if (new_part.empty())
            return false;
          if (!visit.count(new_part)) {
            A.push_back(new_part);
            visit[new_part] = true;
          }
        }
      }
    }
    return true;
  }

  bool model_check(Sentence knowledge, Sentence query) {
    if (knowledge.empty())
      return false;
    return !check(And(knowledge, Not(query)));
  }

}

#endif // LOGIC_H
