#include "logic.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace logic;

int main() {
  vector<Sentence> symbols;

  // 3������
  Sentence mustard = Symbol("ColMustard");
  Sentence plum = Symbol("ProfPlum");
  Sentence scarlet = Symbol("MsScarlet");
  vector<Sentence> characters = { mustard, plum, scarlet };
  for (Sentence character : characters)
    symbols.push_back(character);

  // 3���ص�
  Sentence ballroom = Symbol("ballroom");
  Sentence kitchen = Symbol("kitchen");
  Sentence library = Symbol("library");
  vector<Sentence> rooms = { ballroom, kitchen, library };
  for (Sentence room : rooms)
    symbols.push_back(room);

  // 3������
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

  // ����������ֻ��һ�����֣�һ��������һ�������ֳ�
  Sentence knowledge = And(
    Or(mustard, plum, scarlet),
    Or(ballroom, kitchen, library),
    Or(knife, revolver, wrench)
  );

  // �����Լ����ϵ��ƣ��ų�һЩ
  knowledge.add(Not(mustard));
  knowledge.add(Not(kitchen));
  knowledge.add(Not(revolver));

  // ���ݱ��˻غϣ��õ�һЩ��������
  knowledge.add(Or(
    Not(scarlet), Not(library), Not(wrench)
  ));

  // �����Լ��Ļغϵõ����鱨���ų�һЩ
  knowledge.add(Not(plum));
  knowledge.add(Not(ballroom));

  cout << knowledge << endl;
  check_knowledge(knowledge);
}
