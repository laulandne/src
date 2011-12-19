#! /usr/bin/perl

$name=$ARGV[0];

# NOTE: format of txt file is the following
# hex
# dec
# len
# time
# name
# format
# subformat
# flag

open(f,"$name.txt");
$goof="_tab";
open(o,">$name$goof.h");
print o "// Don't edit this file, it is machine generated\n";
print o "\n\n";
$goof="_TAB_H";
print o "#ifndef ELIB_$name$goof\n";
print o "#define ELIB_$name$goof\n";
print o "\n\n";
# First output the size table...
seek(f,0,0);
$goof="_size[]";
print o "int $name$goof={\n";
$t=0;  $c=0;
while(<f>) {
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)$/) {
    if($c==256) { print o ","; }
    print o "$3";
    #print "$1\n";
    $t=$t+1;  $c=$c+1;
    if($c!=256) { print o ","; }
    if($t==8) { $t=0;  print o "\n"; }
  }
  else {
    print "Line Rejected: ";
    print;
    print "\n";
  };
};
print o "};\n\n";
# then the time table...
seek(f,0,0);
$goof="_time[]";
print o "int $name$goof={\n";
$t=0;  $c=0;
while(<f>) {
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)$/) {
    if($c==256) { print o ","; }
    print o "$4";
    #print "$1\n";
    $t=$t+1;  $c=$c+1;
    if($c!=256) { print o ","; }
    if($t==8) { $t=0;  print o "\n"; }
  }
  else {
    print "Line Rejected: ";
    print;
    print "\n";
  };
};
print o "};\n\n";
# then the opcode table...
seek(f,0,0);
$goof="_name[]";
print o "const char *$name$goof={\n";
$t=0;  $c=0;
while(<f>) {
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)$/) {
    if($c==256) { print o ","; }
    print o "\"$5\"";
    #print "$1\n";
    $t=$t+1;  $c=$c+1;
    if($c!=256) { print o ","; }
    if($t==8) { $t=0;  print o "\n"; }
  }
  else {
    print "Line Rejected: ";
    print;
    print "\n";
  };
};
print o "};\n\n";
# Then output the decode format...
seek(f,0,0);
$goof="_format[]";
print o "int $name$goof={\n";
$t=0;  $c=0;
while(<f>) {
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)$/) {
    if($c==256) { print o ","; }
    print o "$6";
    #print "$1\n";
    $t=$t+1;  $c=$c+1;
    if($c!=256) { print o ","; }
    if($t==4) { $t=0;  print o "\n"; }
  }
  else {
    print "Line Rejected: ";
    print;
    print "\n";
  };
};
print o "};\n\n";
# Then output the subformat...
seek(f,0,0);
$goof="_subformat[]";
print o "int $name$goof={\n";
$t=0;  $c=0;
while(<f>) {
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)$/) {
    $val=$7;
    if($val eq "FALSE") { $val="false"; }
    if($c==256) { print o ","; }
    print o "$val";
    #print "$1\n";
    $t=$t+1;  $c=$c+1;
    if($c!=256) { print o ","; }
    if($t==4) { $t=0;  print o "\n"; }
  }
  else {
    print "Line Rejected: ";
    print;
    print "\n";
  };
};
print o "};\n\n";
# finally output the flags...
seek(f,0,0);
$goof="_flags[]";
print o "int $name$goof={\n";
$t=0;  $c=0;
while(<f>) {
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)$/) {
    $val=$8;
    if($val eq "FALSE") { $val="false"; }
    if($c==256) { print o ","; }
    print o "$val";
    #print "$1\n";
    $t=$t+1;  $c=$c+1;
    if($c!=256) { print o ","; }
    if($t==4) { $t=0;  print o "\n"; }
  }
  else {
    print "Line Rejected: ";
    print;
    print "\n";
  };
};
print o "};\n\n";
# That's it!
$goof="_TAB_H";
print o "#endif // ELIB_$name$goof\n";
close o;
close f;
