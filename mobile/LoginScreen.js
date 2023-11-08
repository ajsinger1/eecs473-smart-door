import { StatusBar } from 'expo-status-bar';
import { Button, StyleSheet, Text, View, TextInput, TouchableOpacity, ScrollView } from 'react-native';
import { useState } from 'react';

export default function Login({navigation}) {
    const [state,setState] = useState({
        email: '',
        password: '',
    })
        
  return (
   // <ScrollView>
    <View style={styles.container}>
      
        <Text style={styles.title}> Login</Text>
        <View style={styles.inputView}>
            <TextInput
                style={styles.inputText}
                placeholder="Username"
                placeholderTextColor="#003f5c"
                onChangeText={text => setState({email:text})}/>
        </View>
        <View style={styles.inputView}>
            <TextInput
            style={styles.inputText}
            secureTextEntry
            placeholder="Password"
            placeholderTextColor="#003f5c"
            onChangeText={text => setState({password:text})}/>
        </View>

            <Button  title='Submit'  onPress={() => navigation.navigate('Main')}/>
    </View>
   // </ScrollView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
  title:{
    fontWeight: "bold",
    fontSize:50,
    color:"blue",
    marginBottom: 40,
    },
    
  inputView:{
    width:300,
    //backgroundColor:"#3AB4BA",
    //borderRadius:25,
    borderColor: "black",
    borderWidth: 1,
    height:50,
    marginBottom:20,
    justifyContent:"center",
    padding:20
    },
    inputText:{
    height:50,
    color:"black"
    },
    button:{

    }
});