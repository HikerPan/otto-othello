.PHONY: clean run

SRC = src/
EXECUTABLE = othello.exe

# 添加调试标志
CXXFLAGS += -g 

all:
	cd $(SRC); make CXXFLAGS="$(CXXFLAGS)"; mv $(EXECUTABLE) ..;

run:
	./$(EXECUTABLE);

clean:
	rm -f $(EXECUTABLE); cd $(SRC); make clean;
