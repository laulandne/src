#! /usr/bin/perl

while(<>) {
  if(/(.*).txt/) {
    $name=$1;
    $cmd="perl create_dis_h.pl $name";
    system($cmd);
    $cmd="perl create_exec_h.pl $name";
    system($cmd);
    $cmd="perl create_exec_proto.pl $name";
    system($cmd);
    $cmd="perl create_exec_stubs.pl $name";
    system($cmd);
    $cmd="perl create_tab_h.pl $name";
    system($cmd);
  }
  else {
    print "Line rejected:\n";
    print "$_\n";
  }
}

