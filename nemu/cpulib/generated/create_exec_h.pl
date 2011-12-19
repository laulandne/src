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
$goof="_exec.h";
open(o,">$name$goof");
print o "  // Don't edit this file, it is machine generated\n";
print o "  switch(topcode) {\n";
while(<f>) {
  $t=0;
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_COND_DISP\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_bcc(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_SPECIAL\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_special(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_UNIMPLEMENTED\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_unimplemented(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_A_LINE\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_a_line(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_F_LINE\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_f_line(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_COUNT_SIZE_REG\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_$5(loc,reg,size,reg2,dir);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_REG_DATA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_$5(loc,reg,reg2);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_$5(loc,mode,reg);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_SIZE_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_$5(loc,mode,reg,size);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_REG_MODE_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_$5(loc,reg2,mode2,mode,reg);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_DATA_SIZE_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_$5(loc,mode,reg,reg2,size);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_EA_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_$5(loc,topcode,reg2,mode2,mode,reg);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_REG_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_$5(loc,reg2,mode,reg);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_REG_LEA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_$5(loc,reg2,mode,reg);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_COND_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_$5(loc,mode2,mode,reg);  break;\n"; 
    $t=1;
  }
  #if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tECPU_ADDR_NONE\t(.*)\t(.*)$/) {
  #  print o "      case 0x$1:   exec_generic(loc,opcode);  break;  // $5\n"; 
  #  $t=1;
  #}
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tEMIPS_FORMAT.*\t(.*)\t(.*)$/) {
    print o "      case $1:   exec_$5();  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tEPPC_FORMAT.*\t(.*)\t(.*)$/) {
    print o "      case $1:   exec_$5();  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tEx86_FORMAT_SPECIAL\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_special(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tEx86_FORMAT_UNIMPLEMENTED\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_unimplemented(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tEx86_FORMAT_PLUSSEG\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   exec_plusseg(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)$/) {
    if($t==0) {
      if($5 eq "\?\?\?") {
        print o "      case 0x$1:   exec_Illegal(loc,opcode);  break;  // $5\n"; 
      }
      else {
        $n1=$1;  $n2=$5;
        $n2 =~ tr/\./\_/;
        print o "      case 0x$n1:   exec_$n2(loc,opcode);  break;\n"; 
      }
    }
  }
};
$goof="_exec.h";
print o "      default:  exec_invalid(\"$name$goof\"); break;\n";
print o "  }\n";
# That's it!
close o;
close f;
