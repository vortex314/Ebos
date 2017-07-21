##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Common
ConfigurationName      :=Debug
WorkspacePath          :=/home/lieven/workspace
ProjectPath            :=/home/lieven/workspace/Common
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Lieven
Date                   :=29/01/17
CodeLitePath           :=/home/lieven/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Common.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)inc $(IncludeSwitch)/home/lieven/workspace/Arduino/tools/xtensa-lx106-elf/xtensa-lx106-elf/include $(IncludeSwitch)/home/lieven/workspace/Esp8266-Arduino-Makefile/esp8266/cores/esp8266 $(IncludeSwitch)/home/lieven/workspace/Esp8266-Arduino-Makefile/esp8266-2.3.0/tools/sdk/include $(IncludeSwitch)/home/lieven/workspace/Esp8266-Arduino-Makefile/esp8266-2.3.0/variants/generic 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -Os -std=c++11 -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -MMD -ffunction-sections -fdata-sections $(Preprocessors)
CFLAGS   :=  -Os -std=c++11 -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -MMD -ffunction-sections -fdata-sections $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_Bytes.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_BufferedByteStream.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Node.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Msg.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_PIC32_stubs.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Log.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_STM32_stubs.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_CborQueue.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Str.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Msgpack.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_jsmn.c$(ObjectSuffix) $(IntermediateDirectory)/src_EventSource.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Json.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Strpack.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_CircBuf.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_BipBuffer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_printf.c$(ObjectSuffix) $(IntermediateDirectory)/src_Base64.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Cbor.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_EventSource.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_Node.cpp$(ObjectSuffix) $(IntermediateDirectory)/inc_LinkedList.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "/home/lieven/workspace/.build-debug"
	@echo rebuilt > "/home/lieven/workspace/.build-debug/Common"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_Bytes.cpp$(ObjectSuffix): src/Bytes.cpp $(IntermediateDirectory)/src_Bytes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/Bytes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Bytes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Bytes.cpp$(DependSuffix): src/Bytes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Bytes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Bytes.cpp$(DependSuffix) -MM src/Bytes.cpp

$(IntermediateDirectory)/src_Bytes.cpp$(PreprocessSuffix): src/Bytes.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Bytes.cpp$(PreprocessSuffix)src/Bytes.cpp

$(IntermediateDirectory)/src_BufferedByteStream.cpp$(ObjectSuffix): src/BufferedByteStream.cpp $(IntermediateDirectory)/src_BufferedByteStream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/BufferedByteStream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_BufferedByteStream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_BufferedByteStream.cpp$(DependSuffix): src/BufferedByteStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_BufferedByteStream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_BufferedByteStream.cpp$(DependSuffix) -MM src/BufferedByteStream.cpp

$(IntermediateDirectory)/src_BufferedByteStream.cpp$(PreprocessSuffix): src/BufferedByteStream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_BufferedByteStream.cpp$(PreprocessSuffix)src/BufferedByteStream.cpp

$(IntermediateDirectory)/src_Node.cpp$(ObjectSuffix): src/Node.cpp $(IntermediateDirectory)/src_Node.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/Node.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Node.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Node.cpp$(DependSuffix): src/Node.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Node.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Node.cpp$(DependSuffix) -MM src/Node.cpp

$(IntermediateDirectory)/src_Node.cpp$(PreprocessSuffix): src/Node.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Node.cpp$(PreprocessSuffix)src/Node.cpp

$(IntermediateDirectory)/src_Msg.cpp$(ObjectSuffix): src/Msg.cpp $(IntermediateDirectory)/src_Msg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/Msg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Msg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Msg.cpp$(DependSuffix): src/Msg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Msg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Msg.cpp$(DependSuffix) -MM src/Msg.cpp

$(IntermediateDirectory)/src_Msg.cpp$(PreprocessSuffix): src/Msg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Msg.cpp$(PreprocessSuffix)src/Msg.cpp

$(IntermediateDirectory)/src_PIC32_stubs.cpp$(ObjectSuffix): src/PIC32_stubs.cpp $(IntermediateDirectory)/src_PIC32_stubs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/PIC32_stubs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_PIC32_stubs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_PIC32_stubs.cpp$(DependSuffix): src/PIC32_stubs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_PIC32_stubs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_PIC32_stubs.cpp$(DependSuffix) -MM src/PIC32_stubs.cpp

$(IntermediateDirectory)/src_PIC32_stubs.cpp$(PreprocessSuffix): src/PIC32_stubs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_PIC32_stubs.cpp$(PreprocessSuffix)src/PIC32_stubs.cpp

