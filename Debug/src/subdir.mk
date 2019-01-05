################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Array2d_32f.cpp \
../src/Audio.cpp \
../src/AudioQ.cpp \
../src/HashQ.cpp \
../src/Stft.cpp \
../src/TCPServer.cpp \
../src/VAD.cpp \
../src/VIATUtilities.cpp \
../src/ViatFeatureExtractor.cpp \
../src/Wave.cpp \
../src/YapHash.cpp \
../src/melFB.cpp \
../src/parseConfig.cpp 

OBJS += \
./src/Array2d_32f.o \
./src/Audio.o \
./src/AudioQ.o \
./src/HashQ.o \
./src/Stft.o \
./src/TCPServer.o \
./src/VAD.o \
./src/VIATUtilities.o \
./src/ViatFeatureExtractor.o \
./src/Wave.o \
./src/YapHash.o \
./src/melFB.o \
./src/parseConfig.o 

CPP_DEPS += \
./src/Array2d_32f.d \
./src/Audio.d \
./src/AudioQ.d \
./src/HashQ.d \
./src/Stft.d \
./src/TCPServer.d \
./src/VAD.d \
./src/VIATUtilities.d \
./src/ViatFeatureExtractor.d \
./src/Wave.d \
./src/YapHash.d \
./src/melFB.d \
./src/parseConfig.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -g -I../FrameWave_1.3.1 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


