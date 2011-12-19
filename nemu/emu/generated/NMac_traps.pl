#!/usr/bin/perl
open(o,">NMac_traps_tab.h");
# Process 1st column
open(fin,"NMac_traps.txt");
$col=0; $count=0;
print o "// Don't edit this file, it is machine generated\n\n";
# Process 1st column
print o "UINT NMac_TrapWord[]={\n";
while(<fin>) {
  if(/^(.+)\t(.+)$/) {
    print o "$1";
    $col++;
    if($col<8) { print o ","; }
    if($col==8) { 
      $col=0;  $count++;
      print o ",\n";
    }
  }
}
print o "0\n";
print o "};\n\n";
close(fin);
# Process 2nd column
open(fin,"NMac_traps.txt");
$col=0; $count=0;
print o "const char *NMac_TrapName[]={\n";
while(<fin>) {
  if(/^(.+)\t(.+)$/) {
    print o "\"$2\"";
    $col++;
    if($col<4) { print o ","; }
    if($col==4) { 
      $col=0;  $count++;
      print o ",\n";
    }
  }
}
print o "NULL\n";
print o "};\n\n";
close(fin);
# done!
close(o);
