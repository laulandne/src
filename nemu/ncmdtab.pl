print "Starting:(@ARGV)\n";
foreach $file (@ARGV) {
  print "processing $file...\n";
  open(f,$file);
  open(o,">$file.tab");
  while(<f>) {
    if(/^(.*)if..awstrcmp.debug_cmd.\"(.*)\"(.*)$/) {
      print o "$2 : \n";
      print "$1\n";
    }
  };
  # That's it!
  close o;
  close f;
}
print "Done.\n";