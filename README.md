# IOCTL Parser

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE.txt)
[![AppVeyor](https://img.shields.io/appveyor/build/EvanMcBroom/ioctl-parser?logo=AppVeyor)](https://ci.appveyor.com/project/EvanMcBroom/ioctl-parser/)

A simple tool for parsing and identifying IO control (IOCTL) values and their subcomponents.
The structure of an IOCTL value is documented on MSDN.<sup>1</sup>

![IOCTL Parser](screenshot.png)

Name identification for devices and functions are supported by using an embedded offline copy of the values on the "Windows IOCTL Reference" website.<sup>2</sup>
The tool can support identifying additional device and function names by passing configuration files as arguments.
If you are interested in doing this, please use the structure of the [provided example file](example.ini) or the [embedded configuration file](resources/IOCTLs.ini) as a reference.

## References

1. [Defining I/O Control Codes](https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/defining-i-o-control-codes)
2. [Windows IOCTL Reference](http://www.ioctls.net/)