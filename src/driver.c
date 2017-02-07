#include <stdio.h>
#include <mips.h>

int main(int argc, char **argv) {

  printf("ANDDriver\n");
  andgate_driver(true, true);
  andgate_driver(true, false);
  andgate_driver(false, true);
  andgate_driver(false, false);

  printf("ORDriver\n");
  orgate_driver(true, true);
  orgate_driver(true, false);
  orgate_driver(false, true);
  orgate_driver(false, false);

  printf("NOTDriver\n");
  notgate_driver(true);
  notgate_driver(false);

  printf("NANDDriver\n");
  nandcircuit_driver(true, true);
  nandcircuit_driver(true, false);
  nandcircuit_driver(false, true);
  nandcircuit_driver(false, false);

  printf("XORDriver\n");
  xorcircuit_driver(true, true);
  xorcircuit_driver(true, false);
  xorcircuit_driver(false, true);
  xorcircuit_driver(false, false);

  printf("FADriver\n");
  fa_driver(false, false, false);
  fa_driver(false, false, true);
  fa_driver(false, true, false);
  fa_driver(true, false, false);
  fa_driver(false, true, true);
  fa_driver(true, true, false);
  fa_driver(true, false, true);
  fa_driver(true, true, true);

  printf("RCA\n");
  rca_driver(2, 3024000);
  rca_driver(4, 15);
  rca_driver(25, 30);

  printf("PFA\n");
  pfa_driver(0, 0);
  pfa_driver(0, 1);
  pfa_driver(1, 0);
  pfa_driver(1, 1);

  printf("MUX\n");
  mux_driver(0, 0, 0);
  mux_driver(1, 0, 1);
  mux_driver(0, 1, 1);
  mux_driver(1, 1, 0);

  printf("MUX4\n");
  mux4_driver(1, 0, 0, 1, 0, 0);

  printf("ALU\n");
  alu_driver(0, 1, 1, 1 ,1); //01が加算(a, b, op[0], op[1], Bin)

  printf("ALU32\n");
  alu32_driver(3, 1, 1, 1, 1); //(a, b, op[0], op[1], Bin)
  return 0;
}