# How to install application

The application is cross-platform, which means that it can be installed on any desktop operating system - macOS, Linux, Windows.
See instructions below for your OS.

## Linux

### Flatpak
* If you have integrated flathub in you distro (like Fedora) do you can just open Software Installer, search `ArdorQuery` and click on `Install`.  
  
* If you don't have flatpak you need install it first, check you distro install steps.  
After you install it you need to make followed commands:
```shell
sudo flatpak install flathub org.emptyflow.ArdorQuery
```

## Windows
### Winget
- Open console and write
```shell
winget install EmptyFlow.ArdorQuery
```
### Archive
- Open latest release by [link](https://github.com/trueromanus/ArdorQuery/releases/latest)
- Download file `ardorquery_windows-X.X.X.zip` (X will be version numbers)

## macOS

- Open latest release by [link](https://github.com/trueromanus/ArdorQuery/releases/latest)
- Download file `ardorquery_macos-X.X.X.zip` (X will be version numbers)
- Run application `Terminal` and make follow commands:
```shell
$ cd ~/Downloads/
$ xattr -d com.apple.quarantine <name of dmg file>
```
You don't need type full name of dmg file just type `ardor` and press `Tab` button.
- Open `Downloads` folder in `Finder` and just double click on dmg file for mount ArdorQuery disk.
- The mapped drive opens in `Finder` and you can launch ArdorQuery application.
