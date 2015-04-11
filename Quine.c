#include <Uefi.h>
#include <Guid/FileInfo.h>
#include <Library/UefiLib.h>
#include <Library/FileHandleLib.h>
#include <Library/ShellLib.h>
#include <Library/ShellCEntryLib.h>

CHAR16 *filenames[4] = {L"target.txt", L"QuinePkg.dec", L"QuinePkg.dsc", L"Quine.inf"};
CHAR8 *files[4] = {
  // target.txt
  "ACTIVE_PLATFORM=QuinePkg/QuinePkg.dsc\n"
  "TARGET=RELEASE\n"
  "TARGET_ARCH=X64\n"
  "TOOL_CHAIN_CONF=Conf/tools_def.txt\n"
  "TOOL_CHAIN_TAG=GCC49\n"
  "MAX_CONCURRENT_THREAD_NUMBER=2\n"
  "BUILD_RULE_CONF=Conf/build_rule.txt\n"
  "",
  // QuinePkg.dec
  "[Defines]\n"
  "  DEC_SPECIFICATION=0x00010005\n"
  "  PACKAGE_NAME=QuinePkg\n"
  "  PACKAGE_GUID=d7a685f6-4253-4151-80ae-7345459583c9\n"
  "  PACKAGE_VERSION=0.01\n"
  "",
  // QuinePkg.dsc
  "[Defines]\n"
  "  PLATFORM_NAME=QuinePkg\n"
  "  PLATFORM_GUID=67275cc5-1614-4a74-a7f9-cae3b8740399\n"
  "  PLATFORM_VERSION=0.01\n"
  "  DSC_SPECIFICATION=0x00010006\n"
  "  OUTPUT_DIRECTORY=Build/QuinePkg\n"
  "  SUPPORTED_ARCHITECTURES=IA32|X64|ARM|AARCH64\n"
  "  BUILD_TARGETS=DEBUG|RELEASE|NOOPT\n"
  "  SKUID_IDENTIFIER=DEFAULT\n"
  "[LibraryClasses]\n"
  "  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf\n"
  "  ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf\n"
  "  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf\n"
  "  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf\n"
  "  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf\n"
  "  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf\n"
  "  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf\n"
  "  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf\n"
  "  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf\n"
  "  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf\n"
  "  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf\n"
  "  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf\n"
  "  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf\n"
  "  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf\n"
  "  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf\n"
  "  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf\n"
  "  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf\n"
  "[Components]\n"
  "  QuinePkg/Quine.inf\n"
  "",
  // Quine.inf
  "[Defines]\n"
  "  INF_VERSION=0x00010006\n"
  "  BASE_NAME=Quine\n"
  "  FILE_GUID=953a9ad9-cea3-4e27-8c5a-4523bf904a7b\n"
  "  MODULE_TYPE=UEFI_APPLICATION\n"
  "  VERSION_STRING=0.1\n"
  "  ENTRY_POINT=ShellCEntryLib\n"
  "[Sources]\n"
  "  Quine.c\n"
  "[Packages]\n"
  "  MdePkg/MdePkg.dec\n"
  "  ShellPkg/ShellPkg.dec\n"
  "[LibraryClasses]\n"
  "  UefiLib\n"
  "  FileHandleLib\n"
  "  ShellLib\n"
  "  ShellCEntryLib\n"
  ""
};

