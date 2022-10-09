# PIC32MK USB 
This repository contains [PIC32MK](https://www.microchip.com/en-us/products/microcontrollers-and-microprocessors/32-bit-mcus/pic32-32-bit-mcus/pic32mk) USB Device code generated using [MPLAB Harmony V3](https://www.microchip.com/en-us/tools-resources/configure/mplab-harmony). 

The target is a custom PCB containing a [PIC32MK1024GPK064](https://www.microchip.com/en-us/product/PIC32MK1024GPK064) with a 8MHz Crystal Oscillator. 

# Documentation and Resources

* [Microchip MPLAB Harmony Github Repo](https://github.com/Microchip-MPLAB-Harmony)
* [USB Device Applications with MPLAB Harmony USB Stack](https://mu.microchip.com/usb-device-applications-with-mplab-harmony-usb-stack)

MPLAB Harmony 3 Content Manager will download harmony components to a folder on your local drive. Within this root harmony folder should be a [usb\doc](https://github.com/Microchip-MPLAB-Harmony/usb/tree/master/doc) folder containg 
* [help_harmony_usb.pdf](https://github.com/Microchip-MPLAB-Harmony/usb/raw/master/doc/help_harmony_usb.pdf) - Acrobat PDF document
* help_harmony_usb.chm - Windows help file 

# Harmony notes:

* [PIC32MK_USB/firmware/src/config/default/default.mhc/](/craigpeacock/PIC32MK_USB/tree/main/firmware/src/config/default/default.mhc) contains the configuration files for Harmony.

* The remainder of the [PIC32MK_USB/firmware/src/config/default/](/craigpeacock/PIC32MK_USB/tree/main/firmware/src/config/default) folder contains files automatically generated by Harmony and can be deleted and.or recreated by Harmony. They have been included in the repo to allow the code to successfully build without having to use Harmony.


