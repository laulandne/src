#!/usr/bin/perl
open(o,">NAmiga_reg_tab.h");
# Process 1st column
open(fin,"NAmiga_reg.txt");
$col=0; $count=0;
print o "// Don't edit this file, it is machine generated\n\n";
print o "const char *NAmiga_RegName[]={\n";
while(<fin>) {
  if(/^(.+)\t(.+)\t(.+)\t(.+)\t(.+)\t(.+)$/) {
    print o "\"$1\"";
    $col++;
    if($col<4) { print o ","; }
    if($col==4) { 
      $col=0;  $count++;
      if($count!=255) { print o ","; }
      print o "\n";
    }
  }
}
print o "};\n\n";
close(fin);
# Process 2nd column
open(fin,"NAmiga_reg.txt");
$col=0; $count=0;
print o "const char *NAmiga_RegSymbol[]={\n";
while(<fin>) {
  if(/^(.+)\t(.+)\t(.+)\t(.+)\t(.+)\t(.+)$/) {
    print o "\"$2\"";
    $col++;
    if($col<8) { print o ","; }
    if($col==8) { 
      $col=0;  $count++;
      if($count!=256) { print o ","; }
      print o "\n";
    }
  }
}
print o "};\n\n";
close(fin);
# Process 4th column
open(fin,"NAmiga_reg.txt");
$col=0; $count=0;
print o "const char *NAmiga_RegChip[]={\n";
while(<fin>) {
  if(/^(.+)\t(.+)\t(.+)\t(.+)\t(.+)\t(.+)$/) {
    print o "\"$4\"";
    $col++;
    if($col<8) { print o ","; }
    if($col==8) { 
      $col=0;  $count++;
      if($count!=256) { print o ","; }
      print o "\n";
    }
  }
}
print o "};\n\n";
close(fin);
# Process 5th column
open(fin,"NAmiga_reg.txt");
$col=0; $count=0;
print o "const char *NAmiga_RegAccess[]={\n";
while(<fin>) {
  if(/^(.+)\t(.+)\t(.+)\t(.+)\t(.+)\t(.+)$/) {
    print o "\"$5\"";
    $col++;
    if($col<8) { print o ","; }
    if($col==8) { 
      $col=0;  $count++;
      if($count!=256) { print o ","; }
      print o "\n";
    }
  }
}
print o "};\n\n";
close(fin);
# Process 6th column
$count=0;
open(fin,"NAmiga_reg.txt");
print o "const char *NAmiga_RegDesc[]={\n";
while(<fin>) {
  if(/^(.+)\t(.+)\t(.+)\t(.+)\t(.+)\t(.+)$/) {
    print o "\"$6\"";
   $count++;
    if($count!=256) { print o ","; }
    print o "\n";
  }
}
print o "};\n\n";
close(fin);
# done!
close(o);
