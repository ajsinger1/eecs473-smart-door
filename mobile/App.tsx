import { StatusBar } from "expo-status-bar";
import { StyleSheet, Text, Button, View } from "react-native";
import { logger } from "react-native-logs";
import { BleManager } from "react-native-ble-plx";
import React, { useState, useEffect, useCallback } from "react";

const log = logger.createLogger();
const manager = new BleManager();
const DEVICE_NAME = "Smart Handle";

export default function App() {
  const [isConnected, setIsConnected] = useState(false);
  const [device, setDevice] = useState(null);

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

      if (scannedDevice && scannedDevice.name === DEVICE_NAME) {
        manager.stopDeviceScan();
        scannedDevice
          .connect()
          .then((connectedDevice) => {
            setIsConnected(true);
            setDevice(connectedDevice);
          })
          .catch((error) => {
            log.error("Error during device connection:", error);
          });
      }
    });
  }, []);

  const disconnectDevice = () => {
    device.cancelConnection();
    setIsConnected(false);
    setDevice(null);
  };

  return (
    <View style={styles.container}>
      {isConnected ? (
        <>
          <Text>Connected to device!</Text>
          <Button title="Disconnect" onPress={disconnectDevice} />
        </>
      ) : (
        <>
          <Text>Not connected</Text>
          <Button title="Scan and Connect" onPress={scanAndConnect} />
        </>
      )}
      <StatusBar style="auto" />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: "#fff",
    alignItems: "center",
    justifyContent: "center",
  },
});
