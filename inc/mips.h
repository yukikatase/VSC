#define INST_MEM_START 0x04000000
#define DATA_MEM_START 0x10000000

typedef enum {
  false,
  true
} Signal;

typedef struct {
  Signal s;
} Path;

typedef struct {
  Path *in1;
  Path *in2;
  Path *out1;
} ANDGate;

typedef struct {
  Path *in1;
  Path *in2;
  Path *out1;
} ORGate;

typedef struct {
  Path *in1;
  Path *out1;
} NOTGate;

typedef struct {
  ANDGate *agate1;
  NOTGate *ngate1;
} NANDCircuit;

typedef struct {
  NOTGate *ngate1;
  NOTGate *ngate2;
  ANDGate *agate1;
  ANDGate *agate2;
  ORGate *ogate1;
} XORCircuit;

typedef struct {
  XORCircuit *xorc1;
  XORCircuit *xorc2;
  ANDGate *agate1;
  ANDGate *agate2;
  ORGate *ogate1;
} FA;

typedef struct {
  int num;
  Path *p;
} Bus;

typedef struct {
  FA *f;
} RCA;

typedef struct {
  ANDGate *agate1;
  ORGate *ogate1;
  FA *f1;
} PFA;

typedef struct {
  ANDGate *agates;
  ORGate *ogates;
} CLU;

typedef struct {
  int num;
  Path **ins;
  Path *out1;
} ANDGateN;

typedef struct {
  int num;
  Path **ins;
  Path *out1;
} ORGateN;

typedef struct {
  PFA *pfa;
  CLU *clu;
  XORCircuit *xorc;
  ANDGateN *agaten;
  ORGateN *ogaten;
} CLA4;

typedef struct {
  ANDGate *agate1;
  ANDGate *agate2;
  ORGate *ogate1;
  NOTGate *ngate1;
} MUX;

typedef struct {
  ANDGateN *agatens;
  ORGateN *ogatens;
  NOTGate *ngates;
} MUX4;

typedef struct {
  ANDGate *agate1;
  ORGate *ogate1;
  NOTGate *ngate1;
  FA *f1;
  MUX4 *mux;
  MUX *binvert;
} ALU;

typedef struct {
ANDGate *agate1;
ORGate *ogate1;
NOTGate *ngate1;
FA *f1;
MUX4 *mux;
MUX *binvert;
} ALU_MSB;

typedef struct {
  ALU *alus;
  ALU_MSB *alumsb1;
} ALU32;

typedef struct {
int val;
Path *wctl;
Bus *wdata;
Bus *rdata;
} Register;

typedef struct {
Bus *a;
Bus *b;
} Decoder5;

typedef struct {
Bus *ctls;
Bus *ins;
Bus *out1;
} MUX32_Bus;

typedef struct {
ANDGate *agates;
Register *regs;
Decoder5 *dec;
MUX32_Bus *mux1;
MUX32_Bus *mux2;
} RegisterFile;

typedef struct {
int *mem;
Bus *addr;
Bus *inst;
} InstMemory;

typedef struct {
int *mem;
Path *memWrite;
Path *memRead;
Bus *addr;
Bus *wdata;
Bus *rdata;
} DataMemory;

void path_set_signal(Path *p, Signal s);
Signal path_get_signal(Path *p);
void path_init(Path *p);

void andgate_init(ANDGate *agate, Path *in1, Path *in2, Path *out1);
void andgate_run(ANDGate *agate);
void andgate_driver(Signal a, Signal b);

void orgate_init(ORGate *agate, Path *in1, Path *in2, Path *out1);
void orgate_run(ORGate *agate) ;
void orgate_driver(Signal a, Signal b) ;

void notgate_init(NOTGate *ngate, Path *in1, Path *out1);
void notgate_run(NOTGate *ngate);
void notgate_driver(Signal a);

void nandcircuit_init(NANDCircuit *nandc, Path *in1, Path *in2, Path *out1);
void nandcircuit_run(NANDCircuit *nandc);
void nandcircuit_release(NANDCircuit *nandc);
Path* nandcircuit_get_out_path(NANDCircuit *nandc);
void nandcircuit_driver(Signal a, Signal b);

