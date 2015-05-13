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

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../src/ \
../src/QTouch/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../alarm.c \
../buttons.c \
../comparator.c \
../ds3231.c \
../eeprom.c \
../indication.c \
../ir.c \
../main.c \
../sensors_handler.c \
../src/QTouch/init_mcu_atmega324.c \
../touch.c


PREPROCESSING_SRCS +=  \
../src/QTouch/qt_asm_tiny_mega.s


ASM_SRCS += 


OBJS +=  \
alarm.o \
buttons.o \
comparator.o \
ds3231.o \
eeprom.o \
indication.o \
ir.o \
main.o \
sensors_handler.o \
src/QTouch/init_mcu_atmega324.o \
src/QTouch/qt_asm_tiny_mega.o \
touch.o

OBJS_AS_ARGS +=  \
alarm.o \
buttons.o \
comparator.o \
ds3231.o \
eeprom.o \
indication.o \
ir.o \
main.o \
sensors_handler.o \
src/QTouch/init_mcu_atmega324.o \
src/QTouch/qt_asm_tiny_mega.o \
touch.o

C_DEPS +=  \
alarm.d \
buttons.d \
comparator.d \
ds3231.d \
eeprom.d \
indication.d \
ir.d \
main.d \
sensors_handler.d \
src/QTouch/init_mcu_atmega324.d \
src/QTouch/qt_asm_tiny_mega.d \
touch.d

C_DEPS_AS_ARGS +=  \
alarm.d \
buttons.d \
comparator.d \
ds3231.d \
eeprom.d \
indication.d \
ir.d \
main.d \
sensors_handler.d \
src/QTouch/init_mcu_atmega324.d \
src/QTouch/qt_asm_tiny_mega.d \
touch.d

OUTPUT_FILE_PATH +=clock.elf

OUTPUT_FILE_PATH_AS_ARGS +=clock.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler























./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -DNDEBUG  -I"../src/QTouch" -I".."  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega324pa -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	

src/QTouch/%.o: ../src/QTouch/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -DNDEBUG  -I"../src/QTouch" -I".."  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega324pa -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler
src/QTouch/qt_asm_tiny_mega.o: ../src/QTouch/qt_asm_tiny_mega.s
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Assembler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -Wa,-gdwarf2 -x assembler-with-cpp -c -mmcu=atmega324pa -I "../src/QTouch" -I ".."  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	


src/QTouch/%.o: ../src/QTouch/%.s
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Assembler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -Wa,-gdwarf2 -x assembler-with-cpp -c -mmcu=atmega324pa -I "../src/QTouch" -I ".."  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="clock.map" -Wl,--start-group -Wl,-lavr5g1-4qt-k-0rs -Wl,-lm  -Wl,--end-group -Wl,-L"../src/QTouch"  -Wl,--gc-sections -mmcu=atmega324pa 
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "clock.elf" "clock.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "clock.elf" "clock.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "clock.elf" > "clock.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "clock.elf" "clock.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-size.exe" "clock.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "clock.elf" "clock.a" "clock.hex" "clock.lss" "clock.eep" "clock.map" "clock.srec"
	