files = %w(target.txt QuinePkg.dec QuinePkg.dsc Quine.inf).map do |file|
  [file, File.read(file)]
end

source = <<EOF
#include <Uefi.h>
#include <Guid/FileInfo.h>
#include <Library/UefiLib.h>
#include <Library/FileHandleLib.h>
#include <Library/ShellLib.h>
#include <Library/ShellCEntryLib.h>

CHAR16 *filenames[4] = {#{files.map{|(name,_)|?L+name.inspect}.join(", ")}};
CHAR8 *files[4] = {
  $
};

CHAR8 *source =
  ~;

void write_file(CHAR16 *filename, CHAR8 *source) {
  EFI_FILE_HANDLE handle;
  UINTN size = AsciiStrSize(source) - 1;
  ShellOpenFileByName(filename, (void**)&handle,
    EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
  FileHandleWrite(handle, &size, source);
  FileHandleClose(handle);
}

void print_escape(EFI_FILE_HANDLE handle, CHAR8 *source) {
  CHAR8 c;
  CHAR8 s[8];
  UINTN size;
  s[0] = 34; s[1] = 0;
  size = AsciiStrSize(s) - 1;
  FileHandleWrite(handle, &size, s);
  while (c = *source++, c) {
    switch (c) {
    case 34: 
      s[0] = 92; s[1] = c;
      size = 2;
      break;
    case 92:
      s[0] = s[1] = c;
      size = 2;
      break;
    case 10:
      s[0] = 92; s[1] = 110; s[2] = 34; s[3] = c; s[4] = s[5] = 32; s[6] = 34;
      size = 7;
      break;
    default:
      s[0] = c;
      size = 1;
    }
    s[size] = 0;
    size = AsciiStrSize(s) - 1;
    FileHandleWrite(handle, &size, s);
  }
  s[0] = 34; s[1] = 0;
  size = AsciiStrSize(s) - 1;
  FileHandleWrite(handle, &size, s);
}

INTN EFIAPI ShellAppMain (IN UINTN Argc, IN CHAR16 **Argv) {
  CHAR8 c;
  int i;
  CHAR8 s[2];
  CHAR8 *source2 = source;
  EFI_FILE_HANDLE handle;
  UINTN size;
  for (i = 0; i < 4; i++) write_file(filenames[i], files[i]);
  ShellOpenFileByName(L"Quine.c", (void**)&handle,
    EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
  while (c = *source2++, c) {
    if (36 == c) {
      print_escape(handle, files[i++-4]);
    } else if (126 == c) {
      print_escape(handle, source);
    } else {
      s[0] = c; s[1] = 0;
      size = AsciiStrSize(s) - 1;
      FileHandleWrite(handle, &size, s);
    }
  }
  FileHandleClose(handle);
  Print(#{%q{
   L" _____ _____ ___    ___        _            \n"
   L"| ____|  ___|_ _|  / _ \\\\ _   _(_)_ __   ___ \n"
   L"|  _| | |_   | |  | | | | | | | | '_ \\\\ / _ \\\\\n"
   L"| |___|  _|  | |  | |_| | |_| | | | | |  __/\n"
   L"|_____|_|   |___|  \\\\__\\\\_\\\\\\\\__,_|_|_| |_|\\\\___|\n"
  }});

  return 0;
}
EOF

source.gsub!(/~/) {
  source.gsub(/\$/){
    files.map{|(name,_)|
      "// #{name}\n  $"
    }.join(",\n  ")
  }.split(/\n/).map{|line|(line+"\n").inspect}.join("\n  ") + "\n  \"\""
}

source.sub!(/\$/) {
  files.map{|(name, file)|
    "// #{name}\n  \"" + file.split(/\n/).join("\\n\"\n  \"") + "\\n\"\n  \"\""
  }.join(",\n  ")
}

puts source
