while(<>) {
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)$/) {
    $n=$5;
    $n =~ tr/\./\_/;
    print "void FOO::exec_$n(ULONG loc,UINT opcode)\n"; 
    print "{\n"; 
    print "  debugger(\"Unimplemented $5 opcode\");\n"; 
    print "}\n\n\n"; 
  }
};
