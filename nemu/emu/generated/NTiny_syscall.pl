#!/usr/bin/perl
open(o,">NTiny_syscall_tab.h");
# Create defines
open(fin,"NTiny_syscall.txt");
$col=0; $count=0;
print o "// Don't edit this file, it is machine generated\n\n";
while(<fin>) {
  if(/^(.+)\t(.+)\t(.+)\t(.+)$/) {
    print o "#define $1  $2\n";
  }
}
print o "\n";
close(fin);
# done!
close(o);