void xorcircuit_init(XORCircuit *nandc, Path *in1, Path *in2, Path *out1);
void xorcircuit_run(XORCircuit *nandc);
void xorcircuit_release(XORCircuit *nandc);
Path* xorcircuit_get_out_path(XORCircuit *nandc);
void xorcircuit_driver(Signal a, Signal b);

void fa_init(FA *f, Path *in1, Path *in2, Path *carry_in, Path *out1, Path *carry_out);
void fa_run(FA *f);
void fa_release(FA *f);
void fa_driver(Signal a, Signal b, Signal carry_in);
Path* fa_get_out_path(FA *f);
Path* fa_get_carry_out_path();

void bus_init(Bus *b, int n);
void bus_set_value(Bus *b, int val);
int bus_get_value(Bus *b);
Path* bus_get_path(Bus *b, int i);
void bus_release(Bus *b);

void rca_init(RCA *r, Bus *a, Bus *b, Path *carry_in, Bus *s, Path *carry_out);
void rca_run(RCA *r);
void rca_release(RCA *r);
void rca_driver(int na, int nb);

void pfa_init(PFA *pfa, Path *a, Path *b, Path *carry_in, Path *g, Path *p, Path *s);
void pfa_run(PFA *pfa);
void pfa_release(PFA *pfa);
void pfa_driver(int an, int bn);

void clu_init(CLU *clu, Bus *g, Bus *p, Bus *c, Path *carry_out);
void clu_run(CLU *clu);
void clu_release(CLU *clu);
void clu_driver(int an, int bn, int cn);

void andgaten_init(ANDGateN *agaten, Path **ins, Path *out1, int num);
void andgaten_run(ANDGateN *agaten);

void orgaten_init(ORGateN *ogaten, Path **ins, Path *out1, int num);
void orgaten_run(ORGateN *ogaten);

void cla4_init(CLA4 *cla4, Bus *a, Bus *b, Path *carry_in, Bus *s, Path *carry_out, Path *G, Path *P);
void cla4_run(CLA4 *cla4);
void cla4_release(CLA4 *cla4);
void cla4_driver(int na, int nb);

void mux_init(MUX *m, Path *ctl, Path *in1, Path *in2, Path *out1);
void mux_run(MUX *m);
void mux_release(MUX *m);
void mux_driver(int an, int bn, int cn);

void mux4_init(MUX4 *m4, Path *ctl1, Path *ctl2, Path *in1, Path *in2, Path *in3, Path *in4, Path *out1);
void mux4_run(MUX4 *m4);
void mux4_release(MUX4 *m4);
void mux4_driver(int an, int bn, int cn, int dn, int en, int fn);

void alu_init(ALU *alu, Path* op, Path *a, Path *b, Path *less, Path *carryIn, Path *s, Path *carryOut);
void alu_run(ALU *alu);
void alu_release(ALU *alu);
void alu_driver(int an, int bn, int cn, int dn, int en);

void alu_msb_init(ALU_MSB *alu_msb, Path* op, Path *a, Path *b, Path *less, Path *carryIn, Path *s, Path *carryOut, Path *set);
void alu_msb_run(ALU_MSB *alu_msb);
void alu_msb_release(ALU_MSB *alu_msb);

void alu32_init(ALU32 *alu32, Path *op, Bus *a, Bus *b, Bus *s, Path *carryOut);
void alu32_run(ALU32 *alu32);
void alu32_driver(int an, int bn, int cn,int dn, int en);
void alu32_release(ALU32 *alu32);

void register_init(Register *reg, Path *wctl, Bus *wdata, Bus *rdata);
void register_run(Register *reg);
void register_set_value(Register *reg, int v);
int register_get_value(Register *reg);

void decoder5_init(Decoder5 *d, Bus *a, Bus *b);
void decoder5_run(Decoder5 *d);

void mux32_bus_init(MUX32_Bus *mb, Bus *ctls, Bus *ins, Bus *out1);
void mux32_bus_run(MUX32_Bus *mb);

void registerfile_init(RegisterFile *regfile, Path *regWrite, Bus *read1, Bus *read2, Bus *write1, Bus *wdata, Bus *rdata1, Bus *rdata2);
void registerfile_run(RegisterFile *regfile);
void registerfile_release(RegisterFile *regfile);
void registerfile_set_value(RegisterFile *regfile, int reg_num, int val);
int registerfile_get_value(RegisterFile *regfile, int reg_num);
void registerfile_test()