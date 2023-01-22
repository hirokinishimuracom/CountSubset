#include <iostream>
#include <cstdlib>
#include <set>
#include <vector>
#include <iterator> // std::next
#include <climits> // INT_MAX
#include <algorithm> // std::include


struct min_subset { int num = INT_MAX; std::set<std::set<std::set<int>>> collection; };

void print_set(std::set<int> S);
void print_collection(std::set<std::set<int>> C);
std::set<std::set<int>> power_set(std::set<int> S);
std::set<std::set<int>> choose(int total, int num);
std::vector<std::set<std::set<int>>> sort_collection(std::set<std::set<int>> C, int n);
min_subset minimum_subset(int k, std::set<std::set<int>> C, std::vector<std::set<std::set<int>>> P_sorted);
int count_subset(int k, std::set<std::set<int>> C, std::vector<std::set<std::set<int>>> P_sorted);
void count_subset_t(int k, std::vector<std::set<std::set<int>>> P_sorted);
void count_subset_all(int k, std::vector<std::set<std::set<int>>> P_sorted);

int main() {

  std::set<int> X;
  int n, k;
  
  // Constructing X

  std::cout << "Set the size of X [n]: ";
  std::cin >> n;
  std::cout << "\n";

  X.clear();
  for (int i = 0; i < n; i++)
  {
    X.insert(i + 1);
  }

  std::cout << "The set X is constructed as:\n X = ";
  print_set(X);
  std::cout << "\n\n";

  // Constructing the power set of X

  std::cout << "Generating the power set of X...\n";
  std::set<std::set<int>> P = power_set(X);
  std::cout << "The construction of the power set of X is complete. It contains " << P.size() << " subsets of X.\n\n";

  // Sorting the power set of X

  std::cout << "Sorting the power set of X by size...\n";
  std::vector<std::set<std::set<int>>> P_sorted = sort_collection(P, n);
  std::cout << "Sorting of the power set of X is complete.\n\n";

  std::cout << "The number of sets in each layer:";
  for (int i = 0; i < n + 1; ++i)
  {
    std::cout << ' ' << P_sorted[i].size();
  }
  std::cout << "\n\n";

  // Set k

  std::cout << "Set the size of subsets of X [k]: ";
  std::cin >> k;
  std::cout << "\n";

  std::cout << "There are " << P_sorted[k].size()
    << " subsets of X that contains k=" << k << " elements.\n\n";

  // Ask user whether to compute the minimum number of subsets for a specific t
  // or to compute them for all t.

  std::cout << "Type any non-zero integer to set a specific t or 0 to compute for all t: ";
  int ans;
  std::cin >> ans;
  std::cout << "\n";

  if (ans)
  {
    count_subset_t(k, P_sorted);
  }
  else
  {
    count_subset_all(k, P_sorted);
  }

  std::system("pause");

  return 0;

}

void print_set(std::set<int> S) {

  std::cout << "{ ";

  for (auto it = S.begin(); it != S.end(); ++it)
  {
    if (it != S.begin()) std::cout << ", ";
    std::cout << *it;
  }

  std::cout << " }";

}

void print_collection(std::set<std::set<int>> C) {

  std::cout << "{\n";

  for (auto it_C = C.begin(); it_C != C.end(); ++it_C)
  {
    std::cout << "    ";
    print_set(*it_C);
    std::cout << "\n";
  }

  std::cout << "}";
 
}

std::set<std::set<int>> power_set(std::set<int> S) {
  
  std::set<std::set<int>> P;
  std::set<int> empty;
  P.clear();
  empty.clear();
  P.insert(empty);

  for (auto s = S.begin(); s != S.end(); ++s)
  {
    for (auto p = P.begin(); p != P.end(); ++p)
    {
      std::set<int> union_ps = *p;
      union_ps.insert(*s);
      P.insert(union_ps);
    }
  }

  return P;

}

std::set<std::set<int>> choose(int total, int num) {

  // Choose num many elements from the set {0,...,total-1}

  // S = {0,...,total-1}
  std::set<int> S;
  S.clear();
  for (int i = 0; i < total; i++)
  {
    S.insert(i);
  }

  // While constructing the power set P of S,
  // any set of size num is transferred to C and removed from P.
  std::set<std::set<int>> P, C;
  std::set<int> empty;
  P.clear();
  empty.clear();
  P.insert(empty);

  for (auto s = S.begin(); s != S.end(); ++s)
  {
    for (auto p = P.begin(); p != P.end(); ++p)
    {
      std::set<int> union_ps = *p;
      union_ps.insert(*s);
      if (union_ps.size() == num)
      {
        C.insert(union_ps);
      }
      else
      {
        P.insert(union_ps);
      }
    }
  }

  return C;

}

