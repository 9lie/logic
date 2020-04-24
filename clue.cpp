#include "logic.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace logic;

int main() {
  vector<Sentence> symbols;

  // 3个人物
  Sentence mustard = Symbol("ColMustard");
  Sentence plum = Symbol("ProfPlum");
  Sentence scarlet = Symbol("MsScarlet");
  vector<Sentence> characters = { mustard, plum, scarlet };
  for (Sentence character : characters)
    symbols.push_back(character);

  // 3个地点
  Sentence ballroom = Symbol("ballroom");
  Sentence kitchen = Symbol("kitchen");
  Sentence library = Symbol("library");
  vector<Sentence> rooms = { ballroom, kitchen, library };
  for (Sentence room : rooms)
    symbols.push_back(room);

  // 3种凶器
  Sentence knife = Symbol("knife");
  Sentence revolver = Symbol("revolver");
  Sentence wrench = Symbol("wrench");
  vector<Sentence> weapons = { knife, revolver, wrench };
  for (Sentence weapon : weapons)
    symbols.push_back(weapon);

  auto check_knowledge = [&](Sentence knowledge) {
    for (Sentence symbol : symbols) {
      if (model_check(knowledge, symbol))
        cout << "!!! " << symbol << ": YES !!!" << endl;
      else
        cout << symbol << ": MAYBE" << endl;
    }
  };

  // 基础条件，只有一个凶手，一种凶器，一个犯罪现场
  Sentence knowledge = And(
    Or(mustard, plum, scarlet),
    Or(ballroom, kitchen, library),
    Or(knife, revolver, wrench)
  );

  // 根据自己手上的牌，排除一些
  knowledge.add(Not(mustard));
  knowledge.add(Not(kitchen));
  knowledge.add(Not(revolver));

  // 根据别人回合，得到一些限制条件
  knowledge.add(Or(
    Not(scarlet), Not(library), Not(wrench)
  ));

  // 根据自己的回合得到的情报，排除一些
  knowledge.add(Not(plum));
  knowledge.add(Not(ballroom));

  cout << knowledge << endl;
  check_knowledge(knowledge);
}
