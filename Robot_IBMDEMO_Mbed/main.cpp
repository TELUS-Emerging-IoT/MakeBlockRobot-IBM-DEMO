#include "mbed.h"
#include "BG96Interface.h"
#include "TCPSocket.h"
#include "MQTTClient.h"
#include "MQTT_GSM.h"
#include <ctype.h>
//#include "x_nucleo_iks01a1.h"
#include "XNucleoIKS01A2.h"

#include "BG96.h"

//------------------------------------
// Hyperterminal default configuration
// 9600 bauds, 8-bit data, no parity
//------------------------------------
Serial pc(SERIAL_TX, SERIAL_RX, 9600);
Serial megapi(A0, A1, 9600);
DigitalOut myled(LED1);

#define MQTT_MAX_PACKET_SIZE 300
#define MQTT_MAX_PAYLOAD_SIZE 500


#define ORG_QUICKSTART    // comment to connect to play.internetofthings.ibmcloud.com
#define SUBSCRIBE         // uncomment to subscribe to broker msgs (not to be used with IBM broker)

// Configuration values needed to connect to IBM IoT Cloud
#define BROKER_URL ".messaging.internetofthings.ibmcloud.com";
#ifdef ORG_QUICKSTART
#define ORG "9kars7" // connect to quickstart.internetofthings.ibmcloud.com/ For a registered connection, replace with your org
#define ID "VancouverRobotDemoUnit"
#define AUTH_TOKEN "@Y)+94!CPmjobBUWmA"
#define DEFAULT_TYPE_NAME "Telus-IOT-Starter-Kit"
#define PUBTOPIC  "iot-2/evt/status/fmt/json"
#define SUBTOPIC  "iot-2/cmd/+/fmt/String"
#else   // not def ORG_QUICKSTART
#define ORG "pvko17"    // connect to play.internetofthings.ibmcloud.com/ For a registered connection, replace with your org
#define ID "testtype_112233445566"               // For a registered connection, replace with your id
#define AUTH_TOKEN "testtype_112233445566"    // For a registered connection, replace with your auth-token
#define DEFAULT_TYPE_NAME "TestType"
#define TOPIC   "iot-2/type/TestType/id/testtype_112233445566/evt/status/fmt/json"
#endif

// network credential
#define APN   "pp.telus.com"
#define PASSW  ""
#define USNAME ""

#define TYPE DEFAULT_TYPE_NAME       // Device Type defined above
#define MQTT_PORT 1883
#define MQTT_TLS_PORT 8883
#define IBM_IOT_PORT MQTT_PORT


char id[30] = ID;                 // mac without colons
char org[12] = ORG;
int connack_rc = 0; // MQTT connack return code
//const char* ip_addr = "11.12.13.14";
//char* host_addr = "11.12.13.14";
char sensor_id[50];
char type[36] = TYPE;
char auth_token[30] = AUTH_TOKEN; // Auth_token is only used in non-quickstart mode
bool netConnecting = false;
int connectTimeout = 1000;
bool mqttConnecting = false;
bool netConnected = false;
bool connected = false;
int retryAttempt = 0;
char subscription_url[MQTT_MAX_PAYLOAD_SIZE];
char subbed_msg[MQTT_MAX_PAYLOAD_SIZE];
#define SENSOR_ENABLED        1
#define SENSOR_MODEL        2

#define FW_REV                "1.0a"

MQTT::Message message;
MQTTString TopicName={PUBTOPIC};
MQTT::MessageData MsgData(TopicName, message);

/* Instantiate the expansion board */
static XNucleoIKS01A2 *mems_expansion_board = XNucleoIKS01A2::instance(D14, D15, D4, D5);

/* Retrieve the composing elements of the expansion board */
static LSM303AGRMagSensor *magnetometer = mems_expansion_board->magnetometer;
static HTS221Sensor *hum_temp = mems_expansion_board->ht_sensor;
static LPS22HBSensor *press_temp = mems_expansion_board->pt_sensor;
static LSM6DSLSensor *acc_gyro = mems_expansion_board->acc_gyro;
static LSM303AGRAccSensor *accelerometer = mems_expansion_board->accelerometer;

