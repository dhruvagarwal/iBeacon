/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "mbed.h"
#include "BLEDevice.h"
 
/**
 * For this demo application, populate the beacon advertisement payload
 * with 2 AD structures: FLAG and MSD (manufacturer specific data).
 *
 * Reference:
 *  Bluetooth Core Specification 4.0 (Vol. 3), Part C, Section 11, 18
 */
 
BLEDevice ble;
 
    /**
     * The Beacon payload (encapsulated within the MSD advertising data structure)
     * has the following composition:
     * 128-Bit UUID = E2 0A 39 F4 73 F5 4B C4 A1 2F 17 D1 AD 07 A9 61
     * Major/Minor  = 0000 / 0000
     * Tx Power     = C8 (-56dB)
     */
    const static uint8_t iBeaconPayload[] = {
        0x59, 0x00, // Company identifier code (0x0059 == Nordic)
        0x02,       // ID
        0x15,       // length of the remaining payload
        0xE2, 0x0A, 0x39, 0xF4, 0x73, 0xF5, 0x4B, 0xC4, // location UUID
        0xA1, 0x2F, 0x17, 0xD1, 0xAD, 0x07, 0xA9, 0x61,
        0x00, 0x00, // the major value to differentiate a location
        0x00, 0x00, // the minor value to differentiate a location
        0xC8        // 2's complement of the Tx power (-56dB)
    };
const static char     DEVICE_NAME[]        = "agga";

void disconnectionCallback(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    ble.startAdvertising(); // restart advertising
}

int main(void)
{
    /* Initialize BLE baselayer */
    ble.init();
    ble.onDisconnection(disconnectionCallback);
    
    /* Set up iBeacon data*/
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE );
    ble.accumulateAdvertisingPayload(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, iBeaconPayload, sizeof(iBeaconPayload));
 
    ble.setAdvertisingType(GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED);
    /* Set advertising interval. Longer interval = longer battery life */
    ble.setAdvertisingInterval(160); /* 100ms; in multiples of 0.625ms. */
    ble.startAdvertising();
 
    for (;;) {
        ble.waitForEvent(); 
    }
}
 
