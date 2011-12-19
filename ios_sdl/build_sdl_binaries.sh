# Build the simulator binary
# xcodebuild -project SDLiPhoneOS.xcodeproj -target libSDL -configuration Release -sdk iphonesimulator
xcodebuild -project SDLiPhoneOS.xcodeproj -target libSDL -configuration $CONFIGURATION -sdk iphonesimulator

# Build the device binary
# xcodebuild -project SDLiPhoneOS.xcodeproj -target libSDL -configuration Release -sdk iphoneos
xcodebuild -project SDLiPhoneOS.xcodeproj -target libSDL -configuration $CONFIGURATION -sdk iphoneos

# create a simulator/device fat binary from the 2 separate builds
lipo $SYMROOT/$CONFIGURATION-iphonesimulator/libSDL.a $SYMROOT/$CONFIGURATION-iphoneos/libSDL.a -output $SYMROOT/$CONFIGURATION-Universal/libSDL.a -create

echo \"Created Universal Simulator/Device binary at $SYMROOT/$CONFIGURATION-Universal/libSDL.a\"


exit 0
