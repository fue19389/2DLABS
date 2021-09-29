/*
  SD card read/write

 */


/********************Librerias*********************/
#include <SPI.h>
#include <SD.h>

File myFile;
/**************************************************/

/**************SETUP*******************************/

void setup(){
  Serial.begin(9600);
  SPI.setModule(0);
  pinMode(PA_3, OUTPUT);


  
  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}


void loop(){
  /*myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }*/
    int OPC = (int)Serial.read();

  if(OPC == 48){  
  // re-open the file for reading:
  myFile = SD.open("3.txt");
  if (myFile) {
    Serial.println("3.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening 3.txt");
  }
  }

    if(OPC == 49){  
  // re-open the file for reading:
  myFile = SD.open("2.txt");
  if (myFile) {
    Serial.println("2.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening 2.txt");
  }
  }
  
}
