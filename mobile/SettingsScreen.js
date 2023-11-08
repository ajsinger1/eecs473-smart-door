import { StatusBar } from 'expo-status-bar';
import { FlatList, StyleSheet, Text, View, TouchableOpacity, Switch, Button, Alert, Image, Modal } from 'react-native';
import ModalSelector from 'react-native-modal-selector';
import React, { useState, useEffect } from 'react';
import Globals from './Globals';
const data = [
    { key: 1, label: 'UNLOCK' },
    { key: 2, label: 'DO_NOT_UNLOCK' },
    { key: 3, label: 'LOCKOUT' },
];


const Item = ({ item, onPress, onModalClose, backgroundColor, textColor }) => (
    <TouchableOpacity onPress={onPress} style={[styles.item, { backgroundColor, flexDirection: 'horizontal' }]}>
        <Text style={[styles.title, { color: textColor, flex: 2 }]}>{item.name}</Text>
        {item.state == 'DO_NOT_UNLOCK' &&
        <Image style={{flex: 1, height: 20, width: 20}} source={require('./assets/lock-icon.png')}/>}
        {item.state == 'UNLOCK' &&
        <Image style={{flex: 1, height: 20, width: 20}} source={require('./assets/unlock-icon.png')}/>}
        {item.state == 'LOCKOUT' &&
        <Image style={{flex: 1, height: 20, width: 20}} source={require('./assets/lockout1.png')}/>}

        {item.state == 'DO_NOT_UNLOCK' && 
        <ModalSelector 
                //visible={modalVis}
                data={data}
                onModalClose={(data) => {
                    updateUserState(item, data.label);
                }}
                selectedKey={2}
        
            />}
        {item.state == 'UNLOCK' && 
        <ModalSelector 
                //visible={modalVis}
                data={data}
                onModalClose={(data) => {
                    updateUserState(item, data.label);
                }}
                selectedKey={1}
            />}
        {item.state == 'LOCKOUT' && 
        <ModalSelector 
                //visible={modalVis}
                data={data}
                onModalClose={(data) => {
                    updateUserState(item, data.label);
                }}
                selectedKey={3}
            />}
        

    </TouchableOpacity>
);

const updateUserState = (item, data) => {
    fetch(Globals.baseURL + "/state/users/" + item.user_id, {
        method: "PUT",
        body: JSON.stringify({
        state: data,
        }),
    })
        .then((response) => response.json())
        .then((responseData) => {
        console.log(JSON.stringify(responseData));
        })
        
}


export default function Settings() {
    const [selectedId, setSelectedId] = useState(0);
    const [isEnabled, setIsEnabled] = useState(false);
    const [users, setUsers] = useState([]);
    const [fetched, setFetched] = useState(false);
    const [modalVis, setModalVis] = useState(false);

    const toggleSwitch = () => setIsEnabled(previousState => !previousState);

    

    const getUsers = () => {
        let fetchurl = Globals.baseURL + '/state/users';
    
        fetch(fetchurl)
            .then((response) => response.json())
            .then((json) => {
                setUsers(json);
                setFetched(true);
            }
    
            )
            .catch((error) => Alert.alert("Something Went Wrong", error));
    }

    useEffect(() => {
        getUsers();
        
        
    });

    const renderItem = ({ item }) => {
        const backgroundColor =  '#f9c2ff';
        const color = 'black';

        return (
            <Item
                item={item}
                // onPress={() => setSelectedId(item.user_id)}
                backgroundColor={backgroundColor}
                textColor={color}
            />
        );
    };

    const updateUserState = () => {

    }
 
    return (
        
        <View style={styles.container}>
            
            <View style={{flex: 2}}>
                <View style={{flexDirection: 'row', marginTop: 20}}>
                <Text style={{fontSize: 30, marginRight: 20}}>Geo-mode</Text>
                <Switch
                    trackColor={{false: '#767577', true: '#81b0ff'}}
                    thumbColor={isEnabled ? '#f5dd4b' : '#f4f3f4'}
                    onValueChange={toggleSwitch}
                    value={isEnabled}
                    style={{marginTop: 5}}
                 />
                 </View>
            </View>
            <View style={{flex: 6}}>
                <Text style={{fontSize: 30}}>User Management:</Text>
                <FlatList data={users} renderItem={renderItem}
                    keyExtractor={item => item.user_id}
                    extraData={selectedId}
                    onPress={() => {
                        setSelectedId(item.user_id);
                        setModalVis(true);
                    }}

                    />
            </View>
            <View style={{flex: 1}}>
                <Button title='Invite'/>
            </View>
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
    item: {
        padding: 20,
        marginVertical: 8,
        marginHorizontal: 16,
    },
    title: {
        fontSize: 32,
    },
});
