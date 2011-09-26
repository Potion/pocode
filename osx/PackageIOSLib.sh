#!/bin/bash

lipo -create build/Debug-iphoneos/libPotionCode_iOS_d.a build/Debug-iphonesimulator/libPotionCode_iOS_d.a -output ./libPotionCode_ios_d.a;
lipo -create build/Release-iphoneos/libPotionCode_iOS.a build/Release-iphonesimulator/libPotionCode_iOS.a -output ./libPotionCode_ios.a;

mv libPotionCode_ios_d.a /PotionCode/libraries/potionCode;
mv libPotionCode_ios.a /PotionCode/libraries/potionCode;
