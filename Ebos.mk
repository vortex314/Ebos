##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Ebos
ConfigurationName      :=Debug
WorkspacePath          :=/home/lieven/workspace
ProjectPath            :=/home/lieven/workspace/Ebos
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Lieven
Date                   :=28/01/17
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
ObjectsFileList        :="Ebos.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../Common/inc $(IncludeSwitch)/home/lieven/workspace/Arduino/tools/xtensa-lx106-elf/xtensa-lx106-elf/include $(IncludeSwitch)/home/lieven/workspace/Esp8266-Arduino-Makefile/esp8266/cores/esp8266 $(IncludeSwitch)/home/lieven/workspace/Esp8266-Arduino-Makefile/esp8266-2.3.0/tools/sdk/include $(IncludeSwitch)/home/lieven/workspace/Esp8266-Arduino-Makefile/esp8266-2.3.0/variants/generic 
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
Objects0=$(IntermediateDirectory)/Slip.cpp$(ObjectSuffix) $(IntermediateDirectory)/SlipStream.cpp$(ObjectSuffix) $(IntermediateDirectory)/Uid.cpp$(ObjectSuffix) $(IntermediateDirectory)/Actor.cpp$(ObjectSuffix) $(IntermediateDirectory)/EventBus.cpp$(ObjectSuffix) $(IntermediateDirectory)/System.cpp$(ObjectSuffix) $(IntermediateDirectory)/MqttCbor.cpp$(ObjectSuffix) $(IntermediateDirectory)/MqttJson.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/lieven/workspace/.build-debug/Ebos"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Slip.cpp$(ObjectSuffix): Slip.cpp $(IntermediateDirectory)/Slip.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Ebos/Slip.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Slip.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Slip.cpp$(DependSuffix): Slip.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Slip.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Slip.cpp$(DependSuffix) -MM Slip.cpp

$(IntermediateDirectory)/Slip.cpp$(PreprocessSuffix): Slip.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Slip.cpp$(PreprocessSuffix)Slip.cpp

$(IntermediateDirectory)/SlipStream.cpp$(ObjectSuffix): SlipStream.cpp $(IntermediateDirectory)/SlipStream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Ebos/SlipStream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SlipStream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SlipStream.cpp$(DependSuffix): SlipStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SlipStream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SlipStream.cpp$(DependSuffix) -MM SlipStream.cpp

$(IntermediateDirectory)/SlipStream.cpp$(PreprocessSuffix): SlipStream.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SlipStream.cpp$(PreprocessSuffix)SlipStream.cpp

$(IntermediateDirectory)/Uid.cpp$(ObjectSuffix): Uid.cpp $(IntermediateDirectory)/Uid.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Ebos/Uid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Uid.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Uid.cpp$(DependSuffix): Uid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Uid.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Uid.cpp$(DependSuffix) -MM Uid.cpp

$(IntermediateDirectory)/Uid.cpp$(PreprocessSuffix): Uid.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Uid.cpp$(PreprocessSuffix)Uid.cpp

$(IntermediateDirectory)/Actor.cpp$(ObjectSuffix): Actor.cpp $(IntermediateDirectory)/Actor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Ebos/Actor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Actor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Actor.cpp$(DependSuffix): Actor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Actor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Actor.cpp$(DependSuffix) -MM Actor.cpp

$(IntermediateDirectory)/Actor.cpp$(PreprocessSuffix): Actor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Actor.cpp$(PreprocessSuffix)Actor.cpp

$(IntermediateDirectory)/EventBus.cpp$(ObjectSuffix): EventBus.cpp $(IntermediateDirectory)/EventBus.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Ebos/EventBus.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/EventBus.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/EventBus.cpp$(DependSuffix): EventBus.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/EventBus.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/EventBus.cpp$(DependSuffix) -MM EventBus.cpp

$(IntermediateDirectory)/EventBus.cpp$(PreprocessSuffix): EventBus.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/EventBus.cpp$(PreprocessSuffix)EventBus.cpp

$(IntermediateDirectory)/System.cpp$(ObjectSuffix): System.cpp $(IntermediateDirectory)/System.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Ebos/System.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/System.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/System.cpp$(DependSuffix): System.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/System.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/System.cpp$(DependSuffix) -MM System.cpp

$(IntermediateDirectory)/System.cpp$(PreprocessSuffix): System.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/System.cpp$(PreprocessSuffix)System.cpp

$(IntermediateDirectory)/MqttCbor.cpp$(ObjectSuffix): MqttCbor.cpp $(IntermediateDirectory)/MqttCbor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Ebos/MqttCbor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MqttCbor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MqttCbor.cpp$(DependSuffix): MqttCbor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MqttCbor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MqttCbor.cpp$(DependSuffix) -MM MqttCbor.cpp

$(IntermediateDirectory)/MqttCbor.cpp$(PreprocessSuffix): MqttCbor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MqttCbor.cpp$(PreprocessSuffix)MqttCbor.cpp

$(IntermediateDirectory)/MqttJson.cpp$(ObjectSuffix): MqttJson.cpp $(IntermediateDirectory)/MqttJson.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lieven/workspace/Ebos/MqttJson.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MqttJson.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MqttJson.cpp$(DependSuffix): MqttJson.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MqttJson.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MqttJson.cpp$(DependSuffix) -MM MqttJson.cpp

$(IntermediateDirectory)/MqttJson.cpp$(PreprocessSuffix): MqttJson.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MqttJson.cpp$(PreprocessSuffix)MqttJson.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


