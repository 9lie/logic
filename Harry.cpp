#include "logic.h"
#include <iostream>
using namespace std;
using namespace logic;

int main() {
  Sentence rain       = Symbol("rain");       // It rained today.
  Sentence hagrid     = Symbol("hagrid");     // Harry visited Hagrid today.
  Sentence dumbledore = Symbol("dumbledore"); // Harry visited Dumbledore today.

  Sentence knowledge = And(
    Implies(Not(rain), hagrid),   // If it didn't rain, Harry visited Hagrid today.
    Or(hagrid, dumbledore),       // Harry visited Hagrid or Dumbledore today,
    Not(And(hagrid, dumbledore)), // but not both.
    dumbledore                    // Harry visited Dumbledore today.
  );

  cout << knowledge << endl;

  if (model_check(knowledge, hagrid))
    cout << "Harry visited Hagrid today." << endl;
  else
    cout << "Harry did not visit Hagrid today." << endl;

  if (model_check(knowledge, rain))
    cout << "It rained today." << endl;
  else
    cout << "It did not rain today." << endl;
}
