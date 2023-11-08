import { StatusBar } from 'expo-status-bar';
import { StyleSheet, Text, View, TouchableOpacity, Image, Alert, Button } from 'react-native';
import { BleManager, Characteristic } from 'react-native-ble-plx';
import React, { useState, useEffect, useCallback } from "react";
import { logger } from 'react-native-logs';
import Globals from './Globals';
import base64 from 'react-native-base64';

const log = logger.createLogger();
const manager = new BleManager();
const DEVICE_NAME = "Smart Handle";
var value = '1';

export default function Main({ navigation }) {
    const [isConnected, setIsConnected] = useState(false);
    const [device, setDevice] = useState(null);
    const [handleState, setHandleState] = useState("");
    const [mode, setMode] = useState("");

    const getServicesAndCharacteristics = (device) => {
        return new Promise((resolve, reject) => {
            device.services().then(services => {
                const characteristics = []

                services.forEach((service, i) => {
                    service.characteristics().then(c => {
                        characteristics.push(c)

                        if (i === services.length - 1) {
                            const temp = characteristics.reduce(
                                (acc, current) => {
                                    return [...acc, ...current]
                                },
                                []
                            )
                            const dialog = temp.find(
                                characteristic =>
                                    characteristic.isWritableWithoutResponse
                            )
                            if (!dialog) {
                                reject('No writable characteristic')
                            }
                            resolve(dialog)
                        }
                    })
                })
            })
        })
    }

    const getHandleStatus = () => {
        let fetchurl = Globals.baseURL + '/state/handle';

        fetch(fetchurl)
            .then((response) => response.json())
            .then((json) => {
                setHandleState(json["state"]);
                setMode(json["mode"]);
            }

            )
            .catch((error) => Alert.alert("Something Went Wrong", error));
    }

    const scanAndConnect = useCallback(() => {
        manager
            .state()
            .then((currentState) => {
                if (currentState !== "PoweredOn") {
                    log.info(`Phone not ready for scanning (${manager.state()})`);
                    return;
                }
            })
            .catch((error) => {
                log.error(error);
                return;
            });
        log.info("Scanning...");
        manager.startDeviceScan(null, null, (error, scannedDevice) => {
            if (scannedDevice.name) {
                log.info(`Device: ${scannedDevice.name}`);
            }
            if (error) {
                log.error("Error during scanning:", error);
                return;
            }
            if (scannedDevice.id) {
                log.info(`Id: ${scannedDevice.id}`);
            }

            if (scannedDevice && scannedDevice.name === DEVICE_NAME) {
                manager.stopDeviceScan();
                scannedDevice
                    .connect()
                    .then((connectedDevice) => {
                        setIsConnected(true);
                        setDevice(connectedDevice);
                        log.info("connected");
                        // console.log(connectedDevice.discoverAllServicesAndCharacteristics());
                    })
                    .catch((error) => {
                        log.error("Error during device connection:", error);
                    });
            }
        });
    }, []);

    const sendLockCommand = () => {
        device.discoverAllServicesAndCharacteristics().then((device) => {
            device.writeCharacteristicWithResponseForService("1234", "4321", base64.encode("LOCK")).then(
                (characteristic) => {
                    log.info(`Characteristic value: ${characteristic.value}`);
                    return;
                }
            )
        })
    }

    const sendUnlockCommand = () => {
        device.discoverAllServicesAndCharacteristics().then((device) => {
            device.writeCharacteristicWithResponseForService("1234", "4321", base64.encode("UNLOCK")).then(
                (characteristic) => {
                    log.info(`Characteristic value: ${characteristic.value}`);
                    return;
                }
            )
        })
    }

    const disconnectDevice = () => {
        device.cancelConnection();
        setIsConnected(false);
        setDevice(null);
    };
    return (
        <View style={styles.container}>
            {isConnected ? (
                <>
                    <Button title="disconnect" onPress={disconnectDevice}/>
                </>
            ) : (
                <>
                    <Button title="connect" onPress={scanAndConnect}/>
                </>
            )}
            <TouchableOpacity style={styles.lockBorder}  onPress={sendLockCommand}>
                <Image source={require('./assets/lock-icon.png')} style={styles.locks} />
            </TouchableOpacity>
            <TouchableOpacity style={styles.lockBorder} onPress={sendUnlockCommand}>
                <Image source={require('./assets/unlock-icon.png')} style={styles.locks} />
            </TouchableOpacity>
            <TouchableOpacity onPress={() => navigation.navigate('Settings')} >
                <Image source={require('./assets/settings2.png')} style={{ position: 'absolute', left: -170, top: 100, height: 40, width: 40 }} />
            </TouchableOpacity>

        </View>
    );
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor: '#fff',
        alignItems: 'center',
        justifyContent: 'center',
    },
    locks: {
        borderRadius: 200,
        borderColor: 'blue',
        //backgroundColor: 'blue',
        borderWidth: 2,
        height: 100,
        width: 100,
    },
    lockBorder: {
        // borderRadius: 200,
        // borderColor: 'black',
        // borderWidth: 2,
        // height: 100,
        // width: 100,
        margin: 40

    }
});