################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Accel_ADXL345.cpp \
../Accel.cpp \
../BeatBox.cpp \
../Play.cpp \
../Debug.cpp \
../Lights.cpp \
../Log.cpp \
../Mean.cpp \
../Midi.cpp \
../Record.cpp \
../Taps.cpp \
../Trigger.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Accel_ADXL345.o \
Accel.o \
BeatBox.o \
Play.o \
Debug.o \
Lights.o \
Log.o \
Mean.o \
Midi.o \
Record.o \
Taps.o \
Trigger.o

OBJS_AS_ARGS +=  \
Accel_ADXL345.o \
Accel.o \
BeatBox.o \
Play.o \
Debug.o \
Lights.o \
Log.o \
Mean.o \
Midi.o \
Record.o \
Taps.o \
Trigger.o

C_DEPS +=  \
Accel_ADXL345.d \
Accel.d \
BeatBox.d \
Play.d \
Debug.d \
Lights.d \
Log.d \
Mean.d \
Midi.d \
Record.d \
Taps.d \
Trigger.d

C_DEPS_AS_ARGS +=  \
Accel_ADXL345.d \
Accel.d \
BeatBox.d \
Play.d \
Debug.d \
Lights.d \
Log.d \
Mean.d \
Midi.d \
Record.d \
Taps.d \
Trigger.d

OUTPUT_FILE_PATH +=BeatBox.elf

OUTPUT_FILE_PATH_AS_ARGS +=BeatBox.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
C:/Users/Charlie/Documents/ETSII_UPM/TFG/BeatBox_v2/ArduinoCore/Debug/libArduinoCore.a

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler

























./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=16000000 -DARDUINO=100  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.106\include" -I"../../ArduinoCore/include/core" -I"../../ArduinoCore/include/variants/core" -I"../../ArduinoCore/include/variants/mega" -I"../../ArduinoCore/include/Wire" -I"../../ArduinoCore/include/Wire/utility" -I"../../ArduinoCore/include/extLibs"  -Os -fno-exceptions -fdata-sections -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atmega328 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.106\gcc\dev\atmega328" -c -fno-exceptions -ffunction-sections -fdata-sections -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR8/GNU Linker : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-static -Wl,-Map="BeatBox.map" -Wl,--start-group -Wl,-lm -Wl,-lArduinoCore  -Wl,--end-group -Wl,-L"C:\Users\Charlie\Documents\ETSII_UPM\TFG\BeatBox_v2\ArduinoCore\Debug"  -Wl,--gc-sections -mrelax -mmcu=atmega328 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.106\gcc\dev\atmega328" -Os  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "BeatBox.elf" "BeatBox.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "BeatBox.elf" "BeatBox.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "BeatBox.elf" > "BeatBox.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "BeatBox.elf" "BeatBox.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "BeatBox.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "BeatBox.elf" "BeatBox.a" "BeatBox.hex" "BeatBox.lss" "BeatBox.eep" "BeatBox.map" "BeatBox.srec" "BeatBox.usersignatures"
	