$(IntermediateDirectory)/src_Log.cpp$(ObjectSuffix): src/Log.cpp $(IntermediateDirectory)/src_Log.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/Log.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Log.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Log.cpp$(DependSuffix): src/Log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Log.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Log.cpp$(DependSuffix) -MM src/Log.cpp

$(IntermediateDirectory)/src_Log.cpp$(PreprocessSuffix): src/Log.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Log.cpp$(PreprocessSuffix)src/Log.cpp

$(IntermediateDirectory)/src_STM32_stubs.cpp$(ObjectSuffix): src/STM32_stubs.cpp $(IntermediateDirectory)/src_STM32_stubs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/STM32_stubs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_STM32_stubs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_STM32_stubs.cpp$(DependSuffix): src/STM32_stubs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_STM32_stubs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_STM32_stubs.cpp$(DependSuffix) -MM src/STM32_stubs.cpp

$(IntermediateDirectory)/src_STM32_stubs.cpp$(PreprocessSuffix): src/STM32_stubs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_STM32_stubs.cpp$(PreprocessSuffix)src/STM32_stubs.cpp

$(IntermediateDirectory)/src_CborQueue.cpp$(ObjectSuffix): src/CborQueue.cpp $(IntermediateDirectory)/src_CborQueue.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/CborQueue.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_CborQueue.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_CborQueue.cpp$(DependSuffix): src/CborQueue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_CborQueue.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_CborQueue.cpp$(DependSuffix) -MM src/CborQueue.cpp

$(IntermediateDirectory)/src_CborQueue.cpp$(PreprocessSuffix): src/CborQueue.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_CborQueue.cpp$(PreprocessSuffix)src/CborQueue.cpp

$(IntermediateDirectory)/src_Str.cpp$(ObjectSuffix): src/Str.cpp $(IntermediateDirectory)/src_Str.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/Str.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Str.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Str.cpp$(DependSuffix): src/Str.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Str.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Str.cpp$(DependSuffix) -MM src/Str.cpp

$(IntermediateDirectory)/src_Str.cpp$(PreprocessSuffix): src/Str.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Str.cpp$(PreprocessSuffix)src/Str.cpp

$(IntermediateDirectory)/src_Msgpack.cpp$(ObjectSuffix): src/Msgpack.cpp $(IntermediateDirectory)/src_Msgpack.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/Msgpack.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Msgpack.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Msgpack.cpp$(DependSuffix): src/Msgpack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Msgpack.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Msgpack.cpp$(DependSuffix) -MM src/Msgpack.cpp

$(IntermediateDirectory)/src_Msgpack.cpp$(PreprocessSuffix): src/Msgpack.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Msgpack.cpp$(PreprocessSuffix)src/Msgpack.cpp

$(IntermediateDirectory)/src_jsmn.c$(ObjectSuffix): src/jsmn.c $(IntermediateDirectory)/src_jsmn.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/lieven/workspace/Common/src/jsmn.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_jsmn.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_jsmn.c$(DependSuffix): src/jsmn.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_jsmn.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_jsmn.c$(DependSuffix) -MM src/jsmn.c

$(IntermediateDirectory)/src_jsmn.c$(PreprocessSuffix): src/jsmn.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_jsmn.c$(PreprocessSuffix)src/jsmn.c

$(IntermediateDirectory)/src_EventSource.cpp$(ObjectSuffix): src/EventSource.cpp $(IntermediateDirectory)/src_EventSource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/EventSource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_EventSource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_EventSource.cpp$(DependSuffix): src/EventSource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_EventSource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_EventSource.cpp$(DependSuffix) -MM src/EventSource.cpp

$(IntermediateDirectory)/src_EventSource.cpp$(PreprocessSuffix): src/EventSource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_EventSource.cpp$(PreprocessSuffix)src/EventSource.cpp

$(IntermediateDirectory)/src_Json.cpp$(ObjectSuffix): src/Json.cpp $(IntermediateDirectory)/src_Json.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/Json.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Json.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Json.cpp$(DependSuffix): src/Json.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Json.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Json.cpp$(DependSuffix) -MM src/Json.cpp

$(IntermediateDirectory)/src_Json.cpp$(PreprocessSuffix): src/Json.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Json.cpp$(PreprocessSuffix)src/Json.cpp

$(IntermediateDirectory)/src_Strpack.cpp$(ObjectSuffix): src/Strpack.cpp $(IntermediateDirectory)/src_Strpack.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/Strpack.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Strpack.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Strpack.cpp$(DependSuffix): src/Strpack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Strpack.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Strpack.cpp$(DependSuffix) -MM src/Strpack.cpp

