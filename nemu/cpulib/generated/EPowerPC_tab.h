// Don't edit this file, it is machine generated


#ifndef ELIB_EPowerPC_TAB_H
#define ELIB_EPowerPC_TAB_H


int EPowerPC_size[]={
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
};

int EPowerPC_time[]={
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
};

const char *EPowerPC_name[]={
"illegal","illegal","tdi","twi","illegal","illegal","illegal","mulli",
"subfic","dozi","cmpli","cmpi","addic","addic_","addi","addis",
"bc","sc","b","various","rlwimi","rlwinm","rlmi","rlwnm",
"ori","oris","xori","xoris","andi_","andis_","various","various",
"lwz","lwzu","lbz","lbzu","stw","stwu","stb","stbu",
"lhz","lhzu","lha","lhau","sth","sthu","lmw","stmw",
"lfs","lfsu","lfd","lfdu","stfs","stfsu","sfd","stfdu",
"illegal","illegal","various","various","illegal","illegal","various","various",
};

int EPowerPC_format[]={
EPPC_FORMAT_UNKNOWN,EPPC_FORMAT_UNKNOWN,EPPC_FORMAT_D,EPPC_FORMAT_D,
EPPC_FORMAT_UNKNOWN,EPPC_FORMAT_UNKNOWN,EPPC_FORMAT_UNKNOWN,EPPC_FORMAT_D,
EPPC_FORMAT_D,EPPC_FORMAT_UNKNOWN,EPPC_FORMAT_D,EPPC_FORMAT_D,
EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,
EPPC_FORMAT_B,EPPC_FORMAT_SC,EPPC_FORMAT_I,EPPC_FORMAT_VARIOUS,
EPPC_FORMAT_M,EPPC_FORMAT_M,EPPC_FORMAT_UNKNOWN,EPPC_FORMAT_M,
EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,
EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_VARIOUS,EPPC_FORMAT_VARIOUS,
EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,
EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,
EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,
EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,
EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,
EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,EPPC_FORMAT_D,
EPPC_FORMAT_UNKNOWN,EPPC_FORMAT_UNKNOWN,EPPC_FORMAT_VARIOUS,EPPC_FORMAT_VARIOUS,
EPPC_FORMAT_UNKNOWN,EPPC_FORMAT_UNKNOWN,EPPC_FORMAT_VARIOUS,EPPC_FORMAT_VARIOUS,
};

int EPowerPC_subformat[]={
false,false,false,false,
false,false,false,false,
false,false,EPPC_SUBFORMAT_CMPI,EPPC_SUBFORMAT_CMPI,
false,false,false,false,
false,false,false,false,
EPPC_SUBFORMAT_IMMED,EPPC_SUBFORMAT_IMMED,false,EPPC_SUBFORMAT_REG,
false,false,false,false,
false,false,false,false,
EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,
EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,
EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,
EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,
EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,
EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,EPPC_SUBFORMAT_LDST,
false,false,false,false,
false,false,false,false,
};

int EPowerPC_flags[]={
false,false,EPPC_64ONLY,false,
false,false,false,false,
false,EPPC_POWER,false,false,
false,false,false,false,
false,false,false,false,
false,false,EPPC_POWER,false,
false,false,false,false,
false,false,EPPC_64ONLY,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
EPPC_POWER2,EPPC_POWER2,EPPC_64ONLY,false,
EPPC_POWER2,EPPC_POWER2,EPPC_64ONLY,false,
};

#endif // ELIB_EPowerPC_TAB_H
