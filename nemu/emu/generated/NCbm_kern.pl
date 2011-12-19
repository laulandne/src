#!/usr/bin/perl
open(o,">NCbm_kern_tab.h");
# Create defines
open(fin,"NCbm_kern.txt");
$col=0; $count=0;
print o "// Don't edit this file, it is machine generated\n\n";
while(<fin>) {
  if(/^(.+)\t(.+)\t(.+)\t(.+)$/) {
    print o "#define NCBM_KERNEL_$1  0x$2\n";
  }
}
print o "\n";
close(fin);
# Build table
open(fin,"NCbm_kern.txt");
$col=0; $count=0;
print o "#ifdef NCBM_WANT_KERN_TABLE\n";
print o "static ULONG NCbm_Kern[]={\n";
while(<fin>) {
  if(/^(.+)\t(.+)\t(.+)\t(.+)$/) {
    print o "(ULONG)\"$1\",0x$2,$3,$4,\n";
  }
}
print o "0,0,0,0\n";
print o "};\n";
print o "#endif\n";
print o "\n";
close(fin);
# done!
close(o);
