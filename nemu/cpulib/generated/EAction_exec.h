  // Don't edit this file, it is machine generated
  switch(topcode) {
      case 0x0:   exec_end(loc,opcode);  break;
      case 0x1:   exec_illegal(loc,opcode);  break;
      case 0x2:   exec_illegal(loc,opcode);  break;
      case 0x3:   exec_illegal(loc,opcode);  break;
      case 0x4:   exec_nextframe(loc,opcode);  break;
      case 0x5:   exec_prevframe(loc,opcode);  break;
      case 0x6:   exec_play(loc,opcode);  break;
      case 0x7:   exec_stop(loc,opcode);  break;
      case 0x8:   exec_togglequality(loc,opcode);  break;
      case 0x9:   exec_stopsounds(loc,opcode);  break;
      case 0x0A:   exec_add(loc,opcode);  break;
      case 0x0B:   exec_subtract(loc,opcode);  break;
      case 0x0C:   exec_multiply(loc,opcode);  break;
      case 0x0D:   exec_divide(loc,opcode);  break;
      case 0x0E:   exec_equals(loc,opcode);  break;
      case 0x0F:   exec_lessthan(loc,opcode);  break;
      case 0x10:   exec_logicaland(loc,opcode);  break;
      case 0x11:   exec_logicalor(loc,opcode);  break;
      case 0x12:   exec_logicalnot(loc,opcode);  break;
      case 0x13:   exec_stringeq(loc,opcode);  break;
      case 0x14:   exec_stringlength(loc,opcode);  break;
      case 0x15:   exec_substring(loc,opcode);  break;
      case 0x16:   exec_illegal(loc,opcode);  break;
      case 0x17:   exec_pop(loc,opcode);  break;
      case 0x18:   exec_int(loc,opcode);  break;
      case 0x19:   exec_illegal(loc,opcode);  break;
      case 0x1A:   exec_illegal(loc,opcode);  break;
      case 0x1B:   exec_illegal(loc,opcode);  break;
      case 0x1C:   exec_getvariable(loc,opcode);  break;
      case 0x1D:   exec_setvariable(loc,opcode);  break;
      case 0x1E:   exec_illegal(loc,opcode);  break;
      case 0x1F:   exec_illegal(loc,opcode);  break;
      case 0x20:   exec_settargetexpression(loc,opcode);  break;
      case 0x21:   exec_stringconcat(loc,opcode);  break;
      case 0x22:   exec_getproperty(loc,opcode);  break;
      case 0x23:   exec_setproperty(loc,opcode);  break;
      case 0x24:   exec_duplicateclip(loc,opcode);  break;
      case 0x25:   exec_removeclip(loc,opcode);  break;
      case 0x26:   exec_trace(loc,opcode);  break;
      case 0x27:   exec_startdragmovie(loc,opcode);  break;
      case 0x28:   exec_stopdragmovie(loc,opcode);  break;
      case 0x29:   exec_stringlessthan(loc,opcode);  break;
      case 0x2A:   exec_illegal(loc,opcode);  break;
      case 0x2B:   exec_illegal(loc,opcode);  break;
      case 0x2C:   exec_illegal(loc,opcode);  break;
      case 0x2D:   exec_illegal(loc,opcode);  break;
      case 0x2E:   exec_illegal(loc,opcode);  break;
      case 0x2F:   exec_illegal(loc,opcode);  break;
      case 0x30:   exec_random(loc,opcode);  break;
      case 0x31:   exec_mblength(loc,opcode);  break;
      case 0x32:   exec_ord(loc,opcode);  break;
      case 0x33:   exec_chr(loc,opcode);  break;
      case 0x34:   exec_gettimer(loc,opcode);  break;
      case 0x35:   exec_mbsubstring(loc,opcode);  break;
      case 0x36:   exec_mbord(loc,opcode);  break;
      case 0x37:   exec_mbchr(loc,opcode);  break;
      case 0x38:   exec_illegal(loc,opcode);  break;
      case 0x39:   exec_illegal(loc,opcode);  break;
      case 0x3A:   exec_illegal(loc,opcode);  break;
      case 0x3B:   exec_delete(loc,opcode);  break;
      case 0x3C:   exec_varequals(loc,opcode);  break;
      case 0x3D:   exec_callfunction(loc,opcode);  break;
      case 0x3E:   exec_return(loc,opcode);  break;
      case 0x3F:   exec_modulo(loc,opcode);  break;
      case 0x40:   exec_new(loc,opcode);  break;
      case 0x41:   exec_var(loc,opcode);  break;
      case 0x42:   exec_initarray(loc,opcode);  break;
      case 0x43:   exec_initobject(loc,opcode);  break;
      case 0x44:   exec_typeof(loc,opcode);  break;
      case 0x45:   exec_targetpath(loc,opcode);  break;
      case 0x46:   exec_enumerate(loc,opcode);  break;
      case 0x47:   exec_newadd(loc,opcode);  break;
      case 0x48:   exec_newlessthan(loc,opcode);  break;
      case 0x49:   exec_newequals(loc,opcode);  break;
      case 0x4A:   exec_illegal(loc,opcode);  break;
      case 0x4B:   exec_illegal(loc,opcode);  break;
      case 0x4C:   exec_dup(loc,opcode);  break;
      case 0x4D:   exec_swap(loc,opcode);  break;
      case 0x4E:   exec_getmember(loc,opcode);  break;
      case 0x4F:   exec_setmember(loc,opcode);  break;
      case 0x50:   exec_increment(loc,opcode);  break;
      case 0x51:   exec_decrement(loc,opcode);  break;
      case 0x52:   exec_callmethod(loc,opcode);  break;
      case 0x53:   exec_illegal(loc,opcode);  break;
      case 0x54:   exec_illegal(loc,opcode);  break;
      case 0x55:   exec_illegal(loc,opcode);  break;
      case 0x56:   exec_illegal(loc,opcode);  break;
      case 0x57:   exec_illegal(loc,opcode);  break;
      case 0x58:   exec_illegal(loc,opcode);  break;
      case 0x59:   exec_illegal(loc,opcode);  break;
      case 0x5A:   exec_illegal(loc,opcode);  break;
      case 0x5B:   exec_illegal(loc,opcode);  break;
      case 0x5C:   exec_illegal(loc,opcode);  break;
      case 0x5D:   exec_illegal(loc,opcode);  break;
      case 0x5E:   exec_illegal(loc,opcode);  break;
      case 0x5F:   exec_illegal(loc,opcode);  break;
      case 0x60:   exec_bitwiseand(loc,opcode);  break;
      case 0x61:   exec_bitwiseor(loc,opcode);  break;
      case 0x62:   exec_bitwisexor(loc,opcode);  break;
      case 0x63:   exec_shiftleft(loc,opcode);  break;
      case 0x64:   exec_shiftright(loc,opcode);  break;
      case 0x65:   exec_shiftright2(loc,opcode);  break;
      case 0x66:   exec_illegal(loc,opcode);  break;
      case 0x67:   exec_illegal(loc,opcode);  break;
      case 0x68:   exec_illegal(loc,opcode);  break;
      case 0x69:   exec_illegal(loc,opcode);  break;
      case 0x6A:   exec_illegal(loc,opcode);  break;
      case 0x6B:   exec_illegal(loc,opcode);  break;
      case 0x6C:   exec_illegal(loc,opcode);  break;
      case 0x6D:   exec_illegal(loc,opcode);  break;
      case 0x6E:   exec_illegal(loc,opcode);  break;
      case 0x6F:   exec_illegal(loc,opcode);  break;
      case 0x70:   exec_illegal(loc,opcode);  break;
      case 0x71:   exec_illegal(loc,opcode);  break;
      case 0x72:   exec_illegal(loc,opcode);  break;
      case 0x73:   exec_illegal(loc,opcode);  break;
      case 0x74:   exec_illegal(loc,opcode);  break;
      case 0x75:   exec_illegal(loc,opcode);  break;
      case 0x76:   exec_illegal(loc,opcode);  break;
      case 0x77:   exec_illegal(loc,opcode);  break;
      case 0x78:   exec_illegal(loc,opcode);  break;
      case 0x79:   exec_illegal(loc,opcode);  break;
      case 0x7A:   exec_illegal(loc,opcode);  break;
      case 0x7B:   exec_illegal(loc,opcode);  break;
      case 0x7C:   exec_illegal(loc,opcode);  break;
      case 0x7D:   exec_illegal(loc,opcode);  break;
      case 0x7E:   exec_illegal(loc,opcode);  break;
      case 0x7F:   exec_illegal(loc,opcode);  break;
      case 0x80:   exec_illegal(loc,opcode);  break;
      case 0x81:   exec_gotoframe(loc,opcode);  break;
      case 0x82:   exec_illegal(loc,opcode);  break;
      case 0x83:   exec_geturl(loc,opcode);  break;
      case 0x84:   exec_illegal(loc,opcode);  break;
      case 0x85:   exec_illegal(loc,opcode);  break;
      case 0x86:   exec_illegal(loc,opcode);  break;
      case 0x87:   exec_setregister(loc,opcode);  break;
      case 0x88:   exec_constantpool(loc,opcode);  break;
      case 0x89:   exec_strict_mode(loc,opcode);  break;
      case 0x8A:   exec_waitforframe(loc,opcode);  break;
      case 0x8B:   exec_settarget(loc,opcode);  break;
      case 0x8C:   exec_gotolabel(loc,opcode);  break;
      case 0x8D:   exec_waitforframeexpression(loc,opcode);  break;
      case 0x8E:   exec_declarefunction2(loc,opcode);  break;
      case 0x8F:   exec_try(loc,opcode);  break;
      case 0x90:   exec_illegal(loc,opcode);  break;
      case 0x91:   exec_illegal(loc,opcode);  break;
      case 0x92:   exec_illegal(loc,opcode);  break;
      case 0x93:   exec_illegal(loc,opcode);  break;
      case 0x94:   exec_with(loc,opcode);  break;
      case 0x95:   exec_illegal(loc,opcode);  break;
      case 0x96:   exec_pushdata(loc,opcode);  break;
      case 0x97:   exec_illegal(loc,opcode);  break;
      case 0x98:   exec_illegal(loc,opcode);  break;
      case 0x99:   exec_branchalways(loc,opcode);  break;
      case 0x9A:   exec_geturl2(loc,opcode);  break;
      case 0x9B:   exec_definefunction(loc,opcode);  break;
      case 0x9C:   exec_illegal(loc,opcode);  break;
      case 0x9D:   exec_branchiftrue(loc,opcode);  break;
      case 0x9E:   exec_callframe(loc,opcode);  break;
      case 0x9F:   exec_gotoexpression(loc,opcode);  break;
      case 0xA0:   exec_illegal(loc,opcode);  break;
      case 0xA1:   exec_illegal(loc,opcode);  break;
      case 0xA2:   exec_illegal(loc,opcode);  break;
      case 0xA3:   exec_illegal(loc,opcode);  break;
      case 0xA4:   exec_illegal(loc,opcode);  break;
      case 0xA5:   exec_illegal(loc,opcode);  break;
      case 0xA6:   exec_illegal(loc,opcode);  break;
      case 0xA7:   exec_illegal(loc,opcode);  break;
      case 0xA8:   exec_illegal(loc,opcode);  break;
      case 0xA9:   exec_illegal(loc,opcode);  break;
      case 0xAA:   exec_illegal(loc,opcode);  break;
      case 0xAB:   exec_illegal(loc,opcode);  break;
      case 0xAC:   exec_illegal(loc,opcode);  break;
      case 0xAD:   exec_illegal(loc,opcode);  break;
      case 0xAE:   exec_illegal(loc,opcode);  break;
      case 0xAF:   exec_illegal(loc,opcode);  break;
      case 0xB0:   exec_illegal(loc,opcode);  break;
      case 0xB1:   exec_illegal(loc,opcode);  break;
      case 0xB2:   exec_illegal(loc,opcode);  break;
      case 0xB3:   exec_illegal(loc,opcode);  break;
      case 0xB4:   exec_illegal(loc,opcode);  break;
      case 0xB5:   exec_illegal(loc,opcode);  break;
      case 0xB6:   exec_illegal(loc,opcode);  break;
      case 0xB7:   exec_illegal(loc,opcode);  break;
      case 0xB8:   exec_illegal(loc,opcode);  break;
      case 0xB9:   exec_illegal(loc,opcode);  break;
      case 0xBA:   exec_illegal(loc,opcode);  break;
      case 0xBB:   exec_illegal(loc,opcode);  break;
      case 0xBC:   exec_illegal(loc,opcode);  break;
      case 0xBD:   exec_illegal(loc,opcode);  break;
      case 0xBE:   exec_illegal(loc,opcode);  break;
      case 0xBF:   exec_illegal(loc,opcode);  break;
      case 0xC0:   exec_illegal(loc,opcode);  break;
      case 0xC1:   exec_illegal(loc,opcode);  break;
      case 0xC2:   exec_illegal(loc,opcode);  break;
      case 0xC3:   exec_illegal(loc,opcode);  break;
      case 0xC4:   exec_illegal(loc,opcode);  break;
      case 0xC5:   exec_illegal(loc,opcode);  break;
      case 0xC6:   exec_illegal(loc,opcode);  break;
      case 0xC7:   exec_illegal(loc,opcode);  break;
      case 0xC8:   exec_illegal(loc,opcode);  break;
      case 0xC9:   exec_illegal(loc,opcode);  break;
      case 0xCA:   exec_illegal(loc,opcode);  break;
      case 0xCB:   exec_illegal(loc,opcode);  break;
      case 0xCC:   exec_illegal(loc,opcode);  break;
      case 0xCD:   exec_illegal(loc,opcode);  break;
      case 0xCE:   exec_illegal(loc,opcode);  break;
      case 0xCF:   exec_illegal(loc,opcode);  break;
      case 0xD0:   exec_illegal(loc,opcode);  break;
      case 0xD1:   exec_illegal(loc,opcode);  break;
      case 0xD2:   exec_illegal(loc,opcode);  break;
      case 0xD3:   exec_illegal(loc,opcode);  break;
      case 0xD4:   exec_illegal(loc,opcode);  break;
      case 0xD5:   exec_illegal(loc,opcode);  break;
      case 0xD6:   exec_illegal(loc,opcode);  break;
      case 0xD7:   exec_illegal(loc,opcode);  break;
      case 0xD8:   exec_illegal(loc,opcode);  break;
      case 0xD9:   exec_illegal(loc,opcode);  break;
      case 0xDA:   exec_illegal(loc,opcode);  break;
      case 0xDB:   exec_illegal(loc,opcode);  break;
      case 0xDC:   exec_illegal(loc,opcode);  break;
      case 0xDD:   exec_illegal(loc,opcode);  break;
      case 0xDE:   exec_illegal(loc,opcode);  break;
      case 0xDF:   exec_illegal(loc,opcode);  break;
      case 0xE0:   exec_illegal(loc,opcode);  break;
      case 0xE1:   exec_illegal(loc,opcode);  break;
      case 0xE2:   exec_illegal(loc,opcode);  break;
      case 0xE3:   exec_illegal(loc,opcode);  break;
      case 0xE4:   exec_illegal(loc,opcode);  break;
      case 0xE5:   exec_illegal(loc,opcode);  break;
      case 0xE6:   exec_illegal(loc,opcode);  break;
      case 0xE7:   exec_illegal(loc,opcode);  break;
      case 0xE8:   exec_illegal(loc,opcode);  break;
      case 0xE9:   exec_illegal(loc,opcode);  break;
      case 0xEA:   exec_illegal(loc,opcode);  break;
      case 0xEB:   exec_illegal(loc,opcode);  break;
      case 0xEC:   exec_illegal(loc,opcode);  break;
      case 0xED:   exec_illegal(loc,opcode);  break;
      case 0xEE:   exec_illegal(loc,opcode);  break;
      case 0xEF:   exec_illegal(loc,opcode);  break;
      case 0xF0:   exec_illegal(loc,opcode);  break;
      case 0xF1:   exec_illegal(loc,opcode);  break;
      case 0xF2:   exec_illegal(loc,opcode);  break;
      case 0xF3:   exec_illegal(loc,opcode);  break;
      case 0xF4:   exec_illegal(loc,opcode);  break;
      case 0xF5:   exec_illegal(loc,opcode);  break;
      case 0xF6:   exec_illegal(loc,opcode);  break;
      case 0xF7:   exec_illegal(loc,opcode);  break;
      case 0xF8:   exec_illegal(loc,opcode);  break;
      case 0xF9:   exec_illegal(loc,opcode);  break;
      case 0xFA:   exec_illegal(loc,opcode);  break;
      case 0xFB:   exec_illegal(loc,opcode);  break;
      case 0xFC:   exec_illegal(loc,opcode);  break;
      case 0xFD:   exec_illegal(loc,opcode);  break;
      case 0xFE:   exec_illegal(loc,opcode);  break;
      case 0xFF:   exec_illegal(loc,opcode);  break;
      default:  exec_invalid("EAction_exec.h"); break;
  }