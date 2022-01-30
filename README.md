# ESP32 IDF IEC61851/SAE J1772 Demo Code

![Prototype EVSE Interface](https://www.beyondlogic.org/wp-content/uploads/2022/01/IEC61851-J1772-EVSE-PCB.png)

Source code intended for a Prototype IEC61851/J1772 EVSE Interface:<BR>
https://www.beyondlogic.org/prototype-iec61851-j1772-evse-interface/

The above interface is already working with a dsPIC33EP:<BR>
https://github.com/craigpeacock/dsPIC33EP_IEC61851_J1772_ChargePort_EVSE/tree/main/EVSE.X

The hope is to port it to the ESP32 and use WiFi/MQTT (on a single chip) to obtain electricity pricing information or solar PV instantaneous generation data.
