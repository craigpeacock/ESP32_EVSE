# ESP32 IDF IEC61851/SAE J1772 Demo Code

Source code intended for IEC61851/J1772 ChargePort Controller:
https://circuitmaker.com/Projects/Details/Craig-Peacock-4/IEC61851-J1772-EVSE-Interface

The above interface is already working with a dsPIC33EP:
https://github.com/craigpeacock/dsPIC33EP_IEC61851_J1772_ChargePort_EVSE/tree/main/EVSE.X

The hope is to port it to the ESP32 and use WiFi/MQTT (on a single chip) to obtain electricity pricing information or solar PV instantaneous generation data.
