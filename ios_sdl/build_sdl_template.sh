# clean up the framework, remove headers, extra files

temp=$BUILD_DIR/$BUILD_STYLE-template
# Wrong! 1. Can't assume location of Xcode directory (use xcode-select)
# 2. Project templates should go in Application Support directories anyway.
template_dir_name=\"SDL iOS Application\"
# dest=\"$(HOME)/Library/Application Support/Developer/Shared/Xcode/Project Templates/SDL/SDL iOS Application\"
rsync_flags=\"--exclude *.svn --links -r\"

# mkdir -p $dest
mkdir -p $temp
mkdir -p \"$temp/$template_dir_name/SDL/lib/\"
mkdir -p \"$temp/$template_dir_name/SDL/include\"

# copy template
rsync $rsync_flags \"../template/$template_dir_name\" $temp/

# copy Universal libSDL.a
rsync $rsync_flags -r $SYMROOT/$CONFIGURATION-Universal/libSDL.a \"$temp/$template_dir_name/SDL/lib/\"

# copy headers
rsync $rsync_flags ../../include/ \"$temp/$template_dir_name/SDL/include\"

#install (nah, don't install)
# cp -fr \"$temp/$template_dir_name\" \"$dest\"