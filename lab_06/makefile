.PHONY : build unit stat clean make_outdir

APP := app.exe
UNIT_APP := unit_tests.exe
#STAT_APP := stat.exe

SRC_DIR := ./src
INC_DIR := ./inc
OUT_DIR := ./out
APPS_DIR := ./apps1

UNIT_DIR := ./unit_tests
FTEST_DIR := ./func_tests

CC := gcc
CFLAGS := -std=c99 -I$(INC_DIR) -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -Wfloat-conversion -Wvla
LD_FLAGS := `pkg-config --cflags --libs check`

ifeq ($(mode), debug)
    CFLAGS += -g3 -O0
endif

ifeq ($(mode), release)
    CFLAGS += -O3 -g0
endif

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o)
UNIT_SRCS := $(wildcard $(UNIT_DIR)/*.c)
UNIT_OBJS := $(UNIT_SRCS:$(UTEST_DIR)/%.c=$(OUT_DIR)/%.o)

build: $(APP)

unit: $(UNIT_APP)

$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LD_FLAGS)

$(UNIT_APP) : $(filter-out $(OUT_DIR)/main.o, $(OBJS)) $(UNIT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LD_FLAGS)

make_outdir:
	mkdir -p $(OUT_DIR)

${OUT_DIR}/%.o : ${SRC_DIR}/%.c make_outdir
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/check_%.o : $(UNIT_DIR)/check_%.c make_outdir
	$(CC) $(CFLAGS) -o $@ -c $<

$(INC_DIR)/%.h:

clean:
	rm -f $(APP) $(UNIT_EXEC) $(STAT_EXEC) ${OUT_DIR}/*
	rm -rf $(OUT_DIR)

