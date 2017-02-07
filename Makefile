GCC = gcc

CFLAGS = -Wall -O3 -g
INCLUDE = -I./inc

BIN = ./bin
SRC = ./src
OBJ = ./obj

OBJS_GATE = $(OBJ)/path.o $(OBJ)/and_gate.o $(OBJ)/not_gate.o $(OBJ)/or_gate.o $(OBJ)/and_gate_n.o $(OBJ)/or_gate_n.o
OBJS_CIRCUIT = $(OBJ)/nand_circuit.o $(OBJ)/xor_circuit.o
OBJS_FADRIVER = $(OBJ)/fa_driver.o
OBJS_BUS = $(OBJ)/bus.o
OBJS_RCA =  $(OBJ)/rca.o
OBJS_CLA = $(OBJ)/pfa.o $(OBJ)/clu.o $(OBJ)/cla4.o
OBJS_ALU = $(OBJ)/mux.o $(OBJ)/mux4.o $(OBJ)/alu.o $(OBJ)/alu_msb.o $(OBJ)/alu32.o
OBJS_DRIVER = $(OBJS_GATE) $(OBJS_CIRCUIT) $(OBJS_FADRIVER) $(OBJS_BUS) $(OBJS_RCA) $(OBJS_CLA) $(OBJS_ALU) $(OBJ)/driver.o

all :
	make driver

driver : $(OBJS_DRIVER)
	$(GCC) -o $(BIN)/$@ $(OBJS_DRIVER) $(CFLAGS) $(INCLUDE)

$(OBJ)/%.o : $(SRC)/%.c
	mkdir -p $(dir $@)
	$(GCC) -c $(CFLAGS) $(INCLUDE) -o $@ $<

clean :
	rm -rf $(BIN)/*
	rm -rf $(OBJ)/*