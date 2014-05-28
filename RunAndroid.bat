set PATH=F:\adt-bundle-windows-x86_64-20131030\sdk\platform-tools;%PATH%
set JAVA_HOME=E:\Program Files\Java\jdk1.7.0_45
echo %PATH%
adb kill-server
adb start-server
cocos run -p android
