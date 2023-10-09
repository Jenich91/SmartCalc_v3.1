all: install
INSTALL_PATH=$(HOME)/Desktop
VERSION=3.0
PROJECT_NAME=SmartCalc-$(VERSION)
FILENAME=$(PROJECT_NAME)-jar-with-dependencies

#	!!BEFORE RUN - MAYBE NEED SET JAVA_HOME and M2_HOME ENV VARIABLE!!
#	For mac:
#	set path to jdk, like:
#	if you know path type:
#	export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk-18.0.1.1.jdk/Contents/Home
#	or try find:
#	echo $(/usr/libexec/java_home)
#
#	download maven, and extract somewhere
#	(cd /opt/goinfre/$(whoami) && curl https://dlcdn.apache.org/maven/maven-3/3.9.4/binaries/apache-maven-3.9.4-bin.zip | tar xvz && chmod a+x apache-maven-3.9.4/bin/mvn)
#
#	set path to maven, like:
#	export M2_HOME=/opt/goinfre/$(whoami)/apache-maven-3.9.4
#
#
#	add to path evn var
#	PATH=$PATH:$JAVA_HOME/bin:$M2_HOME/bin
#	export PATH

#	maven wrapper install
#	mvn -N io.takari:maven:wrapper

#	For linux(ubuntu):
#	wget https://download.java.net/java/GA/jdk18/43f95e8614114aeaa8e8a5fcf20a682d/36/GPL/openjdk-18_linux-x64_bin.tar.gz
#	tar -xvf openjdk-18_linux-x64_bin.tar.gz
#	sudo mv jdk-18* /opt/
#	export JAVA_HOME=/opt/jdk-18

#	sudo apt install maven
#	export M2_HOME=/usr/bin/mvn
#	PATH=$PATH:$JAVA_HOME/bin:$M2_HOME/bin
#	export PATH

clean:
	./mvnw clean
	make -C src/main/cpp clean
	rm -rf src/resources/libnativeLib.dylib src/main/resources/edu/school/calc/libnativeLib.dylib src/main/java/edu/school/calc/*.class
	find . -name ".DS_Store" -type f -delete

install: clean uninstall
	./mvnw install -DskipTests
	cp -r ./Build/ $(INSTALL_PATH)/$(PROJECT_NAME)
	chmod +x $(INSTALL_PATH)/$(PROJECT_NAME)/$(FILENAME).jar
	ln -sv $(INSTALL_PATH)/$(PROJECT_NAME)/$(FILENAME).jar ~/Desktop/$(FILENAME).desktop

open:
	open $(INSTALL_PATH)/$(PROJECT_NAME)/$(FILENAME).jar

#open_from_console:
#	java -jar $(INSTALL_PATH)/$(PROJECT_NAME)/$(FILENAME).jar

uninstall:
	rm -rf $(INSTALL_PATH)/$(PROJECT_NAME)
	rm -f ~/Desktop/$(FILENAME).desktop

test:
	./mvnw test

report: test
	open -a 'google chrome' target/site/jacoco/edu.school.calc/Controller.html