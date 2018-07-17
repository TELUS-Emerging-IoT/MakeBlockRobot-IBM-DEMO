import React, {Component} from 'react';
import { Alert, StyleSheet, Image, Text, View, TouchableOpacity } from 'react-native';

//Requests with pre-defined payload values for movement
const req_moveForward = new Request("https://vancouverrobotibm.mybluemix.net/request?payload=forward&topic=");
const req_moveBackwards = new Request("https://vancouverrobotibm.mybluemix.net/request?payload=backward&topic=");
const req_turnLeft = new Request("https://vancouverrobotibm.mybluemix.net/request?payload=left&topic=");
const req_turnRight = new Request("https://vancouverrobotibm.mybluemix.net/request?payload=right&topic=");
const req_pickUp = new Request("https://vancouverrobotibm.mybluemix.net/request?payload=pick_up&topic=");
const req_dropDown = new Request("https://vancouverrobotibm.mybluemix.net/request?payload=drop_down&topic=");

//Requests with pre-defined payload values for faces
const req_heart = new Request("https://vancouverrobotibm.mybluemix.net/request?payload=heart&topic=");
const req_happy = new Request("https://vancouverrobotibm.mybluemix.net/request?payload=happy&topic=");
const req_sad = new Request("https://vancouverrobotibm.mybluemix.net/request?payload=sad&topic=");
const req_mad = new Request("https://vancouverrobotibm.mybluemix.net/request?payload=mad&topic=");
const req_dead = new Request("https://vancouverrobotibm.mybluemix.net/request?payload=dead&topic=");

function timeout(ms, promise) {
  return new Promise(function(resolve, reject) {
    setTimeout(function() {
      reject(new Error("timeout"))
    }, ms)
    promise.then(resolve, reject)
  });
}

export default class App extends React.Component {
  constructor() {
    super();
    this.state = {showAvailable: false};
    this._onButtonPress = this._onButtonPress.bind(this);
  }
  
  //Control functions
  _onButtonPress(val) {
    timeout(1000, fetch(val))
    .then(response => {
      if (response.status == 200) {
        this.setState({showAvailable: false});
      }
      else {
        Alert.alert('Failed to bring system offline!');
        throw new Error('Something went wrong!');
      }
    }).catch(function(error){
      Alert.alert('Failed to bring system offline!');
      throw new Error('Something went wrong!');
    });
  }

  
  render() {
    //Image Links
    let telus = require('./images/telusLogo.png');
    let robot = require('./images/robot.jpg');
    let upArrow = require('./images/upArrow.png');
    let downArrow = require('./images/downArrow.png');
    let leftArrow = require('./images/leftArrow.png');
    let rightArrow = require('./images/rightArrow.png');
    let grab = require('./images/grab.png');
    let release = require('./images/release.png');

    //Faces Image Links
    let heart = require('./images/heart.png');
    let happy = require('./images/happy.png');
    let sad = require('./images/sad.png');
    let mad = require('./images/mad.png');
    let dead = require('./images/dead.png');
    let rip = require('./images/rip.png');


    return (
      <View style={styles.container}>
        <View style={[styles.boxContainer, styles.boxOne, styles.hLine]}>
          <Image style={styles.telusImage} source={telus} />
        </View> 
        <View style={[styles.boxContainer, styles.robotBox, styles.hLine]}>
          <View style={styles.robotInnerBoxOne}>
            <Text style={[styles.textcolor, styles.textSize]}>MakeBlock Robot App</Text>
          </View>
          <View style={styles.robotInnerBoxTwo}>
            <Image source={robot} style={styles.robotImage} />
          </View>
        </View>
        <View style={[styles.boxContainer, styles.controllerBox, styles.hLine]}>
          <View style={styles.controllerInnerBoxOne}>
            <Text style={[styles.textcolor, styles.controllsTextSize]}>Movement</Text>
            <Text style={[styles.textcolor, styles.controllsTextSize]}>Forward/Backwards</Text>
            <View style={styles.fbMovementButtons}>
            <TouchableOpacity style={styles.button} onPress={this._onButtonPress.bind(this, req_moveForward)}>
              <Image source={upArrow} style={styles.buttonImg} />
            </TouchableOpacity>
            <TouchableOpacity style={styles.button} onPress={this._onButtonPress.bind(this, req_moveBackwards)}>
              <Image source={downArrow} style={styles.buttonImg} />
            </TouchableOpacity>
            </View>
          </View>
          <View style={styles.controllerInnerBoxTwo}>
            <Text style={[styles.textcolor, styles.controllsTextSize]}>Turning</Text>
            <Text style={[styles.textcolor, styles.controllsTextSize]}>Left/Right</Text>
            <View style={styles.lrMovementButtons}>
              <TouchableOpacity style={styles.button} onPress={this._onButtonPress.bind(this, req_turnLeft)}>
                <Image source={leftArrow} style={[styles.buttonImg, styles.fixedMarginLeft]} />
              </TouchableOpacity>
              <TouchableOpacity style={styles.button} onPress={this._onButtonPress.bind(this, req_turnRight)}>
                <Image source={rightArrow} style={[styles.buttonImg, styles.fixedMarginRight]} />
              </TouchableOpacity>
            </View>
          </View>
          <View style={styles.controllerInnerBoxThree}>
            <Text style={[styles.textcolor, styles.controllsTextSize]}>Arm</Text>
            <Text style={[styles.textcolor, styles.controllsTextSize]}>Pick Up/Drop Down</Text>
            <View style={styles.grMovementButtons}>
              <TouchableOpacity style={styles.button} onPress={this._onButtonPress.bind(this, req_pickUp)}>
                <Image source={grab} style={styles.buttonImg} />
              </TouchableOpacity>
              <TouchableOpacity style={styles.button} onPress={this._onButtonPress.bind(this, req_dropDown)}>
                <Image source={release} style={styles.buttonImg} />
              </TouchableOpacity>
            </View>
          </View>
        </View>
        <View style={[styles.boxContainer, styles.facesBox]}>
          <Text style={styles.textcolor}>MakeBlock Robot Faces</Text>
          <Text style={styles.textcolor}>Press the button for a face effect!</Text>
              <View style={[styles.boxContainer, styles.facesInnerBoxOne]}>
                <TouchableOpacity style={styles.button} onPress={this._onButtonPress.bind(this, req_heart)}>
                  <Image source={heart} style={styles.buttonImg} />
                </TouchableOpacity>
                <TouchableOpacity style={styles.button} onPress={this._onButtonPress.bind(this, req_happy)}>
                  <Image source={happy} style={styles.buttonImg} />
                </TouchableOpacity>
                <TouchableOpacity style={styles.button} onPress={this._onButtonPress.bind(this, req_sad)}>
                  <Image source={sad} style={styles.buttonImg} />
                </TouchableOpacity>
              </View>
              <View style={styles.facesInnerBoxTwo}>
                <TouchableOpacity style={styles.button} onPress={this._onButtonPress.bind(this, req_mad)}>
                  <Image source={mad} style={styles.buttonImg} />
                </TouchableOpacity>
                <TouchableOpacity style={styles.button} onPress={this._onButtonPress.bind(this, req_dead)}>
                  <Image source={dead} style={styles.buttonImg} />
                </TouchableOpacity>
              </View>
        </View>
      </View>
    );  
  }
}

