.PHONY: all clean main
PROJECT_NAME = calc
CC = g++ -std=c++17
OS := $(shell uname -s)
JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk-18.0.1.1.jdk/Contents/Home
package=edu.school.calc
package_path=edu/school/calc

clean:
	rm -f *.o *.gcno *.gcda *.info *.out */.clang-format .clang-format *.a model/*.gch ./report/ *.dSYM *.so *.dylib *.log
	@clear

jni_header: clean
	javac -h . ../java/$(package_path)/CppLib.java

jni_shit: jni_header
	$(CC) -c -o model.o model.cc
	$(CC) -c -o calcWrapper.o calcWrapper.cc
	$(CC) -c -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/darwin" javawrapper_nativeLib.cc -o javawrapper_nativeLib.o
	$(CC) -dynamiclib -o libnativeLib.dylib javawrapper_nativeLib.o calcWrapper.o model.o

jni_run: jni_shit
	#javac -cp ../java/ ../java/$(package_path)/*.java
	javac -cp ../java/ ../java/$(package_path)/Model.java ../java/$(package_path)/MainTest.java
	java -cp ../java/ -Djava.library.path="." $(package).MainTest $(package).CppLib

#all: prepareForJava
#
#prepareForJava: jni_shit
#	cp libnativeLib.dylib ../resources/edu/school/calc/

#main: clean
#	$(CC) main.cc model.cc -o model_core.out

#d_lib_create: clean
#ifeq ($(OS), Darwin)
#	$(CC) -c -o model.o model.cc
#	$(CC) -c -o calcWrapper.o calcWrapper.cc
#	$(CC) -dynamiclib -o libmodel.dylib model.o calcWrapper.o
#else
#	$(CC) -fPIC -c -o model.o model.cc
#	$(CC) -shared -o libmodel.so model.o
#endif