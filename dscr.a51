;;-----------------------------------------------------------------------------
;;   File:      dscr.a51
;;   Contents:  This file contains descriptor data tables.
;;
;; $Archive: /USB/Examples/FX2LP/hid_kb/dscr.a51 $
;; $Date: 5/27/04 1:25p $
;; $Revision: 2 $
;;
;;
;;-----------------------------------------------------------------------------
;; Copyright 2003, Cypress Semiconductor Corporation
;;-----------------------------------------------------------------------------
;;-----------------------------------------------------------------------------
   
DSCR_DEVICE   equ   1   ;; Descriptor type: Device
DSCR_CONFIG   equ   2   ;; Descriptor type: Configuration
DSCR_STRING   equ   3   ;; Descriptor type: String
DSCR_INTRFC   equ   4   ;; Descriptor type: Interface
DSCR_ENDPNT   equ   5   ;; Descriptor type: Endpoint
DSCR_DEVQUAL  equ   6   ;; Descriptor type: Device Qualifier

DSCR_DEVICE_LEN   equ   18
DSCR_CONFIG_LEN   equ    9
DSCR_INTRFC_LEN   equ    9
DSCR_ENDPNT_LEN   equ    7
DSCR_DEVQUAL_LEN  equ   10

ET_CONTROL   equ   0   ;; Endpoint type: Control
ET_ISO       equ   1   ;; Endpoint type: Isochronous
ET_BULK      equ   2   ;; Endpoint type: Bulk
ET_INT       equ   3   ;; Endpoint type: Interrupt

public      DeviceDscr, DeviceQualDscr, HighSpeedConfigDscr, FullSpeedConfigDscr, StringDscr, UserDscr, HIDDscr
public      HIDReportDscr, HIDReportDscrEnd
DSCR   SEGMENT   CODE PAGE

;;-----------------------------------------------------------------------------
;; Global Variables
;;-----------------------------------------------------------------------------
      rseg DSCR      ;; locate the descriptor table in on-part memory.

DeviceDscr:   
      db   DSCR_DEVICE_LEN      ;; Descriptor length
      db   DSCR_DEVICE   ;; Decriptor type
      dw   0002H      ;; Specification Version (BCD)
      db   00H        ;; Device class
      db   00H         ;; Device sub-class
      db   00H         ;; Device sub-sub-class
      db   64         ;; Maximum packet size
      db   01H,02H      ;; Vendor ID
      db   03H,04H      ;; Product ID (Sample Device)
      dw   0000H      ;; Product version ID
      db   1         ;; Manufacturer string index
      db   2         ;; Product string index
      db   0         ;; Serial number string index
      db   1         ;; Number of configurations

DeviceQualDscr:
      db   DSCR_DEVQUAL_LEN   ;; Descriptor length
      db   DSCR_DEVQUAL   ;; Decriptor type
      dw   0002H      ;; Specification Version (BCD)
      db   00H        ;; Device class
      db   00H         ;; Device sub-class
      db   00H         ;; Device sub-sub-class
      db   64         ;; Maximum packet size
      db   1         ;; Number of configurations
      db   0         ;; Reserved

HighSpeedConfigDscr:   
      db   DSCR_CONFIG_LEN               ;; Descriptor length
      db   DSCR_CONFIG                  ;; Descriptor type
      db   (HighSpeedConfigDscrEnd-HighSpeedConfigDscr) mod 256 ;; Total Length (LSB)
      db   (HighSpeedConfigDscrEnd-HighSpeedConfigDscr)  /  256 ;; Total Length (MSB)
      db   1      ;; Number of interfaces
      db   1      ;; Configuration number
      db   0      ;; Configuration string
      db   10100000b   ;; Attributes (b7 - buspwr, b6 - selfpwr, b5 - rwu)
      db   50      ;; Power requirement (div 2 ma)

;; Interface Descriptor
      db   DSCR_INTRFC_LEN      ;; Descriptor length
      db   DSCR_INTRFC         ;; Descriptor type
      db   0               ;; Zero-based index of this interface
      db   0               ;; Alternate setting
      db   2               ;; Number of end points 
      db   03H            ;; Interface class
      db   00H               ;; Interface sub class
      db   00H               ;; Interface sub sub class
      db   0               ;; Interface descriptor string index

HIDDscr:
      db   09h      ; length
      db   21h      ; type: HID
      db   010h,01h      ; release: HID class rev 1.1
      db   00h      ; country code (none)
      db   01h      ; number of HID class descriptors to follow
      db   22h      ; report descriptor type (HID)
      db   (HIDReportDscrEnd - HIDReportDscr) ; length of HID descriptor
      db   00h
HIDDscrEnd:
      
