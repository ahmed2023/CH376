#include <Ch376msc.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 6);
Ch376msc flashDrive(mySerial);
char adatBuffer[255];
boolean readMore;
char Data[] = "ASH.\n";
bool input = false;
uint8_t count = 0;
char File_Name[] = "TEST8.TXT";
bool Dir = true;
char* myString = "Ash";
void readString() {
  char endMarker = '\n';
  char rc;
  while (Serial.available() > 0 ) {
    rc = Serial.read();
    if (rc != endMarker) {
      myString += rc;
      count++;
    }
    else {
      input = true;
      Serial.println(myString);
    }
  }
}
void USB_Write(){
  if(Dir){
    Serial.println("COMMAND1: Create and write data to file : ");
    flashDrive.setFileName(File_Name);  //set the file name
    Serial.println(File_Name);
    flashDrive.openFile();                //open the file
    for(int a = 0; a < 5; a++){
      flashDrive.writeFile(myString, strlen(myString)); //string, string length
      flashDrive.writeNumln(count);
    }
    flashDrive.closeFile();
    Serial.println("Done!");
    input = false;
    USB_Read();
    Dir = false;
  } else {
    Serial.println("COMMAND2: Append data to file: ");
    flashDrive.setFileName(File_Name);  //set the file name
    Serial.println(File_Name);
    if(flashDrive.openFile() == ANSW_USB_INT_SUCCESS){               //open the file
      flashDrive.moveCursor(CURSOREND);
    }
    for(int a = 0; a < 3; a++){
      if(flashDrive.getFreeSectors()){
        // flashDrive.writeRaw(myString, strlen(myString) );
        flashDrive.writeNumln(count);
      } else {
        Serial.println("Disk full");
      }
    }
    flashDrive.closeFile();
    Serial.println("Done!");
    USB_Read();
    input = false;
  }
} 
void USB_Read(){
  flashDrive.setFileName(File_Name);  //set the file name
  flashDrive.openFile();                //open the file
  readMore = true;
  //read data from flash drive until we reach EOF
  while(readMore){
  	readMore = flashDrive.readFile(adatBuffer, sizeof(adatBuffer));
    Serial.print(adatBuffer);
  }
  flashDrive.closeFile();
}
void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  flashDrive.init();
}
void loop() {
  if(flashDrive.checkIntMessage()){
		if(flashDrive.getDeviceStatus()){
			Serial.println(F("Flash drive attached!"));
		} else {
			Serial.println(F("Flash drive detached!"));
		}
	}
  readString();
  if(input){
    USB_Write();
  }
}