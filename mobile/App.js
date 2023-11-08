import { StatusBar } from 'expo-status-bar';
import { StyleSheet, Text, View } from 'react-native';
import Login from './LoginScreen';
import Main from './MainScreen';
import Settings from './SettingsScreen';
import { NavigationContainer } from '@react-navigation/native';
import { createNativeStackNavigator } from '@react-navigation/native-stack';

const RootStack = createNativeStackNavigator();

export default function App() {
  return (
    <NavigationContainer>
      <RootStack.Navigator headerMode='none'>
        <RootStack.Screen name="Login" component={Login} options={{headerShown: false}}/>
        <RootStack.Screen name="Main"  component={Main} options={{headerShown: false}}/>
        <RootStack.Screen name="Settings"  component={Settings}/>
      </RootStack.Navigator>
    </NavigationContainer>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
});
