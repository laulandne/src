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
$goof="_dis.h";
open(o,">$name$goof");
print o "  // Don't edit this file, it is machine generated\n";
print o "  switch(topcode) {\n";
while(<f>) {
  $t=0;
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_COND_DISP\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_bcc(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_SPECIAL\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_special(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_UNIMPLEMENTED\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_unimplemented(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_A_LINE\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_a_line(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_F_LINE\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_f_line(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_COUNT_SIZE_REG\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_$5(loc,reg,size,reg2,dir);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_REG_DATA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_$5(loc,reg,reg2);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_$5(loc,mode,reg);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_SIZE_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_$5(loc,mode,reg,size);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_REG_MODE_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_$5(loc,reg2,mode2,mode,reg);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_DATA_SIZE_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_$5(loc,mode,reg,reg2,size);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_EA_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_$5(loc,topcode,reg2,mode2,mode,reg);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_REG_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_$5(loc,reg2,mode,reg);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_REG_LEA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_$5(loc,reg2,mode,reg);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tE68k_COND_EA\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_$5(loc,mode2,mode,reg);  break;\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tECPU_ADDR_NONE\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_generic(loc,opcode);  break;  // $5\n"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tEMIPS_FORMAT.*\t(.*)\t(.*)$/) {
    print o "      case $1:   dis_$5();  break;"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tEPPC_FORMAT.*\t(.*)\t(.*)$/) {
    print o "      case $1:   dis_$5();  break;"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tEx86_FORMAT_SPECIAL\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_special(loc,opcode);  break; // $5"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tEx86_FORMAT_UNIMPLEMENTED\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_unimplemented(loc,opcode);  break; // $5"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\tEx86_FORMAT_PLUSSEG\t(.*)\t(.*)$/) {
    print o "      case 0x$1:   dis_plusseg(loc,opcode);  break; // $5"; 
    $t=1;
  }
  if(/^(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)$/) {
    if($t==0) {
      print o "      case 0x$1:   dis_$5(loc,opcode);  break;  // $5\n"; 
    }
  }
};
$goof="_dis.h";
print o "      default:  dis_invalid(\"$name$goof\"); break;\n";
print o "  }\n";
# That's it!
close o;
close f;