void subscribe_cb(MQTT::MessageData & msgMQTT) {
    char msg[MQTT_MAX_PAYLOAD_SIZE];
    msg[0]='\0';
    subbed_msg[0] = msg[0];
    strncat(msg, (char*)msgMQTT.message.payload, msgMQTT.message.payloadlen);
    //pc.printf("%s \r\n", msg);
    sprintf(subbed_msg, "%s", msg);
    
    if (strcmp(subbed_msg, "forward") == 0)
    {
        megapi.putc('x');
        pc.printf("forward \r\n");
    }
    else if (strcmp(subbed_msg, "backward") == 0)
    {
        megapi.putc('y');
        pc.printf("backward \r\n");
    }
    else if (strcmp(subbed_msg, "left") == 0)
    {
        megapi.putc('l');
        pc.printf("left \r\n");
    }
    else if (strcmp(subbed_msg, "right") == 0)
    {
        megapi.putc('r');
        pc.printf("right \r\n");
    }
    else if (strcmp(subbed_msg, "pick_up") == 0)
    {
        megapi.putc('t');
        pc.printf("pick up \r\n");
    }
    else if (strcmp(subbed_msg, "drop_down") == 0)
    {
        megapi.putc('d');
        pc.printf("drop down \r\n");
    }
    //Faces
    else if (strcmp(subbed_msg, "heart") == 0)
    {
        megapi.putc('6');
        pc.printf("display heart \r\n");
    }
    else if (strcmp(subbed_msg, "happy") == 0)
    {
        megapi.putc('2');
        pc.printf("display happy \r\n");
    }
    else if (strcmp(subbed_msg, "sad") == 0)
    {
        megapi.putc('4');
        pc.printf("display sad \r\n");
    }
    else if (strcmp(subbed_msg, "mad") == 0)
    {
        megapi.putc('3');
        pc.printf("display mad \r\n");
    }
    else if (strcmp(subbed_msg, "dead") == 0)
    {
        megapi.putc('5');
        pc.printf("display dead");
    }
}

int subscribe(MQTT::Client<MQTT_GSM, Countdown, MQTT_MAX_PACKET_SIZE>* client, MQTT_GSM* ipstack)
{
    char* subTopic = SUBTOPIC;
    return client->subscribe(subTopic, MQTT::QOS0, subscribe_cb);
}

int connect(MQTT::Client<MQTT_GSM, Countdown, MQTT_MAX_PACKET_SIZE>* client, MQTT_GSM* ipstack)
{
    const char* iot_ibm = BROKER_URL;
    
    
    char hostname[strlen(org) + strlen(iot_ibm) + 1];
    sprintf(hostname, "%s%s", org, iot_ibm);
    
    
    // Construct clientId - d:org:type:id
    char clientId[strlen(org) + strlen(type) + strlen(id) + 5];
    
#ifdef ORG_QUICKSTART
    sprintf(clientId, "d:%s:%s:%s", org, type, id);  //@@
#else
    sprintf(clientId, "g:%s:%s:%s", org, type, id);  //@@
#endif
    
    sprintf(subscription_url, "%s.%s/#/device/%s/sensor/", org, "internetofthings.ibmcloud.com",id);
    
    netConnecting = true;
    ipstack->open(&ipstack->getGSM());
    int rc = ipstack->connect(hostname, IBM_IOT_PORT, connectTimeout);
    if (rc != 0)
    {
        //WARN("IP Stack connect returned: %d\n", rc);
        return rc;
    }
    pc.printf ("--->TCP Connected\n\r");
    netConnected = true;
    netConnecting = false;
    
    // MQTT Connect
    mqttConnecting = true;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3.1;
    data.struct_version=0;
    data.clientID.cstring = clientId;
    
    data.username.cstring = "use-token-auth";
    data.password.cstring = auth_token;
    
    if ((rc = client->connect(data)) == 0)
    {
        connected = true;
        pc.printf ("--->MQTT Connected\n\r");
        
        if (!subscribe(client, ipstack)) printf ("--->>>MQTT subscribed to: %s\n",SUBTOPIC);
        
    }
    else {
        //WARN("MQTT connect returned %d\n", rc);
    }
    if (rc >= 0)
        connack_rc = rc;
    mqttConnecting = false;
    return rc;
}

int getConnTimeout(int attemptNumber)
{      // First 10 attempts try within 3 seconds, next 10 attempts retry after every 1 minute
    // after 20 attempts, retry every 10 minutes
    return (attemptNumber < 10) ? 3 : (attemptNumber < 20) ? 60 : 600;
}

void attemptConnect(MQTT::Client<MQTT_GSM, Countdown, MQTT_MAX_PACKET_SIZE>* client, MQTT_GSM* ipstack)
{
    connected = false;
    
    while (connect(client, ipstack) != MQTT_CONNECTION_ACCEPTED)
    {
        if (connack_rc == MQTT_NOT_AUTHORIZED || connack_rc == MQTT_BAD_USERNAME_OR_PASSWORD) {
            printf ("File: %s, Line: %d Error: %d\n\r",__FILE__,__LINE__, connack_rc);
            return; // don't reattempt to connect if credentials are wrong
        }
        int timeout = getConnTimeout(++retryAttempt);
        //WARN("Retry attempt number %d waiting %d\n", retryAttempt, timeout);
        
        // if ipstack and client were on the heap we could deconstruct and goto a label where they are constructed
        //  or maybe just add the proper members to do this disconnect and call attemptConnect(...)
        // this works - reset the system when the retry count gets to a threshold
        if (retryAttempt == 5){
            pc.printf ("\n\n\rFAIL!! system reset!!\n\n\r");
            NVIC_SystemReset();
        }
        else wait(timeout);
    }
}
float hum_global = 50.0;
uint32_t n_msg = 0;

