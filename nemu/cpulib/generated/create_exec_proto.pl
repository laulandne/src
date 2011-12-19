while(<>) {
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)$/) {
    $n=$5;
    $n =~ tr/\./\_/;
    print "void exec_$n(ULONG loc,UINT opcode);\n"; 
  }
};