std::vector<std::set<std::set<int>>> sort_collection(std::set<std::set<int>> C, int n) {

  std::vector<std::set<std::set<int>>> C_sorted;
  std::set<std::set<int>> empty;
  empty.clear();
  C_sorted.assign(n + 1, empty);

  for (auto it_C = C.begin(); it_C != C.end(); ++it_C)
  {
    C_sorted[it_C->size()].insert(*it_C);
  }

  return C_sorted;

}

min_subset minimum_subset(int k, std::set<std::set<int>> C, std::vector<std::set<std::set<int>>> P_sorted) {

  min_subset val;
  val.num = INT_MAX;

  // Temporary collection of t-many sets of size k
  std::set<std::set<int>> temp_collection;

  for (auto it_C = C.begin(); it_C != C.end(); ++it_C)
  {
    // Constructing temp_collection by choosing sets from P_sorted[k] according to it_C
    temp_collection.clear();
    for (auto it_CC = it_C->begin(); it_CC != it_C->end(); ++it_CC)
    {
      temp_collection.insert(*std::next(P_sorted[k].begin(), *it_CC));
    }
    
    // Counting the number of subsets of temp_collection
    int n_subset = count_subset(k, temp_collection, P_sorted);

    // Updating val as necessary
    if (n_subset == val.num) val.collection.insert(temp_collection);
    if (n_subset < val.num)
    {
      val.num = n_subset;
      val.collection.clear();
      val.collection.insert(temp_collection);
    }
  }

  return val;

}

int count_subset(int k, std::set<std::set<int>> C, std::vector<std::set<std::set<int>>> P_sorted) {
  
  if (k < 1) return 0;

  int num = 0;
  for (auto it_P = P_sorted[k - 1].begin(); it_P != P_sorted[k - 1].end(); ++it_P)
  {
    for (auto it_C = C.begin(); it_C != C.end(); ++it_C)
    {
      if (std::includes(it_C->begin(), it_C->end(), it_P->begin(), it_P->end()))
      {
        ++num;
        break;
      }
    }
  }

  return num;

}

void count_subset_t(int k, std::vector<std::set<std::set<int>>> P_sorted) {

  int t;

  std::cout << "Set the number of sets [t]: ";
  std::cin >> t;
  std::cout << "\n";

  // Constructing choices of t sets from the collection of sets of size k

  std::cout << "Constructing all choices of A_1,...,A_" << t
    << " from the collection of " << P_sorted[k].size() << " sets...\n";
  std::set<std::set<int>> C = choose(int(P_sorted[k].size()), t);
  std::cout << "The construction is complete. There are "
    << C.size() << " (" << P_sorted[k].size() << " choose " << t << ") many choices.\n\n";

  // Computing the minimum number of subsets of t-many sets of size k

  std::cout << "Computing the minimum number of sets of size " << k - 1
    << " that are subsets of at least one of A_1,...,A_" << t <<
    " across " << C.size() << " many choices of such sets...\n";
  min_subset val = minimum_subset(k, C, P_sorted);
  std::cout << "The computation is complete. The minimum number is "
    << val.num << ".\n\n";

  std::cout << "There are " << val.collection.size()
    << " collection(s) that achieve the minimum number.\n\n";
  std::cout << "The first five collections are:\n\n";
  int counter = 0;
  int ans = 0;
  for (auto it = val.collection.begin(); it != val.collection.end(); ++it)
  {
    print_collection(*it);
    std::cout << "\n\n";
    if (++counter == 5)
    {
      std::cout << "Type any non-zero integer to continue or 0 to terminate: ";
      std::cin >> ans;
      std::cout << "\n";
      if (ans)
      {
        counter = 0;
        continue;
      }
      else
      {
        break;
      }
    }
  }

}

void count_subset_all(int k, std::vector<std::set<std::set<int>>> P_sorted) {

  int t;

  // Execute, for each t, the same process as function count_subset_t

  for (t = 1; t <= P_sorted[k].size(); ++t)
  {
    std::set<std::set<int>> C = choose(int(P_sorted[k].size()), t);
    min_subset val = minimum_subset(k, C, P_sorted);
    std::cout << "t = " << t << "\tMIN = " << val.num << "\n\n";
  }
    
  if (k)
  {
    std::cout << "n Choose k-1 = " << P_sorted[k - 1].size() << "\n\n";
  }
  else
  {
    std::cout << "n Choose k-1 = 0\n\n";
  }

}
