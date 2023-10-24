import React, {useState, useEffect, useCallback} from 'react';
import {SafeAreaView, Button, Text} from 'react-native';
import {BleManager} from 'react-native-ble-plx';

const manager = new BleManager();
const App = () => {
  const [isConnected, setIsConnected] = useState(false);
  const [device, setDevice] = useState(null);
  // Use useMemo to create the manager instance only once

  const scanAndConnect = useCallback(() => {
    manager.startDeviceScan(null, null, (error, scannedDevice) => {
      if (error) {
        console.log('Error during scanning:', error);
        return;
      }

      if (scannedDevice && scannedDevice.name === 'Smart Handle') {
        manager.stopDeviceScan();
        scannedDevice
          .connect()
          .then(connectedDevice => {
            setIsConnected(true);
            setDevice(connectedDevice);
          })
          .catch(error => {
            console.log('Error during device connection:', error);
          });
      }
    });
  }, []);

  useEffect(() => {
    const subscription = manager.onStateChange(state => {
      if (state === 'PoweredOn') {
        scanAndConnect();
      }
    }, true);
    return () => {
      subscription.remove();
    };
  }, [scanAndConnect]);

  const disconnectDevice = () => {
    device.disconnect();
    setIsConnected(false);
    setDevice(null);
  };

  return (
    <SafeAreaView
      style={{flex: 1, justifyContent: 'center', alignItems: 'center'}}>
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
    </SafeAreaView>
  );
};

export default App;