int publish(MQTT::Client<MQTT_GSM, Countdown, MQTT_MAX_PACKET_SIZE>* client, MQTT_GSM* ipstack)
{
    MQTT::Message message;
    char* pubTopic = PUBTOPIC;
    
    char buf[MQTT_MAX_PAYLOAD_SIZE];
    float temp, temp1, temp2, press, hum;
    int acc[3], gyro[3];
    
#if SENSOR_ENABLED
    pc.printf("A02 reading sensors...");
    
    hum_temp->get_temperature(&temp1);
    hum_temp->get_humidity(&hum);
    press_temp->get_temperature(&temp2);
    press_temp->get_pressure(&press);
    acc_gyro->get_x_axes(acc);
    acc_gyro->get_g_axes(gyro);
    
    temp = (temp1+temp2)/2;
    
    pc.printf(" DONE\r\n");
#else
    temp=25.5;
    hum_global +=0.1;
    if (hum_global>99.0)
        hum_global = 50.0;
    hum=hum_global;
    press=999;
    acc[0] = 0;
    acc[1] = 0;
    acc[2] = 0;
    gyro[0] = 0;
    gyro[1] = 0;
    gyro[2] = 0;
#endif
    
    sprintf(buf, "{\"d\":{\"ST\":\"Nucleo-IoT-mbed\",\"Temp\":%0.4f,\"Pressure\":%0.4f,\"Humidity\":%0.4f,\"Acc_X\":%d,\"Acc_Y\":%d,\"Acc_Z\":%d,\"Gyro_X\":%d,\"Gyro_Y\":%d,\"Gyro_Z\":%d}}", temp, press, hum, acc[0], acc[1], acc[2], gyro[0], gyro[1], gyro[2]);
    
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadlen = strlen(buf);
    
    //LOG("Publishing %s\n\r", buf);
    n_msg++;
    pc.printf("Publishing V%s #%d %s\n\r", FW_REV, n_msg, buf);
    return client->publish(pubTopic, message);
}


int loop_count = 0;
//int loop_count_2 = 0;

int main()
{
    const char * apn = APN; // Network must be visible otherwise it can't connect
    const char * username = USNAME;
    const char * password = PASSW;
    BG96Interface bg96_if(D8, D2, false);
    sprintf(sensor_id,"%s",bg96_if.get_mac_address());
    wait(0.1);
    
    pc.printf("\r\n*************************************************");
    wait( 0.1 );
    pc.printf("TELUS LTE-M IoT Starter Kit: Robot Demo");
    wait( 0.1 );
    pc.printf("MBED online version %s\r\n", FW_REV);
    wait( 0.1 );
    pc.printf("APN  = %s\r\n", APN);
    
#if SENSOR_ENABLED
    /* Enable all sensors */
    hum_temp->enable();
    press_temp->enable();
    magnetometer->enable();
    accelerometer->enable();
    acc_gyro->enable_x();
    acc_gyro->enable_g();
#endif
    
    MQTT_GSM ipstack(bg96_if, apn, username, password);
    MQTT::Client<MQTT_GSM, Countdown, MQTT_MAX_PACKET_SIZE> client(ipstack);
    
    attemptConnect(&client, &ipstack);
    
    if (connack_rc == MQTT_NOT_AUTHORIZED || connack_rc == MQTT_BAD_USERNAME_OR_PASSWORD)
    {
        while (true)
            wait(1.0); // Permanent failures - don't retry
    }
    
    sprintf(sensor_id,"%s",bg96_if.get_mac_address());
    megapi.putc('0');
    pc.printf("Ready!");
    megapi.putc('i');
    while (true)
    {
        if (++loop_count == 10)
        {
            // Publish a message every 5 second
            pc.printf("\n");
            
            if (publish(&client, &ipstack) != 0) {
                myled=0;
                attemptConnect(&client, &ipstack);   // if we have lost the connection
            }
            loop_count = 0;
        }
        
        client.yield(500);  // allow the MQTT client to receive messages
        pc.printf ("loop %d\r", (loop_count+1));
    }
}
