  // Don't edit this file, it is machine generated
  switch(topcode) {
      case 0x0:   exec_special0(loc,opcode);  break;
      case 0x1:   exec_mov_l(loc,opcode);  break;
      case 0x2:   exec_mov_l(loc,opcode);  break;
      case 0x3:   exec_illegal(loc,opcode);  break;
      case 0x4:   exec_special4(loc,opcode);  break;
      case 0x5:   exec_mov(loc,opcode);  break;
      case 0x6:   exec_mov(loc,opcode);  break;
      case 0x7:   exec_add(loc,opcode);  break;
      case 0x8:   exec_bcc(loc,opcode);  break;
      case 0x9:   exec_mov_w(loc,opcode);  break;
      case 0x0A:   exec_illegal(loc,opcode);  break;
      case 0x0B:   exec_bra(loc,opcode);  break;
      case 0x0C:   exec_bsr(loc,opcode);  break;
      case 0x0D:   exec_mov_l(loc,opcode);  break;
      case 0x0E:   exec_mov(loc,opcode);  break;
      case 0x0F:   exec_illegal(loc,opcode);  break;
      default:  exec_invalid("ESh_exec.h"); break;
  }
