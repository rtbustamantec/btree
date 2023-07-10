
#include <iostream>
#include <vector>
#include "btree.h"
#include "tester.h"

using namespace std;

int main() {
  BTree<int>* btree = new BTree<int>(10);
  int numbers[10] = {45, 75, 100, 36, 120, 70, 11, 111, 47, 114};
  for (int i = 0; i < 10; i++) {
    btree->insert(numbers[i]);
  }

  ASSERT(btree->toString(" ") == "11 36 45 47 70 75 100 111 114 120",
         "The function insert is not working");
  ASSERT(btree->height() == 1, "The function height is not working");
  for (int i = 0; i < 10; i=i+2) {
    ASSERT(btree->search(numbers[i]) == true,
           "The function search is not working");
  }
  ASSERT(btree->minKey() == 11, "The function minKey is not working");
  ASSERT(btree->maxKey() == 120, "The function maxKey is not working");

  btree->remove(100);
  btree->remove(111);
  btree->remove(45);

  ASSERT(btree->toString(" ") == "11 36 47 70 75 114 120",
         "The function remove is not working");
  ASSERT(btree->height() == 1, "The function height is not working");

  btree->remove(114);
  btree->insert(40);

  ASSERT(btree->toString(" - ") == "11 - 36 - 40 - 47 - 70 - 75 - 120",
         "The function toString is not working");
  ASSERT(btree->height() == 1, "The function height is not working");

  btree->insert(125);
  btree->insert(115);

  ASSERT(btree->maxKey() == 125, "The function maxKey is not working");
  ASSERT(btree->search(47) == true,  "The function search is not working");

  btree->remove(11);

  ASSERT(btree->search(11) == false,  "The function search is not working");
  ASSERT(btree->minKey() == 36, "The function minKey is not working");
  ASSERT(btree->size() == 8, "The function size is not working");

  std::vector<int> result = btree->searchRange(50, 120);
  string result_string = "";
  string test_result_string = "70 75 115 120 ";
  for (int i = 0; i < result.size(); i++) {
      result_string += to_string(result[i]) + " ";
  }
  ASSERT(result.size() == 4, "The function searchRange is not working");
  ASSERT(result_string == test_result_string, "The function searchRange is not working");

  btree->clear();
  ASSERT(btree->size() == 0, "The function size is not working");
  ASSERT(btree->height() == 0, "The function height is not working");

  return 0;
}
