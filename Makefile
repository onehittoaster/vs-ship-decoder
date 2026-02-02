OUT_DIR:=	.

CXX:=		clang++
#CXXFLAGS +=	-Wall -Werror -Wextra -Wfatal-errors -pedantic-errors -o0
#CXXFLAGS +=	-Wall -Wextra -pedantic-errors -fpermissive
CXXFLAGS +=	-Wall -fpermissive -Wno-logical-op-parentheses -Wno-unused-variable
CXXFLAGS +=	--std=c++23
CXXFLAGS +=	-I. -Ivendor/LightNBT -Ivendor/zstr/src -Ivendor/json/include
CXXFLAGS +=	-lz
CXXFLAGS +=	-o0 -g3

VS_SHIP_DECODER:=	$(OUT_DIR)/vs-ship-decoder

TARGETS:=	$(VS_SHIP_DECODER)

all:		$(TARGETS)

clean:
		rm $(VS_SHIP_DECODER)

format:
		clang-format -i *.cc

$(VS_SHIP_DECODER):	./vs-ship-decoder.cc
		$(CXX) $(CXXFLAGS) $^ -lz -o $@
