# uefi-quine

The [Quine](http://en.wikipedia.org/wiki/Quine_%28computing%29) program run on [UEFI](http://en.wikipedia.org/wiki/Unified_Extensible_Firmware_Interface)

## Usage

I tested on Ubuntu 14.10 x64.

First, install [EDK2](https://github.com/tianocore/edk2):

```console
$ sudo apt install -y git build-essential nasm iasl uuid-dev
$ git clone https://github.com/tianocore/edk2.git
$ cd edk2
$ make -c BaseTools
$ . edksetup.sh
```

Second, build uefi-quine:

```console
$ git clone https://github.com/MakeNowJust/uefi-quine.git QuinePkg
$ cp QuinePkg/target.txt Conf/target.txt
$ build
```

Third, copy `Quine.efi` to EFI System Partition (ESP):

```console
$ sudo mkdir -p /boot/efi/EFI/quine
$ sudo cp Build/QuinePkg/RELEASE_GCC49/X64/Quine.efi /boot/efi/EFI/quine/quine.efi
```

Fourth, reboot your computer and execute `quine.efi` on EFI shell:

```console
on Ubuntu
$ shutdown -r now
...
on EFI shell
$ fs0:
$ cd EFI\quine
$ quine.efi
 _____ _____ ___    ___        _            
| ____|  ___|_ _|  / _ \ _   _(_)_ __   ___ 
|  _| | |_   | |  | | | | | | | | '_ \ / _ \
| |___|  _|  | |  | |_| | |_| | | | | |  __/
|_____|_|   |___|  \__\_\\__,_|_|_| |_|\___|
$ ls
listed up Quine.c, Quine.inf, QuinePkg.dec, QuinePkg.dsc and target.txt
```

Final, check Quine!

```console
on Ubuntu in directory edk2/QuinePkg
$ for file in Quine.c Quine.inf QuinePkg.dec QuinePkg.dsc target.txt; do diff $file /boot/efi/EFI/quine/$file; done
```


## LICENSE

MIT License: <https://makenowjust.github.io/license/mit?2015>