$(IntermediateDirectory)/src_Strpack.cpp$(PreprocessSuffix): src/Strpack.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Strpack.cpp$(PreprocessSuffix)src/Strpack.cpp

$(IntermediateDirectory)/src_CircBuf.cpp$(ObjectSuffix): src/CircBuf.cpp $(IntermediateDirectory)/src_CircBuf.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/CircBuf.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_CircBuf.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_CircBuf.cpp$(DependSuffix): src/CircBuf.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_CircBuf.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_CircBuf.cpp$(DependSuffix) -MM src/CircBuf.cpp

$(IntermediateDirectory)/src_CircBuf.cpp$(PreprocessSuffix): src/CircBuf.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_CircBuf.cpp$(PreprocessSuffix)src/CircBuf.cpp

$(IntermediateDirectory)/src_BipBuffer.cpp$(ObjectSuffix): src/BipBuffer.cpp $(IntermediateDirectory)/src_BipBuffer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/BipBuffer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_BipBuffer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_BipBuffer.cpp$(DependSuffix): src/BipBuffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_BipBuffer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_BipBuffer.cpp$(DependSuffix) -MM src/BipBuffer.cpp

$(IntermediateDirectory)/src_BipBuffer.cpp$(PreprocessSuffix): src/BipBuffer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_BipBuffer.cpp$(PreprocessSuffix)src/BipBuffer.cpp

$(IntermediateDirectory)/src_printf.c$(ObjectSuffix): src/printf.c $(IntermediateDirectory)/src_printf.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/lieven/workspace/Common/src/printf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_printf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_printf.c$(DependSuffix): src/printf.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_printf.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_printf.c$(DependSuffix) -MM src/printf.c

$(IntermediateDirectory)/src_printf.c$(PreprocessSuffix): src/printf.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_printf.c$(PreprocessSuffix)src/printf.c

$(IntermediateDirectory)/src_Base64.cpp$(ObjectSuffix): src/Base64.cpp $(IntermediateDirectory)/src_Base64.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/Base64.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Base64.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Base64.cpp$(DependSuffix): src/Base64.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Base64.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Base64.cpp$(DependSuffix) -MM src/Base64.cpp

$(IntermediateDirectory)/src_Base64.cpp$(PreprocessSuffix): src/Base64.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Base64.cpp$(PreprocessSuffix)src/Base64.cpp

$(IntermediateDirectory)/src_Cbor.cpp$(ObjectSuffix): src/Cbor.cpp $(IntermediateDirectory)/src_Cbor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/Cbor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Cbor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Cbor.cpp$(DependSuffix): src/Cbor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Cbor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Cbor.cpp$(DependSuffix) -MM src/Cbor.cpp

$(IntermediateDirectory)/src_Cbor.cpp$(PreprocessSuffix): src/Cbor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Cbor.cpp$(PreprocessSuffix)src/Cbor.cpp

$(IntermediateDirectory)/src_EventSource.cpp$(ObjectSuffix): src/EventSource.cpp $(IntermediateDirectory)/src_EventSource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/EventSource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_EventSource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_EventSource.cpp$(DependSuffix): src/EventSource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_EventSource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_EventSource.cpp$(DependSuffix) -MM src/EventSource.cpp

$(IntermediateDirectory)/src_EventSource.cpp$(PreprocessSuffix): src/EventSource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_EventSource.cpp$(PreprocessSuffix)src/EventSource.cpp

$(IntermediateDirectory)/src_Node.cpp$(ObjectSuffix): src/Node.cpp $(IntermediateDirectory)/src_Node.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/src/Node.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Node.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Node.cpp$(DependSuffix): src/Node.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Node.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Node.cpp$(DependSuffix) -MM src/Node.cpp

$(IntermediateDirectory)/src_Node.cpp$(PreprocessSuffix): src/Node.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Node.cpp$(PreprocessSuffix)src/Node.cpp

$(IntermediateDirectory)/inc_LinkedList.cpp$(ObjectSuffix): inc/LinkedList.cpp $(IntermediateDirectory)/inc_LinkedList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Common/inc/LinkedList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/inc_LinkedList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/inc_LinkedList.cpp$(DependSuffix): inc/LinkedList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/inc_LinkedList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/inc_LinkedList.cpp$(DependSuffix) -MM inc/LinkedList.cpp

$(IntermediateDirectory)/inc_LinkedList.cpp$(PreprocessSuffix): inc/LinkedList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/inc_LinkedList.cpp$(PreprocessSuffix)inc/LinkedList.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


