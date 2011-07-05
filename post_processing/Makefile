ifdef PAR
	CFLAGS=-DPAR
	CC=mpicc
else
	CC=gcc
endif
TARGET_DIR=bin
COMMON=common
$(shell [ -d "$(TARGET_DIR)" ] || mkdir -p $(TARGET_DIR))

all : $(TARGET_DIR)/curv_mesh_3d.ex $(TARGET_DIR)/curv_mesh_oc_3d.ex $(TARGET_DIR)/avizo_amira_oc.ex

$(TARGET_DIR)/curv_mesh_3d.ex : $(COMMON)/visit_writer.o source/curv_mesh_3d.c
	$(CC) -I$(COMMON) $(COMMON)/visit_writer.o source/curv_mesh_3d.c -o $(TARGET_DIR)/curv_mesh_3d.ex

$(TARGET_DIR)/curv_mesh_oc_3d.ex : $(COMMON)/visit_writer.o source/curv_mesh_oc_3d.c
	$(CC) -I$(COMMON) $(COMMON)/visit_writer.o source/curv_mesh_oc_3d.c -o $(TARGET_DIR)/curv_mesh_oc_3d.ex

$(TARGET_DIR)/avizo_amira_oc.ex: source/avizo_amira_oc.c
	$(CC) source/avizo_amira_oc.c -o $(TARGET_DIR)/avizo_amira_oc.ex

$(COMMON)/visit_writer.o :
	$(CC) -c -o common/visit_writer.o common/visit_writer.c -I $(COMMON)

.PHONY : clean
clean:
	rm -r bin
	rm common/*.o
