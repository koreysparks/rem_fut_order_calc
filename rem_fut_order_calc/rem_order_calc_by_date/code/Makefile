CXX=g++
AR=ar
OUT_NAME=rem_trade_statistical

OUT_PATH=../lib/
SRC_PATH=./
OBJ_PATH=./obj/

SL_EFH_API_PATH=../../common_libs_pub/
SL_EFH_API_INC_PATH=$(SL_EFH_API_PATH)include/
SL_EFH_API_LIB_PATH=$(SL_EFH_API_PATH)lib/linux/centos6.4/debug/


DEBUG_PATH=debug/
RELEASE_PATH=release/

PUBLISH_PATH=../bin/linux/

INC_DEBUG_PATH=	\

INC_RELEASE_PATH= \

LIB_DEBUG_FILE= -lpthread \
		-lrt \
		-ldl  \
		-L./ -lmysqlclient \
		$(SL_EFH_API_LIB_PATH)shengli_xml.a \
		$(SL_EFH_API_LIB_PATH)shengli_tool.a \

LIB_RELEASE_FILE= -lpthread \

SRC_FILE=$(wildcard $(SRC_PATH) *.cpp)
ALL_FILE=$(notdir $(SRC_FILE))
OBJ_FILE=$(patsubst %.cpp,%.o,$(ALL_FILE))

DEBUG_CXXFLAGS=-g -ggdb3  -Wall -fPIC  
RELEASE_CXXFLAGS=-O2 -fPIC

DEBUG_ARFLAGS=cqs
RELEASE_ARFLAGS=cqs

OUT_DEBUG_FILE=$(OUT_PATH)$(OUT_NAME)
OBJ_DEBUG_FILE=$(addprefix  $(OBJ_PATH)$(DEBUG_PATH),$(notdir $(OBJ_FILE)))

OUT_RELEASE_FILE=$(OUT_PATH)$(RELEASE_PATH)$(OUT_NAME)
OBJ_RELEASE_FILE=$(addprefix  $(OBJ_PATH)$(RELEASE_PATH),$(notdir $(OBJ_FILE)))

debug : pre_debug $(OUT_DEBUG_FILE)

release : per_release $(OUT_RELEASE_FILE)

all : debug release

publish:  pre_publish

pre_debug :
	-$(shell mkdir $(OBJ_PATH) -p)
	-$(shell mkdir $(OBJ_PATH)$(DEBUG_PATH) -p)
	-$(shell mkdir $(OUT_PATH) -p)
	-$(shell mkdir $(OUT_PATH)$(DEBUG_PATH) -p)
	-$(shell cp  $(SL_EFH_API_LIB_PATH)* $(OUT_PATH)$(DEBUG_PATH) -p)

per_release :
	-$(shell mkdir $(OBJ_PATH) -p)
	-$(shell mkdir $(OBJ_PATH)$(RELEASE_PATH) -p)
	-$(shell mkdir $(OUT_PATH) -p)
	-$(shell mkdir $(OUT_PATH)$(RELEASE_PATH) -p)

$(OUT_DEBUG_FILE) : $(OBJ_DEBUG_FILE)
	$(CXX) $(DEBUG_CXXFLAGS) $(addprefix  $(OBJ_PATH)$(DEBUG_PATH),$(notdir $^)) -o $@ $(LIB_DEBUG_FILE)
#	$(AR) $(DEBUG_ARFLAGS) $(OUT_DEBUG_FILE) $(addprefix  $(OBJ_PATH)$(DEBUG_PATH),$(notdir $^)) $@ $(LIB_DEBUG_FILE)

$(OUT_RELEASE_FILE) : $(OBJ_RELEASE_FILE)
	$(CXX) $(RELEASE_CXXFLAGS) $(addprefix  $(OBJ_PATH)$(RELEASE_PATH),$(notdir $^)) -o $@ $(LIB_RELEASE_FILE)
#	$(AR) $(RELEASE_ARFLAGS) $(OUT_RELEASE_FILE) $(addprefix  $(OBJ_PATH)$(RELEASE_PATH),$(notdir $^)) $@ $(LIB_RELEASE_FILE)

$(OBJ_PATH)$(DEBUG_PATH)%.o : %.cpp
	$(CXX) -c $(DEBUG_CXXFLAGS) $(INC_DEBUG_PATH) $(SRC_PATH)$< -o $@
	
$(OBJ_PATH)$(RELEASE_PATH)%.o : %.cpp
	$(CXX) -c $(RELEASE_CXXFLAGS) $(INC_RELEASE_PATH) $(SRC_PATH)$< -o $@

pre_publish:
	$(shell cp $(OUT_PATH)$(DEBUG_PATH)$(OUT_NAME) $(PUBLISH_PATH) -p)
	@echo "publish finish"
	
clean :
	@echo  "make clean"
	$(shell rm $(OBJ_PATH) -rf)
	$(shell rm $(OUT_PATH)$(DEBUG_PATH)$(OUT_NAME) -f)
	$(shell rm $(OUT_PATH)$(RELEASE_PATH)$(OUT_NAME) -f)

