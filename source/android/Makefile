appName=app-release

buildPath=app/build/outputs/apk/release
JAVA_HOME=/usr/local/java/jdk-11.0.27/Contents/Home
GRADLE_HOME=/usr/local/gradle/wrapper/dists/gradle-7.5-bin/f1w0cc9nuo0eivz97x2xlu9sv/gradle-7.5
ANDROID_SDK=~/android-sdk
ANDROID_TOOLS_HOME=$(ANDROID_SDK)/build-tools/34.0.0
ANDROID_PLATFORM_HOME=$(ANDROID_SDK)/platform-tools
#/usr/local/android/android-sdk/emulator/emulator -netdelay none -netspeed full -avd Nexus_6_API_16

.PHONY: build package clean depen connect

build:
	@$(GRADLE_HOME)/bin/gradle build -x test

package:
	@$(GRADLE_HOME)/bin/gradle OpenRedPlayerCore:assemble

clean:
	@$(GRADLE_HOME)/bin/gradle clean

depen:
	@$(GRADLE_HOME)/bin/gradle app:dependencies


