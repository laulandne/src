  // Don't edit this file, it is machine generated
  switch(topcode) {
      case 0x0:   exec_test(loc,opcode);  break;
      case 0x1:   exec_nop(loc,opcode);  break;
      case 0x2:   exec_idiv(loc,opcode);  break;
      case 0x3:   exec_fdiv(loc,opcode);  break;
      case 0x4:   exec_lsrd(loc,opcode);  break;
      case 0x5:   exec_asrd(loc,opcode);  break;
      case 0x6:   exec_tap(loc,opcode);  break;
      case 0x7:   exec_tpa(loc,opcode);  break;
      case 0x8:   exec_inx(loc,opcode);  break;
      case 0x9:   exec_dex(loc,opcode);  break;
      case 0x0A:   exec_clv(loc,opcode);  break;
      case 0x0B:   exec_sev(loc,opcode);  break;
      case 0x0C:   exec_clc(loc,opcode);  break;
      case 0x0D:   exec_sec(loc,opcode);  break;
      case 0x0E:   exec_cli(loc,opcode);  break;
      case 0x0F:   exec_sei(loc,opcode);  break;
      case 0x10:   exec_sba(loc,opcode);  break;
      case 0x11:   exec_cba(loc,opcode);  break;
      case 0x12:   exec_brset(loc,opcode);  break;
      case 0x13:   exec_brclr(loc,opcode);  break;
      case 0x14:   exec_bset(loc,opcode);  break;
      case 0x15:   exec_bclr(loc,opcode);  break;
      case 0x16:   exec_tab(loc,opcode);  break;
      case 0x17:   exec_tba(loc,opcode);  break;
      case 0x18:   exec_special(loc,opcode);  break;
      case 0x19:   exec_daa(loc,opcode);  break;
      case 0x1A:   exec_special(loc,opcode);  break;
      case 0x1B:   exec_aba(loc,opcode);  break;
      case 0x1C:   exec_bset(loc,opcode);  break;
      case 0x1D:   exec_bclr(loc,opcode);  break;
      case 0x1E:   exec_brset(loc,opcode);  break;
      case 0x1F:   exec_brclr(loc,opcode);  break;
      case 0x20:   exec_bra(loc,opcode);  break;
      case 0x21:   exec_brn(loc,opcode);  break;
      case 0x22:   exec_bhi(loc,opcode);  break;
      case 0x23:   exec_bls(loc,opcode);  break;
      case 0x24:   exec_bcc(loc,opcode);  break;
      case 0x25:   exec_bcs(loc,opcode);  break;
      case 0x26:   exec_bne(loc,opcode);  break;
      case 0x27:   exec_beq(loc,opcode);  break;
      case 0x28:   exec_bvc(loc,opcode);  break;
      case 0x29:   exec_bvs(loc,opcode);  break;
      case 0x2A:   exec_bpl(loc,opcode);  break;
      case 0x2B:   exec_bmi(loc,opcode);  break;
      case 0x2C:   exec_bge(loc,opcode);  break;
      case 0x2D:   exec_blt(loc,opcode);  break;
      case 0x2E:   exec_bgt(loc,opcode);  break;
      case 0x2F:   exec_ble(loc,opcode);  break;
      case 0x30:   exec_tsx(loc,opcode);  break;
      case 0x31:   exec_ins(loc,opcode);  break;
      case 0x32:   exec_pula(loc,opcode);  break;
      case 0x33:   exec_pulb(loc,opcode);  break;
      case 0x34:   exec_des(loc,opcode);  break;
      case 0x35:   exec_txs(loc,opcode);  break;
      case 0x36:   exec_psha(loc,opcode);  break;
      case 0x37:   exec_pshb(loc,opcode);  break;
      case 0x38:   exec_pulx(loc,opcode);  break;
      case 0x39:   exec_rts(loc,opcode);  break;
      case 0x3A:   exec_abx(loc,opcode);  break;
      case 0x3B:   exec_rti(loc,opcode);  break;
      case 0x3C:   exec_pshx(loc,opcode);  break;
      case 0x3D:   exec_mul(loc,opcode);  break;
      case 0x3E:   exec_wai(loc,opcode);  break;
      case 0x3F:   exec_swi(loc,opcode);  break;
      case 0x40:   exec_nega(loc,opcode);  break;
      case 0x41:   exec_unknown(loc,opcode);  break;
      case 0x42:   exec_unknown(loc,opcode);  break;
      case 0x43:   exec_coma(loc,opcode);  break;
      case 0x44:   exec_lsra(loc,opcode);  break;
      case 0x45:   exec_unknown(loc,opcode);  break;
      case 0x46:   exec_rora(loc,opcode);  break;
      case 0x47:   exec_asra(loc,opcode);  break;
      case 0x48:   exec_asla(loc,opcode);  break;
      case 0x49:   exec_rola(loc,opcode);  break;
      case 0x4A:   exec_deca(loc,opcode);  break;
      case 0x4B:   exec_illegal(loc,opcode);  break;
      case 0x4C:   exec_inca(loc,opcode);  break;
      case 0x4D:   exec_tsta(loc,opcode);  break;
      case 0x4E:   exec_unknown(loc,opcode);  break;
      case 0x4F:   exec_clra(loc,opcode);  break;
      case 0x50:   exec_negb(loc,opcode);  break;
      case 0x51:   exec_unknown(loc,opcode);  break;
      case 0x52:   exec_unknown(loc,opcode);  break;
      case 0x53:   exec_comb(loc,opcode);  break;
      case 0x54:   exec_lsrb(loc,opcode);  break;
      case 0x55:   exec_unknown(loc,opcode);  break;
      case 0x56:   exec_rorb(loc,opcode);  break;
      case 0x57:   exec_asrb(loc,opcode);  break;
      case 0x58:   exec_aslb(loc,opcode);  break;
      case 0x59:   exec_rolb(loc,opcode);  break;
      case 0x5A:   exec_decb(loc,opcode);  break;
      case 0x5B:   exec_illegal(loc,opcode);  break;
      case 0x5C:   exec_incb(loc,opcode);  break;
      case 0x5D:   exec_tstb(loc,opcode);  break;
      case 0x5E:   exec_illegal(loc,opcode);  break;
      case 0x5F:   exec_clrb(loc,opcode);  break;
      case 0x60:   exec_neg(loc,opcode);  break;
      case 0x61:   exec_illegal(loc,opcode);  break;
      case 0x62:   exec_unknown(loc,opcode);  break;
      case 0x63:   exec_com(loc,opcode);  break;
      case 0x64:   exec_lsr(loc,opcode);  break;
      case 0x65:   exec_unknown(loc,opcode);  break;
      case 0x66:   exec_ror(loc,opcode);  break;
      case 0x67:   exec_asr(loc,opcode);  break;
      case 0x68:   exec_asl(loc,opcode);  break;
      case 0x69:   exec_rol(loc,opcode);  break;
      case 0x6A:   exec_dec(loc,opcode);  break;
      case 0x6B:   exec_illegal(loc,opcode);  break;
      case 0x6C:   exec_inc(loc,opcode);  break;
      case 0x6D:   exec_tst(loc,opcode);  break;
      case 0x6E:   exec_jmp(loc,opcode);  break;
      case 0x6F:   exec_clr(loc,opcode);  break;
      case 0x70:   exec_neg(loc,opcode);  break;
      case 0x71:   exec_unknown(loc,opcode);  break;
      case 0x72:   exec_unknown(loc,opcode);  break;
      case 0x73:   exec_com(loc,opcode);  break;
      case 0x74:   exec_lsr(loc,opcode);  break;
      case 0x75:   exec_unknown(loc,opcode);  break;
      case 0x76:   exec_ror(loc,opcode);  break;
      case 0x77:   exec_asr(loc,opcode);  break;
      case 0x78:   exec_asl(loc,opcode);  break;
      case 0x79:   exec_rol(loc,opcode);  break;
      case 0x7A:   exec_dec(loc,opcode);  break;
      case 0x7B:   exec_illegal(loc,opcode);  break;
      case 0x7C:   exec_inc(loc,opcode);  break;
      case 0x7D:   exec_tst(loc,opcode);  break;
      case 0x7E:   exec_jmp(loc,opcode);  break;
      case 0x7F:   exec_clr(loc,opcode);  break;
      case 0x80:   exec_suba(loc,opcode);  break;
      case 0x81:   exec_cmpa(loc,opcode);  break;
      case 0x82:   exec_sbca(loc,opcode);  break;
      case 0x83:   exec_subd(loc,opcode);  break;
      case 0x84:   exec_anda(loc,opcode);  break;
      case 0x85:   exec_bita(loc,opcode);  break;
      case 0x86:   exec_lda(loc,opcode);  break;
      case 0x87:   exec_illegal(loc,opcode);  break;
      case 0x88:   exec_eora(loc,opcode);  break;
      case 0x89:   exec_adca(loc,opcode);  break;
      case 0x8A:   exec_ora(loc,opcode);  break;
      case 0x8B:   exec_adda(loc,opcode);  break;
      case 0x8C:   exec_cmpx(loc,opcode);  break;
      case 0x8D:   exec_bsr(loc,opcode);  break;
      case 0x8E:   exec_lds(loc,opcode);  break;
      case 0x8F:   exec_xgdx(loc,opcode);  break;
      case 0x90:   exec_suba(loc,opcode);  break;
      case 0x91:   exec_cmpa(loc,opcode);  break;
      case 0x92:   exec_sbca(loc,opcode);  break;
      case 0x93:   exec_subd(loc,opcode);  break;
      case 0x94:   exec_anda(loc,opcode);  break;
      case 0x95:   exec_bita(loc,opcode);  break;
      case 0x96:   exec_lda(loc,opcode);  break;
      case 0x97:   exec_sta(loc,opcode);  break;
      case 0x98:   exec_eora(loc,opcode);  break;
      case 0x99:   exec_adca(loc,opcode);  break;
      case 0x9A:   exec_ora(loc,opcode);  break;
      case 0x9B:   exec_adda(loc,opcode);  break;
      case 0x9C:   exec_cpx(loc,opcode);  break;
      case 0x9D:   exec_jsr(loc,opcode);  break;
      case 0x9E:   exec_lds(loc,opcode);  break;
      case 0x9F:   exec_sts(loc,opcode);  break;
      case 0xA0:   exec_suba(loc,opcode);  break;
      case 0xA1:   exec_cmpa(loc,opcode);  break;
      case 0xA2:   exec_adda(loc,opcode);  break;
      case 0xA3:   exec_subd(loc,opcode);  break;
      case 0xA4:   exec_anda(loc,opcode);  break;
      case 0xA5:   exec_bita(loc,opcode);  break;
      case 0xA6:   exec_lda(loc,opcode);  break;
      case 0xA7:   exec_sta(loc,opcode);  break;
      case 0xA8:   exec_eora(loc,opcode);  break;
      case 0xA9:   exec_adca(loc,opcode);  break;
      case 0xAA:   exec_ora(loc,opcode);  break;
      case 0xAB:   exec_adda(loc,opcode);  break;
      case 0xAC:   exec_cpx(loc,opcode);  break;
      case 0xAD:   exec_jsr(loc,opcode);  break;
      case 0xAE:   exec_ldx(loc,opcode);  break;
      case 0xAF:   exec_stx(loc,opcode);  break;
      case 0xB0:   exec_suba(loc,opcode);  break;
      case 0xB1:   exec_cmpa(loc,opcode);  break;
      case 0xB2:   exec_sbca(loc,opcode);  break;
      case 0xB3:   exec_subd(loc,opcode);  break;
      case 0xB4:   exec_anda(loc,opcode);  break;
      case 0xB5:   exec_bita(loc,opcode);  break;
      case 0xB6:   exec_lda(loc,opcode);  break;
      case 0xB7:   exec_sta(loc,opcode);  break;
      case 0xB8:   exec_eora(loc,opcode);  break;
      case 0xB9:   exec_adca(loc,opcode);  break;
      case 0xBA:   exec_ora(loc,opcode);  break;
      case 0xBB:   exec_adda(loc,opcode);  break;
      case 0xBC:   exec_cpx(loc,opcode);  break;
      case 0xBD:   exec_jsr(loc,opcode);  break;
      case 0xBE:   exec_lds(loc,opcode);  break;
      case 0xBF:   exec_sts(loc,opcode);  break;
      case 0xC0:   exec_subb(loc,opcode);  break;
      case 0xC1:   exec_cmpb(loc,opcode);  break;
      case 0xC2:   exec_sbcb(loc,opcode);  break;
      case 0xC3:   exec_addd(loc,opcode);  break;
      case 0xC4:   exec_andb(loc,opcode);  break;
      case 0xC5:   exec_bitb(loc,opcode);  break;
      case 0xC6:   exec_ldb(loc,opcode);  break;
      case 0xC7:   exec_illegal(loc,opcode);  break;
      case 0xC8:   exec_eorb(loc,opcode);  break;
      case 0xC9:   exec_adcb(loc,opcode);  break;
      case 0xCA:   exec_orb(loc,opcode);  break;
      case 0xCB:   exec_addb(loc,opcode);  break;
      case 0xCC:   exec_ldd(loc,opcode);  break;
      case 0xCD:   exec_special(loc,opcode);  break;
      case 0xCE:   exec_ldx(loc,opcode);  break;
      case 0xCF:   exec_stop(loc,opcode);  break;
      case 0xD0:   exec_subb(loc,opcode);  break;
      case 0xD1:   exec_cmpb(loc,opcode);  break;
      case 0xD2:   exec_sbcb(loc,opcode);  break;
      case 0xD3:   exec_addd(loc,opcode);  break;
      case 0xD4:   exec_andb(loc,opcode);  break;
      case 0xD5:   exec_bitb(loc,opcode);  break;
      case 0xD6:   exec_ldb(loc,opcode);  break;
      case 0xD7:   exec_stb(loc,opcode);  break;
      case 0xD8:   exec_eorb(loc,opcode);  break;
      case 0xD9:   exec_adcb(loc,opcode);  break;
      case 0xDA:   exec_orb(loc,opcode);  break;
      case 0xDB:   exec_addb(loc,opcode);  break;
      case 0xDC:   exec_ldd(loc,opcode);  break;
      case 0xDD:   exec_std(loc,opcode);  break;
      case 0xDE:   exec_ldx(loc,opcode);  break;
      case 0xDF:   exec_stx(loc,opcode);  break;
      case 0xE0:   exec_subb(loc,opcode);  break;
      case 0xE1:   exec_cmpb(loc,opcode);  break;
      case 0xE2:   exec_sbcb(loc,opcode);  break;
      case 0xE3:   exec_addd(loc,opcode);  break;
      case 0xE4:   exec_andb(loc,opcode);  break;
      case 0xE5:   exec_bitb(loc,opcode);  break;
      case 0xE6:   exec_ldb(loc,opcode);  break;
      case 0xE7:   exec_stb(loc,opcode);  break;
      case 0xE8:   exec_eorb(loc,opcode);  break;
      case 0xE9:   exec_adcb(loc,opcode);  break;
      case 0xEA:   exec_orb(loc,opcode);  break;
      case 0xEB:   exec_addb(loc,opcode);  break;
      case 0xEC:   exec_ldd(loc,opcode);  break;
      case 0xED:   exec_std(loc,opcode);  break;
      case 0xEE:   exec_ldx(loc,opcode);  break;
      case 0xEF:   exec_stx(loc,opcode);  break;
      case 0xF0:   exec_subb(loc,opcode);  break;
      case 0xF1:   exec_cmpb(loc,opcode);  break;
      case 0xF2:   exec_sbcb(loc,opcode);  break;
      case 0xF3:   exec_addd(loc,opcode);  break;
      case 0xF4:   exec_andb(loc,opcode);  break;
      case 0xF5:   exec_bitb(loc,opcode);  break;
      case 0xF6:   exec_ldb(loc,opcode);  break;
      case 0xF7:   exec_stb(loc,opcode);  break;
      case 0xF8:   exec_eorb(loc,opcode);  break;
      case 0xF9:   exec_adcb(loc,opcode);  break;
      case 0xFA:   exec_orb(loc,opcode);  break;
      case 0xFB:   exec_addb(loc,opcode);  break;
      case 0xFC:   exec_ldd(loc,opcode);  break;
      case 0xFD:   exec_std(loc,opcode);  break;
      case 0xFE:   exec_ldx(loc,opcode);  break;
      case 0xFF:   exec_stx(loc,opcode);  break;
      default:  exec_invalid("Em6811_exec.h"); break;
  }