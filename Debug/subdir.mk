################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../genomeGenerator.cpp \
../hiddenmessage.cpp \
../rdtsc.cpp 

OBJS += \
./genomeGenerator.o \
./hiddenmessage.o \
./rdtsc.o 

CPP_DEPS += \
./genomeGenerator.d \
./hiddenmessage.d \
./rdtsc.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