;; Endpoint Descriptor
      db   DSCR_ENDPNT_LEN      ;; Descriptor length
      db   DSCR_ENDPNT         ;; Descriptor type
      db   81H               ;; Endpoint number, and direction
      db   ET_INT            ;; Endpoint type
      db   40H               ;; Maximun packet size (LSB)
      db   00H               ;; Max packect size (MSB)
      db   0AH               ;; Polling interval

;; Endpoint Descriptor
      db   DSCR_ENDPNT_LEN      ;; Descriptor length
      db   DSCR_ENDPNT         ;; Descriptor type
      db   01H               ;; Endpoint number, and direction
      db   ET_INT            ;; Endpoint type
      db   40H               ;; Maximun packet size (LSB)
      db   00H               ;; Max packect size (MSB)
      db   0AH               ;; Polling interval

HighSpeedConfigDscrEnd:   

      db    00h               ;; pad

FullSpeedConfigDscr:   
      db   DSCR_CONFIG_LEN               ;; Descriptor length
      db   DSCR_CONFIG                  ;; Descriptor type
      db   (FullSpeedConfigDscrEnd-FullSpeedConfigDscr) mod 256 ;; Total Length (LSB)
      db   (FullSpeedConfigDscrEnd-FullSpeedConfigDscr)  /  256 ;; Total Length (MSB)
      db   1      ;; Number of interfaces
      db   1      ;; Configuration number
      db   0      ;; Configuration string
      db   10100000b   ;; Attributes (b7 - buspwr, b6 - selfpwr, b5 - rwu)
      db   50      ;; Power requirement (div 2 ma)

;; Interface Descriptor
      db   DSCR_INTRFC_LEN      ;; Descriptor length
      db   DSCR_INTRFC         ;; Descriptor type
      db   0               ;; Zero-based index of this interface
      db   0               ;; Alternate setting
      db   2               ;; Number of end points 
      db   03H            ;; Interface class
      db   00H               ;; Interface sub class
      db   00H               ;; Interface sub sub class
      db   0               ;; Interface descriptor string index

;; HID Descriptor
      db   09h      ; length
      db   21h      ; type: HID
      db   010h,01h      ; release: HID class rev 1.1
      db   00h      ; country code (none)
      db   01h      ; number of HID class descriptors to follow
      db   22h      ; report descriptor type (HID)
      db   (HIDReportDscrEnd - HIDReportDscr) ; length of HID descriptor
      db   00h
      
;; Endpoint Descriptor
      db   DSCR_ENDPNT_LEN      ;; Descriptor length
      db   DSCR_ENDPNT         ;; Descriptor type
      db   81H               ;; Endpoint number, and direction
      db   ET_INT            ;; Endpoint type
      db   40H               ;; Maximun packet size (LSB)
      db   00H               ;; Max packect size (MSB)
      db   0AH               ;; Polling interval

;; Endpoint Descriptor
      db   DSCR_ENDPNT_LEN      ;; Descriptor length
      db   DSCR_ENDPNT         ;; Descriptor type
      db   01H               ;; Endpoint number, and direction
      db   ET_INT            ;; Endpoint type
      db   40H               ;; Maximun packet size (LSB)
      db   00H               ;; Max packect size (MSB)
      db   0AH               ;; Polling interval

FullSpeedConfigDscrEnd:   

      db    00h               ;; pad
StringDscr:

StringDscr0:   
      db   StringDscr0End-StringDscr0      ;; String descriptor length
      db   DSCR_STRING
      db   09H,04H
StringDscr0End:

StringDscr1:   
      db   StringDscr1End-StringDscr1      ;; String descriptor length
      db   DSCR_STRING
      db   'W',00
      db   'y',00
      db   'v',00
      db   'e',00
      db   'r',00
      db   'n',00
StringDscr1End:

StringDscr2:   
      db   StringDscr2End-StringDscr2      ;; Descriptor length
      db   DSCR_STRING
      db   'H',00
      db   'e',00
      db   'r',00
      db   'o',00
      db   'e',00
      db   's',00
StringDscr2End:

HIDReportDscr:
      db 06h, 0A0h, 0FFh     ; Usage Page (Generic Desktop)
      db 09h, 0A5h     ; Usage (Keyboard)
      db 0A1h, 01h     ; Collection (Application)
      db 09h, 0A6h     ; Usage (vendor definded)

      db 09h, 0A7h     ; Usage (vendor definded)
      db 15h, 00h       ; Logical Minimum
      db 25h, 7Fh       ; Logical Maximum
      db 75h, 08h       ; Report Size
      db 95h, 01h       ; Report Count
      db 81h, 02h       ; Input

      db 09h, 0A9h     ; Usage (vendor definded)
      db 15h, 00h       ; Logical Minimum
      db 25h, 0Fh       ; Logical Maximum
      db 75h, 08h       ; Report Size
      db 95h, 01h       ; Report Count
      db 91h, 02h       ; Output

      db 0C0h          ; End Collection
HIDReportDscrEnd:

UserDscr:      
      dw   0000H
      end
      
