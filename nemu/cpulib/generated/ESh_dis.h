  // Don't edit this file, it is machine generated
  switch(topcode) {
      case 0x0:   dis_special0(loc,opcode);  break;  // special0
      case 0x1:   dis_mov_l(loc,opcode);  break;  // mov_l
      case 0x2:   dis_mov_l(loc,opcode);  break;  // mov_l
      case 0x3:   dis_generic(loc,opcode);  break;  // illegal
      case 0x4:   dis_special4(loc,opcode);  break;  // special4
      case 0x5:   dis_mov(loc,opcode);  break;  // mov
      case 0x6:   dis_mov(loc,opcode);  break;  // mov
      case 0x7:   dis_add(loc,opcode);  break;  // add
      case 0x8:   dis_bcc(loc,opcode);  break;  // bcc
      case 0x9:   dis_mov_w(loc,opcode);  break;  // mov_w
      case 0x0A:   dis_generic(loc,opcode);  break;  // illegal
      case 0x0B:   dis_bra(loc,opcode);  break;  // bra
      case 0x0C:   dis_bsr(loc,opcode);  break;  // bsr
      case 0x0D:   dis_mov_l(loc,opcode);  break;  // mov_l
      case 0x0E:   dis_mov(loc,opcode);  break;  // mov
      case 0x0F:   dis_generic(loc,opcode);  break;  // illegal
      default:  dis_invalid("ESh_dis.h"); break;
  }
