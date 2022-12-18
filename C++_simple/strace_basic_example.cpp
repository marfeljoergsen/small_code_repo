// From: https://lovethepenguin.com/linux-troubleshoot-programs-with-strace-like-a-pro-34b7a166a5f9

// --- If "test.txt" does not exist: ---
// g++ strace_basic_example.cpp  && ./a.out
// File "test.txt" can *NOT* be opened

// --- If "test.txt" does exist: ---
// g++ strace_basic_example.cpp  && ./a.out
// File "test.txt" can be opened

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
int main()
{
   const string fileName = "test.txt";
   ifstream istr;
   istr.open(fileName.c_str());
   if (istr.fail())
   {
      cout <<"File \"" << fileName << "\" can *NOT* be opened"<<endl;
      exit (1);
   }
   cout <<"File \"" << fileName << "\" can be opened"<<endl;
   return (0);
}

/*

strace -c ./a.out
File "test.txt" can *NOT* be opened
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 47,58    0,000619           3       172       167 openat
 31,13    0,000405           2       136       118 newfstatat
  9,53    0,000124           5        22           mmap
  3,31    0,000043           6         7           mprotect
  1,38    0,000018           3         5           close
  1,31    0,000017           4         4           read
  1,31    0,000017           3         5           pread64
  1,08    0,000014          14         1           munmap
  0,92    0,000012          12         1           write
  0,54    0,000007           2         3           brk
  0,31    0,000004           2         2         1 arch_prctl
  0,31    0,000004           4         1           prlimit64
  0,31    0,000004           4         1           getrandom
  0,31    0,000004           4         1           rseq
  0,23    0,000003           3         1           futex
  0,23    0,000003           3         1           set_tid_address
  0,23    0,000003           3         1           set_robust_list
  0,00    0,000000           0         1         1 access
  0,00    0,000000           0         1           execve
------ ----------- ----------- --------- --------- ----------------
100,00    0,001301           3       366       287 total


strace -c ./a.out
File "test.txt" can be opened
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 43,32    0,000269           1       172       166 openat
 20,61    0,000128           5        22           mmap
 11,43    0,000071           0       136       118 newfstatat
  7,25    0,000045           6         7           mprotect
  3,06    0,000019           3         6           close
  2,58    0,000016           4         4           read
  2,58    0,000016           3         5           pread64
  2,25    0,000014          14         1           munmap
  1,93    0,000012          12         1           write
  1,29    0,000008           2         3           brk
  0,64    0,000004           4         1           prlimit64
  0,64    0,000004           4         1           getrandom
  0,48    0,000003           1         2         1 arch_prctl
  0,48    0,000003           3         1           futex
  0,48    0,000003           3         1           set_tid_address
  0,48    0,000003           3         1           set_robust_list
  0,48    0,000003           3         1           rseq
  0,00    0,000000           0         1         1 access
  0,00    0,000000           0         1           execve
------ ----------- ----------- --------- --------- ----------------
100,00    0,000621           1       367       286 total

Lets examine the errors, strace accepts the -e parameter which allows to filter
only errors of specific type like file errors or network errors, but since we
don't know the exact error lets try:

❯ strace ./a.out
...
...
getrandom("\xae\xd9\x12\x6e\x6d\xdc\x41\x2f", 8, GRND_NONBLOCK) = 8
brk(NULL)                               = 0x556623468000
brk(0x556623489000)                     = 0x556623489000
futex(0x7f94a82346bc, FUTEX_WAKE_PRIVATE, 2147483647) = 0
openat(AT_FDCWD, "test.txt", O_RDONLY)  = -1 ENOENT (No such file or directory)
newfstatat(1, "", {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0xf), ...}, AT_EMPTY_PATH) = 0
write(1, "File \"test.txt\" can *NOT* be ope"..., 36File "test.txt" can *NOT* be opened
) = 36
exit_group(1)                           = ?
+++ exited with 1 +++

The last error before the exit code was:
openat(AT_FDCWD, "test.txt", O_RDONLY)  = -1 ENOENT (No such file or directory)

*/

