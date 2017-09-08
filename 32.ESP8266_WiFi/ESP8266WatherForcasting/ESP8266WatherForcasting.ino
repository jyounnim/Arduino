/*
http://www.kocoafab.cc/tutorial/view/595
*/

#include "SPI"
#include "WiFi.h"

char ssid[] = "LeTV_Butler";       //�������� SSID
char pass[] = "humax1234";   //�������� password 

//�ν��Ͻ� ���� �ʱ�ȭ
WiFiServer server(80);
WiFiClient client;

IPAddress hostIp;
uint8_t ret;

int temp = 0;

String weather_str="";
String wt_temp="";
String wt_wfKor="";
String wt_wfEn="";
String wt_reh="";

void setup() {
	//�� ������ ������ ���� �Ҵ�
	Serial.begin(115200);    
	
	delay(10);
	
	//WiFi���� �õ�
	Serial.println("Connecting to WiFi....");  
	WiFi.begin(ssid, pass);  //WiFi�� �н����带 ����Ѵٸ� �Ű������� password�� �ۼ�
	
	server.begin();
	Serial.println("Connect success!");
	Serial.println("Waiting for DHCP address");
	//DHCP�ּҸ� ��ٸ���
	while(WiFi.localIP() == INADDR_NONE) {
		Serial.print(".");
		delay(300);
	}
	
	Serial.println("\n");
	printWifiData();
	connectToServer();
}

void loop() {
	if (client.connected()) {
		while (client.available()) {
			//������ �������� ���ڿ��� �����Ѵ�.
			String line = client.readStringUntil('\n');
			Serial.println(line);
			
			//�ð�
			int temp11= line.indexOf("</hour>");
			if(temp11>0) {
				String tmp_str="<hour>";
				String wt_hour = line.substring(line.indexOf(tmp_str)+tmp_str.length(),temp11);
				Serial.print("hour is "); 
				Serial.println(wt_hour);  
			}
			
			//�µ�
			int temp= line.indexOf("</temp>");
			if(temp>0) {
				String tmp_str="<temp>";
				String wt_temp = line.substring(line.indexOf(tmp_str)+tmp_str.length(),temp);
				Serial.print("temperature is "); 
				Serial.println(wt_temp);  
			}
			
			//���� ����
			int wfEn= line.indexOf("</wfEn>");
			if(wfEn>0) {
				String tmp_str="<wfEn>";
				String wt_twfEn = line.substring(line.indexOf(tmp_str)+tmp_str.length(),wfEn);
				Serial.print("weather is ");
				Serial.println(wt_twfEn);  
			}
			
			//����
			int reh= line.indexOf("</reh>");
			if(reh>0) {
				String tmp_str="<reh>";
				String wt_reh = line.substring(line.indexOf(tmp_str)+tmp_str.length(),reh);
				Serial.print("Humidity is ");
				Serial.println(wt_reh);  
			}
		}   
	}
}

//������ ����
void connectToServer() {
	Serial.println("connecting to server...");
	String content = "";
	if (client.connect(hostIp, 80)) {
		Serial.println("Connected! Making HTTP request to www.kma.go.kr");
		//Serial.println("GET /data/2.5/weather?q="+location+"&mode=xml");
		client.println("GET /wid/queryDFSRSS.jsp?zone=4113552000 HTTP/1.1"); //��⵵ ������ �д籸 ����1��
		//���� ������ �ּҿ� �����Ѵ�.
		client.print("HOST: www.kma.go.kr\n");
		client.println("User-Agent: launchpad-wifi");
		client.println("Connection: close");
		
		client.println();
		Serial.println("Weather information for ");
	}
	//���������� ���ῡ ������ �ð��� ���
}


void printHex(int num, int precision) {
	char tmp[16];
	char format[128];
	
	sprintf(format, "%%.%dX", precision);
	
	sprintf(tmp, format, num);
	Serial.print(tmp);
}

void printWifiData() {
	// Wifi������ IP�ּҸ� ���
	Serial.println();
	Serial.println("IP Address Information:");  
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);
	
	//MAC address���
	byte mac[6];  
	WiFi.macAddress(mac);
	Serial.print("MAC address: ");
	printHex(mac[5], 2);
	Serial.print(":");
	printHex(mac[4], 2);
	Serial.print(":");
	printHex(mac[3], 2);
	Serial.print(":");
	printHex(mac[2], 2);
	Serial.print(":");
	printHex(mac[1], 2);
	Serial.print(":");
	printHex(mac[0], 2);
	Serial.println();
	//����� ����ũ ���
	IPAddress subnet = WiFi.subnetMask();
	Serial.print("NetMask: ");
	Serial.println(subnet);
	
	//����Ʈ���� �ּ� ���
	IPAddress gateway = WiFi.gatewayIP();
	Serial.print("Gateway: ");
	Serial.println(gateway);
	
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());
	
	ret = WiFi.hostByName("www.kma.go.kr", hostIp);
	
	Serial.print("ret: ");
	Serial.println(ret);
	
	Serial.print("Host IP: ");
	Serial.println(hostIp);
	Serial.println("");
}

int getInt(String input){
	int i = 2;
	
	while(input[i] != '"'){
		i++;
	}
	input = input.substring(2,i);
	char carray[20];
	//Serial.println(input);
	input.toCharArray(carray, sizeof(carray));
	//Serial.println(carray);
	temp = atoi(carray);
	//Serial.println(temp);
	return temp;
}