CHAR8 *source =
  "#include <Uefi.h>\n"
  "#include <Guid/FileInfo.h>\n"
  "#include <Library/UefiLib.h>\n"
  "#include <Library/FileHandleLib.h>\n"
  "#include <Library/ShellLib.h>\n"
  "#include <Library/ShellCEntryLib.h>\n"
  "\n"
  "CHAR16 *filenames[4] = {L\"target.txt\", L\"QuinePkg.dec\", L\"QuinePkg.dsc\", L\"Quine.inf\"};\n"
  "CHAR8 *files[4] = {\n"
  "  // target.txt\n"
  "  $,\n"
  "  // QuinePkg.dec\n"
  "  $,\n"
  "  // QuinePkg.dsc\n"
  "  $,\n"
  "  // Quine.inf\n"
  "  $\n"
  "};\n"
  "\n"
  "CHAR8 *source =\n"
  "  ~;\n"
  "\n"
  "void write_file(CHAR16 *filename, CHAR8 *source) {\n"
  "  EFI_FILE_HANDLE handle;\n"
  "  UINTN size = AsciiStrSize(source) - 1;\n"
  "  ShellOpenFileByName(filename, (void**)&handle,\n"
  "    EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);\n"
  "  FileHandleWrite(handle, &size, source);\n"
  "  FileHandleClose(handle);\n"
  "}\n"
  "\n"
  "void print_escape(EFI_FILE_HANDLE handle, CHAR8 *source) {\n"
  "  CHAR8 c;\n"
  "  CHAR8 s[8];\n"
  "  UINTN size;\n"
  "  s[0] = 34; s[1] = 0;\n"
  "  size = AsciiStrSize(s) - 1;\n"
  "  FileHandleWrite(handle, &size, s);\n"
  "  while (c = *source++, c) {\n"
  "    switch (c) {\n"
  "    case 34: \n"
  "      s[0] = 92; s[1] = c;\n"
  "      size = 2;\n"
  "      break;\n"
  "    case 92:\n"
  "      s[0] = s[1] = c;\n"
  "      size = 2;\n"
  "      break;\n"
  "    case 10:\n"
  "      s[0] = 92; s[1] = 110; s[2] = 34; s[3] = c; s[4] = s[5] = 32; s[6] = 34;\n"
  "      size = 7;\n"
  "      break;\n"
  "    default:\n"
  "      s[0] = c;\n"
  "      size = 1;\n"
  "    }\n"
  "    s[size] = 0;\n"
  "    size = AsciiStrSize(s) - 1;\n"
  "    FileHandleWrite(handle, &size, s);\n"
  "  }\n"
  "  s[0] = 34; s[1] = 0;\n"
  "  size = AsciiStrSize(s) - 1;\n"
  "  FileHandleWrite(handle, &size, s);\n"
  "}\n"
  "\n"
  "INTN EFIAPI ShellAppMain (IN UINTN Argc, IN CHAR16 **Argv) {\n"
  "  CHAR8 c;\n"
  "  int i;\n"
  "  CHAR8 s[2];\n"
  "  CHAR8 *source2 = source;\n"
  "  EFI_FILE_HANDLE handle;\n"
  "  UINTN size;\n"
  "  for (i = 0; i < 4; i++) write_file(filenames[i], files[i]);\n"
  "  ShellOpenFileByName(L\"Quine.c\", (void**)&handle,\n"
  "    EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);\n"
  "  while (c = *source2++, c) {\n"
  "    if (36 == c) {\n"
  "      print_escape(handle, files[i++-4]);\n"
  "    } else if (126 == c) {\n"
  "      print_escape(handle, source);\n"
  "    } else {\n"
  "      s[0] = c; s[1] = 0;\n"
  "      size = AsciiStrSize(s) - 1;\n"
  "      FileHandleWrite(handle, &size, s);\n"
  "    }\n"
  "  }\n"
  "  FileHandleClose(handle);\n"
  "  Print(\n"
  "   L\" _____ _____ ___    ___        _            \\n\"\n"
  "   L\"| ____|  ___|_ _|  / _ \\\\ _   _(_)_ __   ___ \\n\"\n"
  "   L\"|  _| | |_   | |  | | | | | | | | '_ \\\\ / _ \\\\\\n\"\n"
  "   L\"| |___|  _|  | |  | |_| | |_| | | | | |  __/\\n\"\n"
  "   L\"|_____|_|   |___|  \\\\__\\\\_\\\\\\\\__,_|_|_| |_|\\\\___|\\n\"\n"
  "  );\n"
  "\n"
  "  return 0;\n"
  "}\n"
  "";

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
  Print(
   L" _____ _____ ___    ___        _            \n"
   L"| ____|  ___|_ _|  / _ \\ _   _(_)_ __   ___ \n"
   L"|  _| | |_   | |  | | | | | | | | '_ \\ / _ \\\n"
   L"| |___|  _|  | |  | |_| | |_| | | | | |  __/\n"
   L"|_____|_|   |___|  \\__\\_\\\\__,_|_|_| |_|\\___|\n"
  );

  return 0;
}