const styles = StyleSheet.create({
  
  //Screen Container
  container: {
    flex: 1, //1:1
    backgroundColor: '#fff',
  },

    //Container for the boxes
    boxContainer: {
      flex: 1, //1:4
      alignItems: 'center',
      justifyContent: 'center',

    },

      //Box 1
      boxOne: { //Telus Image container
        flex: 1, //2:10.5
        flexDirection: 'row',
        marginTop: 10,
      },

        telusImage: { //Logo
          width: 75,
          height: 20,
        },

      //Box 2
      robotBox: { //Robot Img container
        flex: 3.5,
        flexDirection: 'row',
      },

        //Box 2 Inner container
        robotInnerBoxOne: {
          flex: 1,
          alignItems: 'center',
        },

        robotInnerBoxTwo: {
          flex: 1,
          marginBottom: 20,
        },

          robotImage: { //Robot Img
            marginLeft: 20,
            marginTop: 20,
            width: 125,
            height: 125,
          },

      //Box 3
      controllerBox: { //Controlls container
        flex: 4.5, //
        flexDirection: 'row',
        alignItems: 'flex-start',
        marginTop: 15,
      },

        //Controller box inner Container
        controllerInnerBoxOne: {
          flex:2,
          alignItems: 'center',
        },

          fbMovementButtons: { //FlexBox for the forward/backwards buttons
            marginTop: 15,
          },

        controllerInnerBoxTwo: { //Controller box inner container #2
          flex: 2,
          alignItems: 'center',
        },

          lrMovementButtons: { //FlexBox for the left/right buttons
            marginTop: 50,
            alignItems: 'center',
            flexDirection: 'row',
          },

            fixedMarginLeft:{ //Margin for the left arrow
              marginLeft:35,
            },

            fixedMarginRight:{ //Margin for the right arrow
              marginRight:35,
            },
          
        controllerInnerBoxThree: { //Controller box inner container #3
          flex:2,
          alignItems: 'center',
        },

          grMovementButtons: {
            marginTop: 15,
          },

      facesBox: { //Faces container
        flex: 4.9,
        marginTop: 15,
        alignItems: 'center',
      },

        facesInnerBoxOne: { //Container for the first row of faces
          flex: 1,
          marginTop: 10,
          alignItems: 'flex-start',
          flexDirection: 'row',
        },

        facesInnerBoxTwo:{ //Container for the second row of faces
          flex: 1,
          flexDirection: 'row',
          alignItems: 'flex-start',
        },

    button: { //Button Styling
      width: 90,
      alignItems: 'center',
      borderRadius: 2,
    },

    buttonImg: { //Button Img styling (size)
      width: 50,
      height: 50,
    },

    textcolor: {
      color: '#481B6B',
    },

    textSize: {
      fontWeight: 'bold',
    },

    controllsTextSize: {
      fontSize: 10,
      fontWeight: 'bold',
    },

    hLine: {
      borderBottomWidth: 0.5,
      borderColor: '#B3B3B3',
    },
});
