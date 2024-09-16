# ArdorQuery
[![CI](https://github.com/trueromanus/ArdorQuery/actions/workflows/ci.yml/badge.svg)](https://github.com/trueromanus/ArdorQuery/actions/workflows/ci.yml)
[![Release](https://github.com/trueromanus/ArdorQuery/actions/workflows/BuildOnMac.yml/badge.svg)](https://github.com/trueromanus/ArdorQuery/actions/workflows/BuildOnMac.yml)  
ArdorQuery is a cross-platform tool for performing any HTTP(S) endpoints like REST API, HTML, Images, GraphQL, OData etc. It is similar to cURL and Postman. 
The main goal is to make an application with minimal user interface and keyboard oriented. Application is fully native, your don't need to install additional stuff. 
The HTTP request is broken down into a human-readable set of lines. Each line describes the type at the beginning, for example `url ` means we define the URL for the HTTP request.
Each type of line has a different color to help visually identify that content.
  
![Screenshoot](https://github.com/trueromanus/ArdorQuery/raw/main/src/screenshoot.png)
## Features
* Making insecure and secure HTTP(s) requests
* Small set of human-readable fields types
* Parts of request filled in separate text blocks with word wrap. The entire request is visible and readable.
* Import/Export field commands to/from clipboard or file
* Supporting h2 by default (can be lowered to HTTP/1.1 with special field)
* Sending any HTTP headers or custom headers
* Attaching files from the file system
* Sending forms
* Sending any body (aliases for JSON and XML)
* Highlight responses for popular formats - JSON, XML, HTML, CSS
* Showing responses as images - JPEG, PNG, GIF, WEBP, SVG
* Adding additional queries and switching between it
* Search text in body
* Image generation for attaching to the messager, bug tracker etc
* Import from OpenAPI v3 (supported only JSON body)
* Running multiple queries
* Scripting in modern JavaScript
* Control over redirection and SSL checks
* Global variables

## Documentation

Please see the latest documentation [here](https://trueromanus.github.io/ArdorQuery/).

## Supported platforms
* Windows 10+
* Linux
* macOS 12+

## Install
[<img src="https://dl.flathub.org/assets/badges/flathub-badge-en.png">](https://flathub.org/apps/org.emptyflow.ArdorQuery)

## Build Requirements
* Qt >= 6.2.3
## Build instructions
### Flatpak
```shell
sudo flatpak install flathub org.kde.Platform//6.6 org.kde.Sdk//6.6
flatpak-builder --user --install --force-clean build org.emptyflow.ArdorQuery.yml
flatpak build-bundle ~/.local/share/flatpak/repo/ ardorquery.flatpak org.emptyflow.ArdorQuery
```
### Debian based distros
```shell
cd src
qmake "CONFIG+=debianinstall"
```
### Manjaro
```shell
cd src
qmake "CONFIG+=manjaroinstall"
```
### Fedora based distros
```shell
cd src
qmake "CONFIG+=fedorainstall"
```
