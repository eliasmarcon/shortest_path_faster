EXECS=mpi_spf
MPICC?=mpicc.mpich

# Define the output directory
OUTDIR=out

# Include directory for header files
INCLUDEDIR=include

# Source files
SRC=$(wildcard src/*.c)

# Create the output directory if it doesn't exist
$(OUTDIR):
	mkdir -p $(OUTDIR)

all: ${EXECS}

mpi_spf: $(OUTDIR) $(SRC)
	${MPICC} -I$(INCLUDEDIR) -o $(OUTDIR)/$@ $(SRC)

mpi_graph: 
	${MPICC} -o $(OUTDIR)/mpi_graph src/Graph.c


clean:
	rm -f $(OUTDIR)/*
