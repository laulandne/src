
// One of the following needs to be defined to specify the kind of CPU we're building
// Em6502
// Em6509
// Em65c02
// Em4510
// Em65816


// NOTE: This MUST be included multiple times...
#ifdef __MWERKS__
#pragma once off
#endif // __MWERKS__


bool theClassType::execMain()
{
  // NOTE: When adding code below, use op and opw for temp values...
  BYTE newP=0,mask=0;
  unsigned int cflag=0;
  bool oldMode=false;
  if(true) {
    switch(opcode) {
      case 0x00:  // BRK
        handle_brk();
        break;
      case 0x01:  // ORA
        SET_A(a|VAL_INDX(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x02:
#ifdef e65x4510
        // CLE
        // Clear stack extend disable bit
        stackMask=0xffff;
        sp=stackBase+sp;  // NOTE: Is this right or will it just keep pushing it higher?
        stackBase=0;
        stackExtend=true;
        statusReg&=(0xff-E6502_STAT_ExtendDisable);
        setStatusReg(getStatusReg());  // make sure its set right...
        return true;
#endif // e65x4510
#ifdef e65816
        // COP
#endif // e65816
        return false;
        break;
      case 0x03:
#ifdef e65x4510
        // SEE
        // Set stack extend disable bit
        stackMask=0x00ff;
        stackBase=sp&0xff00;
        sp&=0xff;
        stackExtend=false;
        statusReg|=E6502_STAT_ExtendDisable;
        setStatusReg(getStatusReg());  // make sure its set right...
        return true;
#endif // e65x4510
#ifdef e65816
        // ORA d,s
#endif // e65816
        return false;
        break;
      case 0x04:
#ifndef e6502
        // TSB
        ea=ADDR_ZP();
        op=READ_DATA(ea);
        sbits->z=((a&op)==0);
        WRITE_DATA(ea,op|a);
        return true;
#endif // e6502
        return false;
        break;
      case 0x05:  // ORA
        SET_A(a|VAL_ZP(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x06:  // ASL
        op=VAL_ZP(M16BIT);
        sbits->c=(op&MHIGHBIT)!=0;
        op=(op<<1)&(MSIZEMASK-1);
        WRITE_DATA(ADDR_ZP(),op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0x07:
#ifdef e65x4510
        // RMB0
        op=VAL_ZP(M16BIT);
        op&=0xfe;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // ORA [d]
#endif // e65816
        return false;
        break;
      case 0x08:  // PHP
        STACK_PUSH(getStatusReg());
        break;
      case 0x09:  // ORA
        SET_A(a|VAL_IMM(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        ADJ_OP_LEN(M16BIT);
        break;
      case 0x0a:  // ASL
        op=a;
        sbits->c=(op&MHIGHBIT)!=0;
        op=(op<<1)&(MSIZEMASK-1);
        SET_A(op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0x0b:
#ifdef e65x4510
        // TSY or TSHY (transfer stack high to y)
        SET_Y((sp>>8)&0xff);
        set_flags_nz(y,MSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // PHD s
#endif // e65816
        return false;
        break;
      case 0x0c:
#ifndef e6502
        // TSB
        ea=ADDR_ABS();
        op=READ_DATA(ea);
        sbits->z=((a&op)==0);
        WRITE_DATA(ea,op&(0xff-a));
        return true;
#endif // e6502
        return false;
        break;
      case 0x0d:  // ORA
        SET_A(a|VAL_ABS(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x0e:  // ASL
        op=VAL_ABS(M16BIT);
        sbits->c=(op&MHIGHBIT)!=0;
        op=(op<<1)&(MSIZEMASK-1);
        WRITE_DATA(ADDR_ABS(),op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0x0f:
#ifdef e65x4510
        // BBR0
        op=VAL_ZP(M16BIT);
        if(!(op&0x01)) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // ORA al
#endif // e65816
        return false;
        break;
      case 0x10:  // BPL
        if(!(getStatusReg()&E6502_STAT_N)) pc+=(SBYTE)READ_CODE(pc+1);
        break;
      case 0x11:  // ORA
        SET_A(a|VAL_INDY(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x12:
#ifdef e65x4510
        // ORA
        SET_A(a|VAL_INDZ(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // ORA (d)
#endif // e65816
        return false;
        break;
      case 0x13:
#ifdef e65x4510
        // BPL (word rel)
        if(!(getStatusReg()&E6502_STAT_N)) pc+=(SINT16)READW_CODE(pc+1);
        return true;
#endif // e65x4510
#ifdef e65816
        // ORA (d,s),y
#endif // e65816
        return false;
        break;
      case 0x14:
#ifdef e65x4510
        // TRB
        // "test and reset bits"
        ea=ADDR_ZP();
        op=READ_DATA(ea);
        sbits->z=((a&op)==0);
        WRITE_DATA(ea,op|a);
        return true;
#endif // e65x4510
#ifdef e65816
        // TRB d
#endif // e65816
        return false;
        break;
      case 0x15:  // ORA
        SET_A(a|VAL_ZPX(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x16:  // ASL
        op=VAL_ZPX(M16BIT);
        sbits->c=(op&MHIGHBIT)!=0;
        op=(op<<1)&(MSIZEMASK-1);
        WRITE_DATA(ADDR_ZPX(),op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0x17:
#ifdef e65x4510
        // RMB1
        op=VAL_ZP(M16BIT);
        op&=0xfd;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // ORA [d],y
#endif // e65816
        return false;
        break;
      case 0x18:  // CLC
        resetStatusBits(E6502_STAT_C);
        break;
      case 0x19:  // ORA
        SET_A(a|VAL_ABSY(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x1a:
#ifndef e6502
        // INA or INC or INC A or whatever...
        SET_A(a+1);
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e6502
        return false;
        break;
      case 0x1b:
#ifdef e65x4510
        // INZ
        SET_Z(z+1);
        set_flags_nz(z,XSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // TCS
        if(nativeMode) { sp=a; }
	else { sp=a&0xff; }
        return true;
 #endif // e65816
        return false;
        break;
      case 0x1c:
#ifdef e65x4510
        // TRB
        // "test and reset bits"
        ea=ADDR_ABS();
        op=READ_DATA(ea);
        sbits->z=((a&op)==0);
        WRITE_DATA(ea,op|a);
        return true;
#endif // e65x4510
#ifdef e65816
        // TCS
#endif // e65816
         return false;
        break;
      case 0x1d:  // ORA
        SET_A(a|VAL_ABSX(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x1e:  // ASL
        op=VAL_ABSX(M16BIT);
        sbits->c=(op&MHIGHBIT)!=0;
        op=(op<<1)&(MSIZEMASK-1);
        WRITE_DATA(ADDR_ABSX(),op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0x1f:
#ifdef e65x4510
        // BBR1
        op=VAL_ZP(M16BIT);
        if(!(op&0x02)) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // ORA al,x
#endif // e65816
        return false;
        break;
      case 0x20:  // JSR
        STACK_PUSHW(pc+2);
        ea=READW_CODE(pc+1);
        stackTracePush(ea);
        jump(ea);
        break;
      case 0x21:  // AND
        SET_A(a&VAL_INDY(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x22:
#ifdef e65x4510
        // JSR (indirect)
        STACK_PUSHW(pc+2);
        ea=READW_CODE(pc+1);
        ea=READW_DATA(ea);
        stackTracePush(ea);
        jump(ea);
        return true;
#endif // e65x4510
#ifdef e65816
       // JSR (long, JSL or I call it JSRL)
        STACK_PUSH(pbr>>16);
        STACK_PUSHW(pc+3);
        ea=READW_CODE(pc+1);
        stackTracePush(ea);
        pbr=(READ_CODE(pc+3)<<16)&0xff0000;
        jump(ea);
        return true;
#endif // e65816
        return false;
        break;
      case 0x23:
#ifdef e65816
        // JSRL
#endif // e65816
        return false;
        break;
      case 0x24:  // BIT
        op=VAL_ZP(M16BIT);
        sbits->z=((a&op)==0);
        sbits->n=((op&(BYTE)MHIGHBIT)!=0);
        sbits->v=((op&(BYTE)MHALFBIT)!=0);
        break;
      case 0x25:  // AND
        SET_A(a&VAL_ZP(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x26:  // ROL
        op=VAL_ZP(M16BIT);
        val=(op<<1)&(MSIZEMASK-1);
        if(sbits->c) val++;
        WRITE_DATA(ADDR_ZP(),val);
        sbits->c=((op&MHIGHBIT)==MHIGHBIT);
        set_flags_nz(val,MSIZEMASK);
        break;
      case 0x27:
#ifdef e65x4510
        // RMB2
        op=VAL_ZP(M16BIT);
        op&=0xfb;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // AND
#endif // e65816
        return false;
        break;
      case 0x28:  // PLP
        newP=STACK_POP();
        setStatusReg(newP);
        break;
      case 0x29:  // AND
        SET_A(a&VAL_IMM(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        ADJ_OP_LEN(M16BIT);
        break;
      case 0x2a:  // ROL
        op=a;
        val=(op<<1)&(MSIZEMASK-1);
        if(sbits->c) val++;
        SET_A(val);
        sbits->c=((op&MHIGHBIT)==MHIGHBIT);
        set_flags_nz(val,MSIZEMASK);
        break;
      case 0x2b:
#ifdef e65x4510
        // TYS or TYSH
        // Transfer Y to stack high byte
        stackBase=(y<<8)&0xff00;
        // NOTE: doesn't set flags
        return true;
#endif // e65x4510
#ifdef e65816
        // PLD
#endif // e65816
        return false;
        break;
      case 0x2c:  // BIT
        op=VAL_ABS(M16BIT);
        sbits->z=((a&op)==0);
        sbits->n=((op&(BYTE)MHIGHBIT)!=0);
        sbits->v=((op&(BYTE)MHALFBIT)!=0);
        break;
      case 0x2d:  // AND
        SET_A(a&VAL_ABS(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x2e:  // ROL
        op=VAL_ABS(M16BIT);
        val=(op<<1)&(MSIZEMASK-1);
        if(sbits->c) val++;
        WRITE_DATA(ADDR_ABS(),val);
        sbits->c=((op&MHIGHBIT)==MHIGHBIT);
        set_flags_nz(val,MSIZEMASK);
        break;
      case 0x2f:
#ifdef e65x4510
        // BBR2
        op=VAL_ZP(M16BIT);
        if(!(op&0x04)) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // AND
#endif // e65816
        return false;
        break;
      case 0x30:  // BMI
        if(getStatusReg()&E6502_STAT_N) pc+=(SBYTE)READ_CODE(pc+1);
        break;
      case 0x31:  // AND
        SET_A(a&VAL_INDX(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x32:
#ifdef e65x4510
        // AND
        SET_A(a&VAL_INDZ(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // AND
#endif // e65816
        return false;
        break;
      case 0x33:
#ifdef e65x4510
        // BMI (word rel)
        if(getStatusReg()&E6502_STAT_N) pc+=(SINT16)READW_CODE(pc+1);
        return true;
#endif // e65x4510
#ifdef e65816
        // AND
#endif // e65816
        return false;
        break;
      case 0x34:
#ifndef e6502
        // BIT
        op=VAL_ZPX(M16BIT);
        sbits->z=((a&op)==0);
        sbits->n=((op&(BYTE)MHIGHBIT)!=0);
        sbits->v=((op&(BYTE)MHALFBIT)!=0);
        return true;
#endif // e6502
        return false;
        break;
      case 0x35:  // AND
        SET_A(a&VAL_ZPX(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x36:  // ROL
        op=VAL_ZPX(M16BIT);
        val=(op<<1)&(MSIZEMASK-1);
        if(sbits->c) val++;
        WRITE_DATA(ADDR_ZPX(),val);
        sbits->c=((op&MHIGHBIT)==MHIGHBIT);
        set_flags_nz(val,MSIZEMASK);
        break;
      case 0x37:
#ifdef e65x4510
        // RMB3
        op=VAL_ZP(M16BIT);
        op&=0xf7;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // AND
#endif // e65816
        return false;
        break;
      case 0x38:  // SEC
        setStatusBits(E6502_STAT_C);
        break;
      case 0x39:  // AND
        SET_A(a&VAL_ABSY(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x3a:
#ifndef e6502
        // DEA or DEC
        SET_A(a-1);
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e6502
        return false;
        break;
      case 0x3b:
#ifdef e65816
        // TSC
	// NOTE: note quite right if in emulation mode...
	a=sp;
	set_flags_nz(a,MSIZEMASK);
	return true;
#endif // e65816
        return false;
	break;
      case 0x3c:
#ifndef e6502
        // BIT
        op=VAL_ABSX(M16BIT);
        sbits->z=((a&op)==0);
        sbits->n=((op&(BYTE)MHIGHBIT)!=0);
        sbits->v=((op&(BYTE)MHALFBIT)!=0);
        return true;
#endif // e6502
        return false;
        break;
      case 0x3d:  // AND
        SET_A(a&VAL_ABSX(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x3e:  // ROL
        op=VAL_ABSX(M16BIT);
        val=(op<<1)&(MSIZEMASK-1);
        if(sbits->c) val++;
        WRITE_DATA(ADDR_ABSX(),val);
        sbits->c=((op&MHIGHBIT)==MHIGHBIT);
        set_flags_nz(val,MSIZEMASK);
        break;
      case 0x3f:
#ifdef e65x4510
        // BBR3
        op=VAL_ZP(M16BIT);
        if(!(op&0x08)) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // AND
#endif // e65816
        return false;
        break;
      case 0x40:  // RTI
        setStatusReg(STACK_POP());
        jump(stack_popw()+1);
        pc--;
        //*w<<"pc will be "<<pc+op_len<<"\n";
#ifdef SHOW_INTS
        DEBUG_OUT<<"-";
#endif // e65x4510
        //trace=false;
        //debugger("RTI instruction\n");
        break;
      case 0x41:  // EOR
        SET_A(a^VAL_INDX(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x42:
#ifdef e65816
        // WDM
#endif // e65816
        return false;
        break;
      case 0x43:
#ifdef e65x4510
        // ASR A
        op=a>>1;
        op=(op&(MSIZEMASK-MHIGHBIT))|(a&MHIGHBIT);
        sbits->c=a&0x01;
        a=op;
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // EOR
#endif // e65816
        return false;
        break;
      case 0x44:
#ifdef e65816
        // MVP
#endif // e65816
        return false;
        break;
      case 0x45:  // EOR
        SET_A(a^VAL_ZP(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x46:  // LSR
        op=VAL_ZP(M16BIT);
        val=(op>>1)&(MSIZEMASK-MHIGHBIT);
        WRITE_DATA(ADDR_ZP(),val);
        sbits->c=((op&0x01)==0x01);  sbits->n=0;  sbits->z=(val==0);
        break;
      case 0x47:
#ifdef e65x4510
        // RMB4
        op=VAL_ZP(M16BIT);
        op&=0xef;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // EOR
#endif // e65816
        return false;
        break;
      case 0x48:  // PHA
#ifdef e65816
        if(M16BIT) STACK_PUSHW(a); else STACK_PUSH(a);
#else
        STACK_PUSH(a);
#endif // e65816
        break;
      case 0x49:  // EOR
        SET_A(a^VAL_IMM(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        ADJ_OP_LEN(M16BIT);
        break;
      case 0x4a:  // LSR
        op=a;
        val=(op>>1)&(MSIZEMASK-MHIGHBIT);
        SET_A(val);
        sbits->c=((op&0x01)==0x01);  sbits->n=0;  sbits->z=(val==0);
        break;
      case 0x4b:
#ifdef e65x4510
        // TAZ
        SET_Z(a);
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // PHK
#endif // e65816
        return false;
        break;
      case 0x4c:  // JMP
        ea=READW_CODE(pc+1);
        jump(ea);
        break;
      case 0x4d:  // EOR
        SET_A(a^VAL_ABS(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x4e:  // LSR
        op=VAL_ABS(M16BIT);
        val=(op>>1)&(MSIZEMASK-MHIGHBIT);
        WRITE_DATA(ADDR_ABS(),val);
        sbits->c=((op&0x01)==0x01);  sbits->n=0;  sbits->z=(val==0);
        break;
      case 0x4f:
#ifdef e65x4510
        // BBR4
        op=VAL_ZP(M16BIT);
        if(!(op&0x10)) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // EOR
#endif // e65816
        return false;
        break;
      case 0x50:  // BVC
        if(!(getStatusReg()&E6502_STAT_V)) pc+=(SBYTE)READ_CODE(pc+1);
        break;
      case 0x51:  // EOR
        SET_A(a^VAL_INDY(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x52:
#ifdef e65x4510
        // EOR
        SET_A(a^VAL_INDZ(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // EOR
#endif // e65816
        return false;
        break;
      case 0x53:
#ifdef e65x4510
        // BVC (word rel)
        if(!(getStatusReg()&E6502_STAT_V)) pc+=(SINT16)READW_CODE(pc+1);
        return true;
#endif // e65x4510
#ifdef e65816
        // EOR
#endif // e65816
        return false;
        break;
      case 0x54:
#ifdef e65x4510
        // ASR
        val=VAL_ZPX(M16BIT);
        op=op>>1;
        op=(op&(MSIZEMASK-MHIGHBIT))|(val&MHIGHBIT);
        sbits->c=val&0x01;
        WRITE_DATA(ADDR_ZPX(),op);
        set_flags_nz(op,MSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // MVN
#endif // e65816
        return false;
        break;
      case 0x55:  // EOR
        SET_A(a^VAL_ZPX(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x56:  // LSR
        op=VAL_ZPX(M16BIT);
        val=(op>>1)&(MSIZEMASK-MHIGHBIT);
        WRITE_DATA(ADDR_ZPX(),val);
        sbits->c=((op&0x01)==0x01);  sbits->n=0;  sbits->z=(val==0);
        break;
      case 0x57:
#ifdef e65x4510
        // RMB5
        op=VAL_ZP(M16BIT);
        op&=0xdf;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // EOR
#endif // e65816
        return false;
        break;
      case 0x58:  // CLI
        resetStatusBits(E6502_STAT_I);
        if(int_pending) cause_int(int_pending);
        break;
      case 0x59:  // EOR
        SET_A(a^VAL_ABSY(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x5a:
#ifndef e6502
        // PHY
#ifdef e65816
        if(X16BIT) STACK_PUSHW(y); else STACK_PUSH(y);
#else
        STACK_PUSH(y);
#endif
        return true;
#endif // e6502
        return false;
        break;
      case 0x5b:
#ifdef e65x4510
        // TAB
        // Transfer A to base page register ("direct page")
        dp=(a<<8)&0xff00;
        // NOTE: doesn't set flags
        return true;
#endif // e65x4510
#ifdef e65816
        // TCD
#endif // e65816
        return false;
        break;
      case 0x5c:
#ifdef e65x4510
        // MAP
        DEBUG_OUT<<"MAP: "<<(unsigned int)a<<" "<<(unsigned int)x<<" "<<(unsigned int)y<<" "<<(unsigned int)z<<"\n";
        mapUpper=((y<<8)&0xff00)|(((z&0x0f)<<16)&0xff0000);
        mapLower=((a<<8)&0xff00)|(((x&0x0f)<<16)&0xff0000);
        mapBlocks=(z&0xf0)|((x>>4)&0x0f);
        handle_map();
        //debugger("MAP!");
        return true;
#endif // e65x4510
#ifdef e65816
        // JMP
#endif // e65816
        return false;
        break;
      case 0x5d:  // EOR
        SET_A(a^VAL_ABSX(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x5e:  // LSR
        op=VAL_ABSX(M16BIT);
        val=(op>>1)&(MSIZEMASK-MHIGHBIT);
        WRITE_DATA(ADDR_ABSX(),val);
        sbits->c=((op&0x01)==0x01);  sbits->n=0;  sbits->z=(val==0);
        break;
      case 0x5f:
#ifdef e65x4510
        // BBR5
        op=VAL_ZP(M16BIT);
        if(!(op&0x20)) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // EOR
#endif // e65816
        return false;
        break;
      case 0x60:  // RTS
        opw=pc;
        jump(stack_popw()+1);
        stackTracePop(opw);
        break;
      case 0x61:  // ADC
        op=VAL_INDX(M16BIT);
        handle_adc(op);
        break;
      case 0x62:
#ifdef e65816
        // PER
#endif // e65816
        return false;
        break;
      case 0x63:
#ifdef e65x4510
        // BSR (word rel)
        ea=pc+(SINT16)READW_CODE(pc+1);
        STACK_PUSHW(ea);
        stackTracePush(ea);
        jump(ea);
        return true;
#endif // e65x4510
#ifdef e65816
        // ADC
#endif // e65816
        return false;
        break;
      case 0x64:
#ifndef e6502
        // STZ
        STORE_Z(ADDR_ZP());
        return true;
#endif // e65x4510
        return false;
        break;
      case 0x65:  // ADC
        op=VAL_ZP(M16BIT);
        handle_adc(op);
        break;
      case 0x66:  // ROR
        op=VAL_ZP(M16BIT);
        val=(op>>1)&(MSIZEMASK-MHIGHBIT);
        if(sbits->c) val|=MHIGHBIT;
        WRITE_DATA(ADDR_ZP(),val);
        sbits->c=((op&0x01)==0x01);
        set_flags_nz(val,MSIZEMASK);
        break;
      case 0x67:
#ifdef e65x4510
        // RMB6
        op=VAL_ZP(M16BIT);
        op&=0xbf;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // ADC
#endif // e65816
        return false;
        break;
      case 0x68:  // PLA
#ifdef e65816
        if(M16BIT) SET_A(STACK_POPW()); else SET_A(STACK_POP());
#else
        SET_A(STACK_POP());
#endif
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x69:  // ADC
        op=VAL_IMM(M16BIT);
        handle_adc(op);
        ADJ_OP_LEN(M16BIT);
        break;
      case 0x6a:  // ROR
        op=a;
        val=(op>>1)&(MSIZEMASK-MHIGHBIT);
        if(sbits->c) val|=MHIGHBIT;
        SET_A(val);
        sbits->c=((op&0x01)==0x01);
        set_flags_nz(val,MSIZEMASK);
        break;
      case 0x6b:
#ifdef e65x4510
        // TZA
        SET_A(z);
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // RTL
#endif // e65816
        return false;
        break;
      case 0x6c:  // JMP
        ea=READW_CODE(pc+1);
        jump(READW_DATA(ea));
        break;
      case 0x6d:  // ADC
        op=VAL_ABS(M16BIT);
        handle_adc(op);
        break;
      case 0x6e:  // ROR
        op=VAL_ABS(M16BIT);
        val=(op>>1)&(MSIZEMASK-MHIGHBIT);
        if(sbits->c) val|=MHIGHBIT;
        WRITE_DATA(ADDR_ABS(),val);
        sbits->c=((op&0x01)==0x01);
        set_flags_nz(val,MSIZEMASK);
        break;
      case 0x6f:
#ifdef e65x4510
        // BBR6
        op=VAL_ZP(M16BIT);
        if(!(op&0x40)) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // ADC
#endif // e65816
        return false;
        break;
      case 0x70:  // BVS
        if(getStatusReg()&E6502_STAT_V) pc+=(SBYTE)READ_CODE(pc+1);
        break;
      case 0x71:  // ADC
        op=VAL_INDY(M16BIT);
        handle_adc(op);
        break;
      case 0x72:
#ifdef e65x4510
        // ADC
        op=VAL_INDZ(M16BIT);
        handle_adc(op);
        return true;
#endif // e65x4510
#ifdef e65816
        // ADC
#endif // e65816
        return false;
        break;
      case 0x73:
#ifdef e65816
        // ADC
#endif // e65816
        return false;
        break;
      case 0x74:
#ifndef e6502
        // STZ
        STORE_Z(ADDR_ZPX());
        return true;
#endif // e6502
        return false;
        break;
      case 0x75:  // ADC
        op=VAL_ZPX(M16BIT);
        handle_adc(op);
        break;
      case 0x76:  // ROR
        op=VAL_ZPX(M16BIT);
        val=(op>>1)&(MSIZEMASK-MHIGHBIT);
        if(sbits->c) val|=MHIGHBIT;
        WRITE_DATA(ADDR_ZPX(),val);
        sbits->c=((op&0x01)==0x01);
        set_flags_nz(val,MSIZEMASK);
        break;
      case 0x77:
#ifdef e65x4510
        // RMB7
        op=VAL_ZP(M16BIT);
        op&=0x7f;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // ADC
#endif // e65816
        return false;
        break;
      case 0x78:  // SEI
        setStatusBits(E6502_STAT_I);
        break;
      case 0x79:  // ADC
        op=VAL_ABSY(M16BIT);
        handle_adc(op);
        break;
      case 0x7a:
#ifndef e6502
        // PLY
#ifdef e65816
        if(X16BIT) SET_Y(STACK_POPW()); else SET_Y(STACK_POP());
#else
        SET_Y(STACK_POP());
#endif
        set_flags_nz(y,XSIZEMASK);
        return true;
#endif // e6502
        return false;
        break;
      case 0x7b:
#ifdef e65x4510
        // TBA
        // Transfer base page register ("direct page") to A
        a=(dp>>8)&0x00ff;
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // TDC
        // NOTE: note quite right if in emulation mode...
        a=dp;
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e65816
        return false;
      case 0x7c:
#ifdef e65816
        // JMP
#endif // e65816
        return false;
        break;
      case 0x7d:  // ADC
        op=VAL_ABSX(M16BIT);
        handle_adc(op);
        break;
      case 0x7e:  // ROR
        op=VAL_ABSX(M16BIT);
        val=(op>>1)&(MSIZEMASK-MHIGHBIT);
        if(sbits->c) val|=MHIGHBIT;
        WRITE_DATA(ADDR_ABSX(),val);
        sbits->c=((op&0x01)==0x01);
        set_flags_nz(val,MSIZEMASK);
        break;
      case 0x7f:
#ifdef e65x4510
        // BBR7
        op=VAL_ZP(M16BIT);
        if(!(op&0x80)) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // ADC
#endif // e65816
        return false;
        break;
      case 0x80:
#ifndef e6502
        // BRA
        pc+=(SBYTE)READ_CODE(pc+1);
        return true;
#endif // e6502
        return false;
        break;
      case 0x81:  // STA
        STORE_A(ADDR_INDX());
        break;
      case 0x82:
#ifdef e65x4510
        // STA
        STORE_A(ADDR_INDYSP());
        return true;
#endif // e65x4510
#ifdef e65816
        // BRL
#endif // e65816
        return false;
        break;
      case 0x83:
#ifdef e65x4510
        // BRA (word rel)
        pc+=(SINT16)READW_CODE(pc+1);
        return true;
#endif // e65x4510
#ifdef e65816
        // STA
#endif // e65816
        return false;
        break;
      case 0x84:  // STY
        STORE_Y(ADDR_ZP());
        break;
      case 0x85:  // STA
        STORE_A(ADDR_ZP());
        break;
      case 0x86:  // STX
        STORE_X(ADDR_ZP());
        break;
      case 0x87:
#ifdef e65x4510
        // SMB0
        op=VAL_ZP(M16BIT);
        op|=0x01;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // STA
#endif // e65816
        return false;
        break;
      case 0x88:  // DEY
        SET_Y(y-1);
        set_flags_nz(y,XSIZEMASK);
        break;
      case 0x89:
#ifndef e6502
        // BIT
        op=VAL_IMM(M16BIT);
        sbits->z=((a&op)==0);
        sbits->n=((op&(BYTE)MHIGHBIT)!=0);
        sbits->v=((op&(BYTE)MHALFBIT)!=0);
        ADJ_OP_LEN(M16BIT);
        return true;
#endif // e6502
        return false;
        break;
      case 0x8a:  // TXA
        SET_A(x);
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x8b:
#ifdef e65x4510
        // STY (abs,x)
        STORE_Y(ADDR_ABSX());
        return true;
#endif // e65x4510
#ifdef e65816
        // PHB
        STACK_PUSH((dbr>>16)&0xff);
	return true;
#endif // e65816
        return false;
        break;
      case 0x8c:  // STY
        STORE_Y(ADDR_ABS());
        break;
      case 0x8d:  // STA
        STORE_A(ADDR_ABS());
        break;
      case 0x8e:  // STX
        STORE_X(ADDR_ABS());
        break;
      case 0x8f:
#ifdef e65x4510
        // BBS0
        op=VAL_ZP(M16BIT);
        if(op&0x01) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // STA abs long
        STORE_A_LONG(ADDR_ABSL());
	return true;
#endif // e65816
        return false;
        break;
      case 0x90:  // BCC
        if(!(getStatusReg()&E6502_STAT_C)) pc+=(SBYTE)READ_CODE(pc+1);
        break;
      case 0x91:  // STA
#ifdef e6509
        STORE_A_INDEXED(ADDR_INDY());
	//DEBUG_OUT<<"STA "<<a<<" to "<<ADDR_INDY()+indexed_offset<<"\n";
#else
        STORE_A(ADDR_INDY());
#endif
        break;
      case 0x92:
#ifdef e65x4510
        // STA
        STORE_A(ADDR_INDZ());
        return true;
#endif // e65x4510
#ifdef e65816
        // STA
#endif // e65816
        return false;
        break;
      case 0x93:
#ifdef e65x4510
        // BCC (word rel)
        if(!(getStatusReg()&E6502_STAT_C)) pc+=(SINT16)READW_CODE(pc+1);
        return true;
#endif // e65x4510
#ifdef e65816
        // STA
#endif // e65816
        return false;
        break;
      case 0x94:  // STY
        STORE_Y(ADDR_ZPX());
        break;
      case 0x95:  // STA
        STORE_A(ADDR_ZPX());
        break;
      case 0x96:  // STX
        STORE_X(ADDR_ZPY());
        break;
      case 0x97:
#ifdef e65x4510
        // SMB1
        op=VAL_ZP(M16BIT);
        op|=0x02;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // STA
#endif // e65816
        return false;
        break;
      case 0x98:  // TYA
        SET_A(y);
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0x99:  // STA
        STORE_A(ADDR_ABSY());
        break;
      case 0x9a:  // TXS
        sp=x;
        // NOTE: doesn't set flags...
        break;
      case 0x9b:
#ifdef e65816
        // TXY
        SET_Y(x);
        set_flags_nz(y,XSIZEMASK);
        return true;
#endif // e65816
        return false;
        break;
      case 0x9c:
#ifndef e6502
        // STZ
        STORE_Z(ADDR_ABS());
        return true;
#endif // e6502
        return false;
        break;
      case 0x9d:  // STA
        STORE_A(ADDR_ABSX());
        break;
      case 0x9e:
#ifndef e6502
        // STZ
        STORE_Z(ADDR_ABSX());
        return true;
#endif // e6502
        return false;
      case 0x9f:
#ifdef e65x4510
        // BBS1
        op=VAL_ZP(M16BIT);
        if(op&0x02) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // STA
        STORE_A_LONG(ADDR_ABSLX());
        return true;
#endif // e65816
        return false;
        break;
      case 0xa0:  // LDY
        SET_Y(VAL_IMM(X16BIT));
        set_flags_nz(y,XSIZEMASK);
        ADJ_OP_LEN(X16BIT);
        break;
      case 0xa1:  // LDA
        LOAD_A(ADDR_INDX());
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0xa2:  // LDX
        SET_X(VAL_IMM(X16BIT));
        set_flags_nz(x,XSIZEMASK);
        ADJ_OP_LEN(X16BIT);
        break;
      case 0xa3:
#ifdef e65x4510
        // LDZ
        z=VAL_IMM(X16BIT);
        set_flags_nz(z,XSIZEMASK);
        ADJ_OP_LEN(X16BIT);
        return true;
#endif // e65x4510
#ifdef e65816
        // LDA stack rel
        LOAD_A(ADDR_SR());
        set_flags_nz(a,MSIZEMASK);
	return true;
#endif // e65816
        return false;
        break;
      case 0xa4:  // LDY
        LOAD_Y(ADDR_ZP());
        set_flags_nz(y,XSIZEMASK);
        break;
      case 0xa5:  // LDA
        LOAD_A(ADDR_ZP());
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0xa6:  // LDX
        LOAD_X(ADDR_ZP());
        set_flags_nz(x,XSIZEMASK);
        break;
      case 0xa7:
#ifdef e65x4510
        // SMB2
        op=VAL_ZP(M16BIT);
        op|=0x04;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // LDA
#endif // e65816
        return false;
        break;
      case 0xa8:  // TAY
        SET_Y(a);
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0xa9:  // LDA
        SET_A(VAL_IMM(M16BIT));
        set_flags_nz(a,MSIZEMASK);
        ADJ_OP_LEN(M16BIT);
        break;
      case 0xaa:  // TAX
        SET_X(a);
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0xab:
#ifdef e65x4510
        // LDZ
        LOAD_Z(ADDR_ABS());
        set_flags_nz(z,XSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // PLB
        op=STACK_POP();
	dbr=op<<16;
        set_flags_nz(op,MSIZEMASK);
	return true;
	break;
#endif // e65816
        return false;
        break;
      case 0xac:  // LDY
        LOAD_Y(ADDR_ABS());
        set_flags_nz(y,XSIZEMASK);
        break;
      case 0xad:  // LDA
        LOAD_A(ADDR_ABS());
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0xae:  // LDX
        LOAD_X(ADDR_ABS());
        set_flags_nz(x,XSIZEMASK);
        break;
      case 0xaf:
#ifdef e65x4510
        // BBS2
        op=VAL_ZP(M16BIT);
        if(op&0x04) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // LDA
#endif // e65816
        return false;
        break;
      case 0xb0:  // BCS
        if(getStatusReg()&E6502_STAT_C) pc+=(SBYTE)READ_CODE(pc+1);
        break;
      case 0xb1:  // LDA
#ifdef e6509
        LOAD_A_INDEXED(ADDR_INDY());
	//DEBUG_OUT<<"LDA "<<a<<" from "<<ADDR_INDY()+indexed_offset<<"\n";
#else
        LOAD_A(ADDR_INDY());
#endif
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0xb2:
#ifdef e65x4510
        // LDA
        LOAD_A(ADDR_INDZ());
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // LDA
#endif // e65816
        return false;
        break;
      case 0xb3:
#ifdef e65x4510
        // BCS (word rel)
        if(getStatusReg()&E6502_STAT_C) pc+=(SINT16)READW_CODE(pc+1);
        return true;
#endif // e65x4510
#ifdef e65816
        // LDA
#endif // e65816
        return false;
        break;
      case 0xb4:  // LDY
        LOAD_Y(ADDR_ZPX());
        set_flags_nz(y,XSIZEMASK);
        break;
      case 0xb5:  // LDA
        LOAD_A(ADDR_ZPX());
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0xb6:  // LDX
        LOAD_X(ADDR_ZPX());
        set_flags_nz(x,MSIZEMASK);
        break;
      case 0xb7:
#ifdef e65x4510
        // SMB3
        op=VAL_ZP(M16BIT);
        op|=0x08;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // LDA
#endif // e65816
        return false;
        break;
      case 0xb8:  // CLV
        resetStatusBits(E6502_STAT_V);
        break;
      case 0xb9:  // LDA
        LOAD_A(ADDR_ABSY());
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0xba:  // TSX
        SET_X(sp);
        set_flags_nz(x,XSIZEMASK);
        break;
      case 0xbb:
#ifdef e65816
        // TYX
        SET_X(y);
        set_flags_nz(x,XSIZEMASK);
        return true;
#endif // e65816
        return false;
        break;
      case 0xbc:  // LDY
        LOAD_Y(ADDR_ABSX());
        set_flags_nz(y,XSIZEMASK);
        break;
      case 0xbd:  // LDA
        LOAD_A(ADDR_ABSX());
        set_flags_nz(a,MSIZEMASK);
        break;
      case 0xbe:  // LDX
        LOAD_X(ADDR_ABSY());
        set_flags_nz(x,XSIZEMASK);
        break;
      case 0xbf:
#ifdef e65x4510
        // BBS3
        op=VAL_ZP(M16BIT);
        if(op&0x08) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // LDA abs long ind x
        LOAD_A_LONG(ADDR_ABSLX());
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e65816
        return false;
        break;
      case 0xc0:  // CPY
        set_flags_nzc(y,VAL_IMM(X16BIT),XSIZEMASK);
        ADJ_OP_LEN(X16BIT);
        break;
      case 0xc1:  // CMP
        set_flags_nzc(a,VAL_INDX(M16BIT),MSIZEMASK);
        ADJ_OP_LEN(M16BIT);
        break;
      case 0xc2:
#ifdef e65x4510
        // CPZ
        set_flags_nzc(z,VAL_IMM(X16BIT),XSIZEMASK);
        ADJ_OP_LEN(X16BIT);
        return true;
#endif // e65x4510
#ifdef e65816
        // REP
        op=VAL_IMM8();
        if(!nativeMode) {
          // SEP can't affect these flags in emu mode
          if(op&E65816_STAT_Break) op-=E65816_STAT_Break;
          if(op&E65816_STAT_Extra) op-=E65816_STAT_Extra;
        }
        resetStatusBits(op);
        return true;
#endif // e65816
        return false;
        break;
      case 0xc3:
#ifdef e65x4510
        // DEW (zp)
        ea=ADDR_ZP();
        opw=(UINT16)(READW_DATA(ea)-1);
        WRITEW_DATA(ea,opw);
        set_flags_w_nz(opw);
        return true;
#endif // e65x4510
#ifdef e65816
        // CMP
#endif // e65816
        return false;
        break;
      case 0xc4:  // CPY
        set_flags_nzc(y,VAL_ZP(X16BIT),XSIZEMASK);
        break;
      case 0xc5:  // CMP
        set_flags_nzc(a,VAL_ZP(M16BIT),MSIZEMASK);
        break;
      case 0xc6:  // DEC
        ea=ADDR_ZP();
        op=(BYTE)(READ_DATA(ea)-1);
        WRITE_DATA(ea,op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0xc7:
#ifdef e65x4510
        // SMB4
        op=VAL_ZP(M16BIT);
        op|=0x10;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // CMP
#endif // e65816
        return false;
        break;
      case 0xc8:  // INY
        SET_Y(y+1);
        set_flags_nz(y,XSIZEMASK);
        break;
      case 0xc9:  // CMP
        set_flags_nzc(a,VAL_IMM(M16BIT),MSIZEMASK);
        ADJ_OP_LEN(M16BIT);
        break;
      case 0xca:  // DEX
        SET_X(x-1);
        set_flags_nz(x,XSIZEMASK);
        break;
      case 0xcb:
#ifdef e65816
        // WAI
#endif // e65816
        return false;
        break;
      case 0xcc:  // CPY
        set_flags_nzc(y,VAL_ABS(X16BIT),XSIZEMASK);
        break;
      case 0xcd:  // CMP
        set_flags_nzc(a,VAL_ABS(M16BIT),MSIZEMASK);
        break;
      case 0xce:  // DEC
        ea=ADDR_ABS();
        op=(BYTE)(READ_DATA(ea)-1);
        WRITE_DATA(ea,op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0xcf:
#ifdef e65x4510
        // BBS4
        op=VAL_ZP(M16BIT);
        if(op&0x10) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // CMP
#endif // e65816
        return false;
        break;
      case 0xd0:  // BNE
        if(!(getStatusReg()&E6502_STAT_Z)) pc+=(SBYTE)READ_CODE(pc+1);
        break;
      case 0xd1:  // CMP
        set_flags_nzc(a,VAL_INDY(M16BIT),MSIZEMASK);
        break;
      case 0xd2:
#ifdef e65x4510
        // CMP
        set_flags_nzc(a,VAL_INDZ(M16BIT),MSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // CMP
#endif // e65816
        return false;
        break;
      case 0xd3:
#ifdef e65816
        // CMP
#endif // e65816
        return false;
        break;
      case 0xd4:
#ifdef e65816
        // PEI
#endif // e65816
        return false;
        break;
      case 0xd5:  // CMP
        set_flags_nzc(a,VAL_ZPX(M16BIT),MSIZEMASK);
        break;
      case 0xd6:  // DEC
        ea=ADDR_ZPX();
        op=(BYTE)(READ_DATA(ea)-1);
        WRITE_DATA(ea,op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0xd7:
#ifdef e65x4510
        // SMB5
        op=VAL_ZP(M16BIT);
        op|=0x20;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // CMP
#endif // e65816
        return false;
        break;
      case 0xd8:  // CLD
        resetStatusBits(E6502_STAT_D);
        break;
      case 0xd9:  // CMP
        set_flags_nzc(a,VAL_ABSY(M16BIT),MSIZEMASK);
        break;
      case 0xda:
#ifndef e6502
        // PHX
#ifdef e65816
        if(X16BIT) STACK_PUSHW(x); else STACK_PUSH(x);
#else
        STACK_PUSH(x);
#endif
        return true;
#endif // e6502
        return false;
        break;
      case 0xdb:
#ifdef e65x4510
        // PHZ
        STACK_PUSH(z);
        return true;
#endif // e65x4510
#ifdef e65816
        // STP
#endif // e65816
        return false;
        break;
      case 0xdc:
#ifdef e65816
        // JMP
#endif // e65816
        return false;
        break;
      case 0xdd:  // CMP
        set_flags_nzc(a,VAL_ABSX(M16BIT),MSIZEMASK);
        break;
      case 0xde:  // DEC
        ea=ADDR_ABSX();
        op=(BYTE)(READ_DATA(ea)-1);
        WRITE_DATA(ea,op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0xdf:
#ifdef e65x4510
        // BBS5
        op=VAL_ZP(M16BIT);
        if(op&0x20) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // CMP abs long indexed x
        set_flags_nzc(a,VAL_ABSLX(M16BIT),MSIZEMASK);
        return true;
#endif // e65816
        return false;
        break;
      case 0xe0:  // CPX
        set_flags_nzc(x,VAL_IMM(X16BIT),XSIZEMASK);
        ADJ_OP_LEN(X16BIT);
        break;
      case 0xe1:  // SBC
        op=VAL_INDX(M16BIT);
        handle_sbc(op);
        break;
      case 0xe2:
#ifdef e65x4510
        // LDA (indirect stack and y)
        LOAD_A(ADDR_INDYSP());
        set_flags_nz(a,MSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // SEP
        op=VAL_IMM8();
        if(!nativeMode) {
          // SEP can't affect these flags in emu mode
          if(op&E65816_STAT_Break) op-=E65816_STAT_Break;
          if(op&E65816_STAT_Extra) op-=E65816_STAT_Extra;
        }
        setStatusBits(op);
        return true;
#endif // e65816
        return false;
        break;
      case 0xe3:
#ifdef e65x4510
        // INW (zp)
        ea=ADDR_ZP();
        opw=(UINT16)(READW_DATA(ea)+1);
        WRITEW_DATA(ea,opw);
        set_flags_w_nz(opw);
        return true;
#endif // e65x4510
#ifdef e65816
        // SBC
#endif // e65816
        return false;
        break;
      case 0xe4:  // CPX
        set_flags_nzc(x,VAL_ZP(X16BIT),XSIZEMASK);
        break;
      case 0xe5:  // SBC
        op=VAL_ZP(M16BIT);
        handle_sbc(op);
        break;
      case 0xe6:  // INC
        ea=ADDR_ZP();
        op=(BYTE)(READ_DATA(ea)+1);
        WRITE_DATA(ea,op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0xe7:
#ifdef e65x4510
        // SMB6
        op=VAL_ZP(M16BIT);
        op|=0x40;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // SBC
#endif // e65816
        return false;
        break;
      case 0xe8:  // INX
        SET_X(x+1);
        set_flags_nz(x,XSIZEMASK);
        break;
      case 0xe9:  // SBC
        op=VAL_IMM(M16BIT);
        handle_sbc(op);
        ADJ_OP_LEN(M16BIT);
        break;
      case 0xea:  // NOP
#ifdef e65x4510
        DEBUG_OUT<<"***intLock turned off***\n";
        intLock=false;
#endif // e65x4510
        break;
      case 0xeb:
#ifdef e65x4510
        // ROW
        // rotate word (always to the left, there is no right)
        opw=VAL_ABSW(M16BIT);
        valw=(opw<<1)&0xfffe;
        if(sbits->c) valw++;
        WRITEW_DATA(ADDR_ABSW(),valw);
        sbits->c=((opw&0x8000)==0x8000);
        set_flags_w_nz(valw);
        return true;
#endif // e65x4510
#ifdef e65816
        // XBA
        if(M16BIT) { a=((a&0xff00)>>8)|((a&0xff)<<8); }
        else { op=a; a=bHolder; bHolder=op; }
        return true;
#endif // e65816
        return false;
        break;
      case 0xec:  // CPX
        set_flags_nzc(x,VAL_ABS(X16BIT),XSIZEMASK);
        break;
      case 0xed:  // SBC
        op=VAL_ABS(M16BIT);
        handle_sbc(op);
        break;
      case 0xee:  // INC
        ea=ADDR_ABS();
        op=(BYTE)(READ_DATA(ea)+1);
        WRITE_DATA(ea,op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0xef:
#ifdef e65x4510
        // BBS6
        op=VAL_ZP(M16BIT);
        if(op&0x40) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // SBC
#endif // e65816
        return false;
        break;
      case 0xf0:  // BEQ
        if(getStatusReg()&E6502_STAT_Z) pc+=(SBYTE)READ_CODE(pc+1);
        break;
      case 0xf1:  // SBC
        op=VAL_INDY(M16BIT);
        handle_sbc(op);
        break;
      case 0xf2:
#ifdef e65x4510
        // SBC
        op=VAL_INDZ(M16BIT);
        handle_sbc(op);
        return true;
#endif // e65x4510
#ifdef e65816
        // SBC
#endif // e65816
        return false;
        break;
      case 0xf3:
#ifdef e65x4510
        // BEQ (word offset)
        if(getStatusReg()&E6502_STAT_Z) pc+=(SINT16)READW_CODE(pc+1);
        return true;
#endif // e65x4510
#ifdef e65816
        // SBC
#endif // e65816
        return false;
        break;
      case 0xf4:
#ifdef e65x4510
        // PHW
        STACK_PUSHW(VAL_IMM16());
        return true;
#endif // e65x4510
#ifdef e65816
        // PEA
        STACK_PUSHW(VAL_IMM16());
        return true;
#endif // e65816
        return false;
        break;
      case 0xf5:  // SBC
        op=VAL_ZPX(M16BIT);
        handle_sbc(op);
        break;
      case 0xf6:  // INC
        ea=ADDR_ZPX();
        op=(BYTE)(READ_DATA(ea)+1);
        WRITE_DATA(ea,op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0xf7:
#ifdef e65x4510
        // SMB7
        op=VAL_ZP(M16BIT);
        op|=0x80;
        WRITE_DATA(ADDR_ZP(),op);
        // NOTE: no flags set
        return true;
#endif // e65x4510
#ifdef e65816
        // SBC
#endif // e65816
        return false;
        break;
      case 0xf8:  // SED
        setStatusBits(E6502_STAT_D);
        break;
      case 0xf9:  // SBC
        op=VAL_ABSY(M16BIT);
        handle_sbc(op);
        break;
      case 0xfa:
#ifndef e6502
        // PLX
#ifdef e65816
        if(X16BIT) SET_X(STACK_POPW()); else SET_X(STACK_POP());
#else
        SET_X(STACK_POP());
#endif
        set_flags_nz(x,XSIZEMASK);
        return true;
#endif // e6502
        return false;
        break;
      case 0xfb:
#ifdef e65x4510
        // PLZ
        SET_Z(STACK_POP());
        set_flags_nz(z,XSIZEMASK);
        return true;
#endif // e65x4510
#ifdef e65816
        // XCE
        if(getStatusReg()&E65816_STAT_C) {
          // C was set
          if(nativeMode) {
            // Going from native to emulation mode
            goEmulation();
            resetStatusBits(E65816_STAT_C);
          }
          else {
            // No change to mode or C
            // already in emulation mode
          }
        }
        else {
          // C was clear
          if(nativeMode) {
            // No change to mode or C
            // already in native mode
          }
          else {
            // Going from native to emulation mode
            goNative();
            setStatusBits(E65816_STAT_C);
          }
        }
        return true;
#endif // e65816
        return false;
        break;
      case 0xfc:
#ifdef e65x4510
        // PHW
        STACK_PUSHW(VAL_IMM16());
        return true;
#endif // e65x4510
#ifdef e65816
        // JSR
#endif // e65816
        return false;
        break;
      case 0xfd:  // SBC
        op=VAL_ABSX(M16BIT);
        handle_sbc(op);
        break;
      case 0xfe:  // INC
        ea=ADDR_ABSX();
        op=(BYTE)(READ_DATA(ea)+1);
        WRITE_DATA(ea,op);
        set_flags_nz(op,MSIZEMASK);
        break;
      case 0xff:
#ifdef e65x4510
        // BBS7
        op=VAL_ZP(M16BIT);
        if(op&0x80) pc+=(SBYTE)READ_CODE(pc+2);
        return true;
#endif // e65x4510
#ifdef e65816
        // SBC
#endif // e65816
        return false;
      default:
        debugger("Unimplemented opcode!");
        break;
    }
  }
  // NOTE: Because most opcodes never reach here, never put anything important here!
  return true;
}


// TODO: Doesn't work at all with op sizes!
void theClassType::handle_adc(unsigned int bb)
{
  opw=0;
  if(sbits->d) opw=handle_adc_d(a,bb);
  else {
    BYTE cc=0;
    if(sbits->c) cc=1;
    opw=a+bb+cc;
    int as=a&MHIGHBIT,bs=bb&MHIGHBIT,opws=opw&MHIGHBIT;
    if((as==bs)&&(as!=opws)) sbits->v=1; else sbits->v=0;
    sbits->c=opw>0xff;
    SET_A(opw);
    set_flags_nz(a,MSIZEMASK);
  }
}


// TODO: Doesn't work at all with op sizes!
void theClassType::handle_sbc(unsigned int bb)
{
  opw=0;
  if(sbits->d) opw=handle_sbc_d(a,bb);
  else {
    BYTE cc=0;
    int sbb=bb;
    if(!sbits->c) cc=1;
    opw=a-bb-cc;
    int as=a&MHIGHBIT,bs=(-sbb)&MHIGHBIT,opws=opw&MHIGHBIT;
    if((as==bs)&&(as!=opws)) sbits->v=1; else sbits->v=0;
    sbits->c=opw<0x100;
    SET_A(opw);
    set_flags_nz(a,MSIZEMASK);
  }
}


// TODO: Doesn't work at all with op sizes!
// TODO: This isn't quite right...
UINT16 theClassType::handle_adc_d(unsigned int aa, unsigned int bb)
{
  //debugger("Decimal mode ADC unimplemented!\n");
  //BYTE cc=0;
  //if(sbits->c) cc=1;
  BYTE la=aa&0xf,lb=bb&0xf;
  BYTE lr=la+lb;
  BYTE ha=aa&0xf0,hb=bb&0xf0;
  ha>>=4;  hb>>=4;
  BYTE hr=ha+hb;
  if(lr>9) hr++;
  opw=((hr&0xf)<<4)+(lr&0xf);
  sbits->c=hr>9;
  //opw=a+b+cc;
  //int as=a&MHIGHBIT,bs=b&MHIGHBIT,opws=opw&MHIGHBIT;
  //if((as==bs)&&(as!=opws)) sbits->v=1; else sbits->v=0;
  //sbits->c=opw>0xff;
  SET_A(opw);
  set_flags_nz(a,MSIZEMASK);
  return opw;
}


// TODO: Doesn't work at all with op sizes!
// TODO: This isn't quite right...
UINT16 theClassType::handle_sbc_d(unsigned int aa, unsigned int bb)
{
  //debugger("Decimal mode SBC unimplemented!\n");
  //BYTE cc=0;
  //if(!sbits->c) cc=1;
  BYTE la=aa&0xf,lb=bb&0xf;
  BYTE lr=la-lb;
  BYTE ha=aa&0xf0,hb=bb&0xf0;
  ha>>=4;  hb>>=4;
  BYTE hr=ha-hb;
  if(lr>9) hr--;
  opw=((hr&0xf)<<4)|(lr&0xf);
  sbits->c=hr>9;
  //opw=a-b-cc;
  //int as=a&MHIGHBIT,bs=(-b)&MHIGHBIT,opws=opw&MHIGHBIT;
  //if((as==bs)&&(as!=opws)) sbits->v=1; else sbits->v=0;
  //sbits->c=opw<0x100;
  SET_A(opw);
  set_flags_nz(a,MSIZEMASK);
  return opw;
}


