# Компилятор и флаги
CXX = g++
XASM = nasm
CXXFLAGS = -ggdb3 -fno-omit-frame-pointer -rdynamic -finstrument-functions -mavx2 -msse4.2 -O3 -Wall -Wextra -std=c++17 -MMD -MP -Wformat=2 -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal \
-Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual \
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual \
-Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE
XASMFLAGS = -f elf64 -ggdb3 -F dwarf

DEBUG = NO_DEBUG
# Директории исходников и объектных файлов
SRC_DIRS = src list list/src Get_file_size Check_flags
SRC_CPP = $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
SRC_ASM = $(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))

OBJ_DIR = obj
OBJ_DIR_ASM = $(OBJ_DIR)/asm
OUTDIR = build
ASM_DIR = asm

# Правильное формирование путей к объектным файлам
OBJ_CPP = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_CPP))
OBJ_ASM = $(patsubst %.asm,$(OBJ_DIR_ASM)/%.o,$(SRC_ASM))

# Выходной файл
OUT = $(OUTDIR)/main.exe

# Цель по умолчанию
all: $(OUT)

# Включение зависимостей
-include $(OBJ_CPP:.o=.d)

# Очистка
clean:
	rm -rf $(OBJ_DIR) $(OUTDIR)

# Универсальное правило для .cpp файлов
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CXX) -D $(DEBUG) $(CXXFLAGS) -Iheaders -Ilist/headers -IGet_file_size -ICheck_flags -c $< -o $@
	@echo "Compiled $<"

# Компиляция .asm файлов
$(OBJ_DIR_ASM)/%.o: %.asm
	@mkdir -p $(@D)
	@$(XASM) $(XASMFLAGS) $< -o $@
	@echo "Assembled $<"

# Линковка
$(OUT): $(OBJ_CPP) $(OBJ_ASM)
	@mkdir -p $(@D)
	@$(CXX) -rdynamic -ggdb3 -o $@ $^
	@echo "Linking completed: $@"

compile_asm:
	$(CXX) -D $(DEBUG) -S -masm=intel -c $(SRC_CPP) $(CXXFLAGS) -Iheaders -Ilist/headers -IGet_file_size -ICheck_flags
	mv *.s $(ASM_DIR)
run:
	./build/main.exe -rf0 text/Oblomov_processed.txt -wf0 buckets.txt -wf1 testing_data.txt
	./build/main.exe -rf0 text/Oblomov_processed.txt -wf0 buckets.txt -wf1 testing_data.txt
	./build/main.exe -rf0 text/Oblomov_processed.txt -wf0 buckets.txt -wf1 testing_data.txt
	./build/main.exe -rf0 text/Oblomov_processed.txt -wf0 buckets.txt -wf1 testing_data.txt
	./build/main.exe -rf0 text/Oblomov_processed.txt -wf0 buckets.txt -wf1 testing_data.txt
	./build/main.exe -rf0 text/Oblomov_processed.txt -wf0 buckets.txt -wf1 testing_data.txt
	./build/main.exe -rf0 text/Oblomov_processed.txt -wf0 buckets.txt -wf1 testing_data.txt
	./build/main.exe -rf0 text/Oblomov_processed.txt -wf0 buckets.txt -wf1 testing_data.txt
