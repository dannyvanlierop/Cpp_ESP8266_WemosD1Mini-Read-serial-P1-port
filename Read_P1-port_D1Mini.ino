//Modules:
#include "Arduino.h"
#include <SoftwareSerial.h> //SoftwareSerial

//HardWare:
#define rxPin D1
#define txPin D2
SoftwareSerial mySerial(D1, D2, false); // RX, TX, inverted

//CONST!!: for Quality check
#define BUFFERLENGTH 609 // length of serialTelegramBuffer
#define DIGITS 366  //Digits in serialTelegramBuffer
#define ALPHAS 36   //Alphas in serialTelegramBuffer
#define PRINTABLE 582   //Printable in serialTelegramBuffer
#define LINES 26
#define ITEMS 31

class itemClass
{
  public:
    String itemValueType;
    String itemName;
    String sItemValue;
    int iItemValue;
    float fItemValue;
    String itemState;
    String itemUnit;

    int lineLength;
    int itemNameStartPos;
    int itemNameStopPos;
    int itemValueStartPos;
    int itemValueStopPos;
    String itemLastUpdate;
};

itemClass item0, item1, item2, item3, item4, item5, item6, item7, item8, item9, item10,
          item11, item12, item13, item14, item15, item16, item17, item18, item19, item20,
          item21, item22, item23, item24, item25, item26, item27, item28, item29, item30;

itemClass *item[ITEMS] = {&item0, &item1, &item2, &item3, &item4, &item5, &item6, &item7, &item8, &item9, &item10,
                          &item11, &item12, &item13, &item14, &item15, &item16, &item17, &item18, &item19, &item20,
                          &item21, &item22, &item23, &item24, &item25, &item26, &item27, &item28, &item29, &item30
                         };
//General
int counter = 0;
int countOk = 0;
int countFail = 0;

//Telegram
bool gotUpdate = false;
String serialTelegramBuffer;//Contains a checked "kinda static" telegram

void InitItemClass()
{
  // put your setup code here, to run once:
  item[0] = new itemClass();
  item[0]->itemValueType = "string";
  item[0]->itemName = ""; //CRC
  item[0]->sItemValue = "E9B";
  item[0]->itemNameStartPos = 0;
  item[0]->itemNameStopPos = item[0]->itemNameStartPos + item[0]->itemName.length();
  item[0]->itemValueStartPos = 0;
  item[0]->itemValueStopPos = item[0]->itemValueStartPos + item[0]->sItemValue.length();
  item[0]->itemLastUpdate = "☐";

  item[1] = new itemClass();
  item[1]->itemValueType = "string";
  item[1]->itemName = ""; //Type
  item[1]->sItemValue = "/Ene5\XS210 ESMR 5.0";
  item[1]->itemNameStartPos = 0;
  item[1]->itemNameStopPos = item[1]->itemNameStartPos + item[1]->itemName.length() + 1;
  item[1]->itemValueStartPos = 0;
  item[1]->itemValueStopPos = item[1]->itemValueStartPos + item[1]->sItemValue.length() - 1;
  item[1]->itemLastUpdate = "☐";

  item[2] = new itemClass();
  item[2]->itemValueType = "int";
  item[2]->itemName = "1-3:0.2.8";
  item[2]->iItemValue = 50;
  item[2]->itemNameStartPos = 0;
  item[2]->itemNameStopPos = item[2]->itemNameStartPos + item[2]->itemName.length() - 1;
  item[2]->itemValueStartPos = item[2]->itemNameStopPos + 2;
  item[2]->itemValueStopPos =  item[2]->itemValueStartPos + String(item[2]->iItemValue).length() - 1;
  item[2]->itemLastUpdate = "☐";

  item[3] = new itemClass();
  item[3]->itemValueType = "digits";//digits ==
  item[3]->itemName = "0-0:1.0.0";
  item[3]->sItemValue = "181210162308";
  item[3]->itemNameStartPos = 0;
  item[3]->itemNameStopPos = item[3]->itemNameStartPos + item[3]->itemName.length() - 1;
  item[3]->itemValueStartPos = item[3]->itemNameStopPos + 2;
  item[3]->itemValueStopPos = item[3]->itemValueStartPos + String(item[3]->sItemValue).length() - 1;
  item[3]->itemLastUpdate = "☐";

  item[4] = new itemClass();
  item[4]->itemValueType = "string";
  item[4]->itemName = "0-0:1.0.0";
  item[4]->sItemValue = "W"; //DTS Active
  item[4]->itemNameStartPos = 0;
  item[4]->itemNameStopPos = item[4]->itemNameStartPos + item[4]->itemName.length() - 1;
  item[4]->itemValueStartPos = item[3]->itemValueStopPos + 1;
  item[4]->itemValueStopPos = item[4]->itemValueStartPos + item[4]->sItemValue.length() - 1;
  item[4]->itemLastUpdate = "☐";

  item[5] = new itemClass();
  item[5]->itemValueType = "digits";
  item[5]->itemName = "0-0:96.1.1";
  item[5]->sItemValue = "1234567890123456789012345678901234";
  item[5]->itemNameStartPos = 0;
  item[5]->itemNameStopPos = item[5]->itemNameStartPos + item[5]->itemName.length() - 1;
  item[5]->itemValueStartPos = item[5]->itemNameStopPos + 2;
  item[5]->itemValueStopPos = item[5]->itemValueStartPos + item[5]->sItemValue.length() - 1;
  item[5]->itemLastUpdate = "☐";

  item[6] = new itemClass();
  item[6]->itemValueType = "float";
  item[6]->itemName = "1-0:1.8.1";
  item[6]->fItemValue = 000000.000;
  item[6]->itemUnit = "kWh";
  item[6]->itemNameStartPos = 0;
  item[6]->itemNameStopPos = item[6]->itemNameStartPos + item[6]->itemName.length() - 1;
  item[6]->itemValueStartPos = item[6]->itemNameStopPos + 2;
  item[6]->itemValueStopPos = item[6]->itemValueStartPos + 9;
  item[6]->itemLastUpdate = "☐";

  item[7] = new itemClass();
  item[7]->itemValueType = "float";
  item[7]->itemName = "1-0:1.8.2";
  item[7]->fItemValue = 000485.710;
  item[7]->itemUnit = "kWh";
  item[7]->itemNameStartPos = 0;
  item[7]->itemNameStopPos = item[7]->itemNameStartPos + item[7]->itemName.length() - 1;
  item[7]->itemValueStartPos = item[7]->itemNameStopPos + 2;
  item[7]->itemValueStopPos = item[7]->itemValueStartPos  + 9;
  item[7]->itemLastUpdate = "☐";


  item[8] = new itemClass();
  item[8]->itemValueType = "float";
  item[8]->itemName = "1-0:2.8.1";
  item[8]->fItemValue = 000585.788;
  item[8]->itemUnit = "kWh";
  item[8]->itemNameStartPos = 0;
  item[8]->itemNameStopPos = item[8]->itemNameStartPos + item[8]->itemName.length() - 1;
  item[8]->itemValueStartPos = item[8]->itemNameStopPos + 2;
  item[8]->itemValueStopPos = item[8]->itemValueStartPos  + 9;
  item[8]->itemLastUpdate = "☐";


  item[9] = new itemClass();
  item[9]->itemValueType = "float";
  item[9]->itemName = "1-0:2.8.2";
  item[9]->fItemValue = 000485.710;
  item[9]->itemUnit = "kWh";
  item[9]->itemNameStartPos = 0;
  item[9]->itemNameStopPos = item[9]->itemNameStartPos + item[9]->itemName.length() - 1;
  item[9]->itemValueStartPos = item[9]->itemNameStopPos + 2;
  item[9]->itemValueStopPos = item[9]->itemValueStartPos  + 9;
  item[9]->itemLastUpdate = "☐";


  item[10] = new itemClass();
  item[10]->itemValueType = "digits";
  item[10]->itemName = "0-0:96.14.0";
  item[10]->sItemValue = "0000";
  item[10]->itemNameStartPos = 0;
  item[10]->itemNameStopPos = item[10]->itemNameStartPos + item[10]->itemName.length() - 1;
  item[10]->itemValueStartPos = item[10]->itemNameStopPos + 2;
  item[10]->itemValueStopPos = item[10]->itemValueStartPos + item[10]->sItemValue.length() - 1;
  item[10]->itemLastUpdate = "☐";


  item[11] = new itemClass();
  item[11]->itemValueType = "float";
  item[11]->itemName = "1-0:1.7.0";
  item[11]->fItemValue = 00.000;
  item[11]->itemUnit = "kW";
  item[11]->itemNameStartPos = 0;
  item[11]->itemNameStopPos = item[11]->itemNameStartPos + item[11]->itemName.length() - 1;
  item[11]->itemValueStartPos = item[11]->itemNameStopPos + 2;
  item[11]->itemValueStopPos = item[11]->itemValueStartPos  + 5;
  item[11]->itemLastUpdate = "☐";


  item[12] = new itemClass();
  item[12]->itemValueType = "float";
  item[12]->itemName = "1-0:2.7.0";
  item[12]->fItemValue = 00.000;
  item[12]->itemUnit = "kW";
  item[12]->itemNameStartPos = 0;
  item[12]->itemNameStopPos = item[12]->itemNameStartPos + item[12]->itemName.length() - 1;
  item[12]->itemValueStartPos = item[12]->itemNameStopPos + 2;
  item[12]->itemValueStopPos = item[12]->itemValueStartPos  + 5;
  item[12]->itemLastUpdate = "☐";


  item[13] = new itemClass();
  item[13]->itemValueType = "digits";
  item[13]->itemName = "0-0:96.7.21";
  item[13]->sItemValue = "00026";
  item[13]->itemNameStartPos = 0;
  item[13]->itemNameStopPos = item[13]->itemNameStartPos + item[13]->itemName.length() - 1;
  item[13]->itemValueStartPos = item[13]->itemNameStopPos + 2;
  item[13]->itemValueStopPos = item[13]->itemValueStartPos + item[13]->sItemValue.length() - 1;
  item[13]->itemLastUpdate = "☐";


  item[14] = new itemClass();
  item[14]->itemValueType = "digits";
  item[14]->itemName = "0-0:96.7.9";
  item[14]->sItemValue = "00001";
  item[14]->itemNameStartPos = 0;
  item[14]->itemNameStopPos = item[14]->itemNameStartPos + item[14]->itemName.length() - 1;
  item[14]->itemValueStartPos = item[14]->itemNameStopPos + 2;
  item[14]->itemValueStopPos = item[14]->itemValueStartPos + item[14]->sItemValue.length() - 1;
  item[14]->itemLastUpdate = "☐";


  item[15] = new itemClass();
  item[15]->itemValueType = "int";
  item[15]->itemName = "1-0:99.97.0";
  item[15]->iItemValue = 1;
  item[15]->itemNameStartPos = 0;
  item[15]->itemNameStopPos = item[15]->itemNameStartPos + String(item[15]->itemName).length() - 1;
  item[15]->itemValueStartPos = item[15]->itemNameStopPos + 2;
  item[15]->itemValueStopPos = item[15]->itemValueStartPos + String(item[15]->iItemValue).length() - 1;
  item[15]->itemLastUpdate = "☐";


  item[16] = new itemClass();
  item[16]->itemValueType = "digits";
  item[16]->itemName = "0-0:96.7.19";
  item[16]->sItemValue = "180716084233";
  item[16]->itemNameStartPos = 16;
  item[16]->itemNameStopPos = item[16]->itemNameStartPos + item[16]->itemName.length() - 1;
  item[16]->itemValueStartPos = item[16]->itemNameStopPos + 2;
  item[16]->itemValueStopPos = item[16]->itemValueStartPos + item[16]->sItemValue.length() - 1;
  item[16]->itemLastUpdate = "☐";


  item[17] = new itemClass();
  item[17]->itemValueType = "string";
  item[17]->itemName = "0-0:96.7.19";
  item[17]->sItemValue = "S";
  item[17]->itemNameStartPos = item[16]->itemNameStartPos;//Belongs to ID 16
  item[17]->itemNameStopPos = item[16]->itemNameStopPos;//Belongs to ID 16
  item[17]->itemValueStartPos = item[16]->itemValueStopPos + 1;
  item[17]->itemValueStopPos = item[17]->itemValueStartPos + item[17]->sItemValue.length() - 1;
  item[17]->itemLastUpdate = "☐";


  item[18] = new itemClass();
  item[18]->itemValueType = "digits";
  item[18]->itemName = "0-0:96.7.19";
  item[18]->sItemValue = "0000000222";
  item[18]->itemUnit = "s";
  item[18]->itemNameStartPos = item[16]->itemNameStopPos;//Belongs to ID 16
  item[18]->itemNameStopPos = item[16]->itemNameStopPos;//Belongs to ID 16
  item[18]->itemValueStartPos = item[17]->itemValueStopPos + 3;
  item[18]->itemValueStopPos = item[18]->itemValueStartPos + item[18]->sItemValue.length() - 1;
  item[18]->itemLastUpdate = "☐";


  item[19] = new itemClass();
  item[19]->itemValueType = "digits";
  item[19]->itemName = "1-0:32.32.0";
  item[19]->sItemValue = "00000";
  item[19]->itemNameStartPos = 0;
  item[19]->itemNameStopPos = item[19]->itemNameStartPos + item[19]->itemName.length() - 1;
  item[19]->itemValueStartPos = item[19]->itemNameStopPos + 2;
  item[19]->itemValueStopPos = item[19]->itemValueStartPos + item[19]->sItemValue.length() - 1;
  item[19]->itemLastUpdate = "☐";;


  item[20] = new itemClass();
  item[20]->itemValueType = "digits";
  item[20]->itemName = "1-0:32.36.0";
  item[20]->sItemValue = "00000";
  item[20]->itemNameStartPos = 0;
  item[20]->itemNameStopPos = item[20]->itemNameStartPos + item[20]->itemName.length() - 1;
  item[20]->itemValueStartPos = item[20]->itemNameStopPos + 2;
  item[20]->itemValueStopPos = item[20]->itemValueStartPos + item[20]->sItemValue.length() - 1;
  item[20]->itemLastUpdate = "☐";


  item[21] = new itemClass();
  item[21]->itemValueType = "int";//will mostly be a none
  item[21]->itemName = "0-0:96.13.0";
  item[21]->iItemValue = 0;
  item[21]->itemNameStartPos = 0;
  item[21]->itemNameStopPos = item[21]->itemNameStartPos + item[21]->itemName.length() - 1;
  item[21]->itemValueStartPos = item[21]->itemNameStopPos + 2;
  item[21]->itemValueStopPos = item[21]->itemValueStartPos + String(item[21]->iItemValue).length() - 1;
  item[21]->itemLastUpdate = "☐";


  item[22] = new itemClass();
  item[22]->itemValueType = "float";
  item[22]->itemName = "1-0:32.7.0";
  item[22]->fItemValue = 229.0;
  item[22]->itemUnit = "V";
  item[22]->itemNameStartPos = 0;
  item[22]->itemNameStopPos = item[22]->itemNameStartPos + item[22]->itemName.length() - 1;
  item[22]->itemValueStartPos = item[22]->itemNameStopPos + 2;
  item[22]->itemValueStopPos = item[22]->itemValueStartPos  + 4;
  item[22]->itemLastUpdate = "☐";


  item[23] = new itemClass();
  item[23]->itemValueType = "digits";
  item[23]->itemName = "1-0:31.7.0";
  item[23]->sItemValue = "003";
  item[23]->itemUnit = "A";
  item[23]->itemNameStartPos = 0;
  item[23]->itemNameStopPos = item[23]->itemNameStartPos + item[23]->itemName.length() - 1;
  item[23]->itemValueStartPos = item[23]->itemNameStopPos + 2;
  item[23]->itemValueStopPos = item[23]->itemValueStartPos + item[23]->sItemValue.length() - 1;
  item[23]->itemLastUpdate = "☐";


  item[24] = new itemClass();
  item[24]->itemValueType = "float";
  item[24]->itemName = "1-0:21.7.0";
  item[24]->fItemValue = 00.665;
  item[24]->itemUnit = "kW";
  item[24]->itemNameStartPos = 0;
  item[24]->itemNameStopPos = item[24]->itemNameStartPos + item[24]->itemName.length() - 1;
  item[24]->itemValueStartPos = item[24]->itemNameStopPos + 2;
  item[24]->itemValueStopPos = item[24]->itemValueStartPos  + 5;
  item[24]->itemLastUpdate = "☐";


  item[25] = new itemClass();
  item[25]->itemValueType = "float";
  item[25]->itemName = "1-0:22.7.0";
  item[25]->fItemValue = 00.000;
  item[25]->itemUnit = "kW";
  item[25]->itemNameStartPos = 0;
  item[25]->itemNameStopPos = item[25]->itemNameStartPos + item[25]->itemName.length() - 1;
  item[25]->itemValueStartPos = item[25]->itemNameStopPos + 2;
  item[25]->itemValueStopPos = item[25]->itemValueStartPos  + 5;
  item[25]->itemLastUpdate = "☐";


  item[26] = new itemClass();
  item[26]->itemValueType = "digits";
  item[26]->itemName = "0-1:24.1.0";
  item[26]->sItemValue = "003";
  item[26]->itemNameStartPos = 0;
  item[26]->itemNameStopPos = item[26]->itemNameStartPos + item[26]->itemName.length() - 1;
  item[26]->itemValueStartPos = item[26]->itemNameStopPos + 2;
  item[26]->itemValueStopPos = item[26]->itemValueStartPos + item[26]->sItemValue.length() - 1;
  item[26]->itemLastUpdate = "☐";

  item[27] = new itemClass();
  item[27]->itemValueType = "digits";
  item[27]->itemName = "0-1:96.1.0";
  item[27]->sItemValue = "1234567890123456789012345678901234";
  item[27]->itemNameStartPos = 0;
  item[27]->itemNameStopPos = item[27]->itemNameStartPos + item[27]->itemName.length() - 1;
  item[27]->itemValueStartPos = item[27]->itemNameStopPos + 2;
  item[27]->itemValueStopPos = item[27]->itemValueStartPos + item[27]->sItemValue.length() - 1;
  item[27]->itemLastUpdate = "☐";


  item[28] = new itemClass();
  item[28]->itemValueType = "digits";
  item[28]->itemName = "0-1:24.2.1";
  item[28]->sItemValue = "181210163000";
  item[28]->itemNameStartPos = 0;
  item[28]->itemNameStopPos = item[28]->itemNameStartPos + item[28]->itemName.length() - 1;
  item[28]->itemValueStartPos = item[28]->itemNameStopPos + 2;
  item[28]->itemValueStopPos = item[28]->itemValueStartPos + item[28]->sItemValue.length() - 1;
  item[28]->itemLastUpdate = "☐";


  item[29] = new itemClass();
  item[29]->itemValueType = "string";//Belongs to ID 28
  item[29]->itemName = "0-1:24.2.1";
  item[29]->sItemValue = "W";
  item[29]->itemNameStartPos = 0;
  item[29]->itemNameStopPos = item[29]->itemNameStartPos + item[29]->itemName.length() - 1;
  item[29]->itemValueStartPos = item[28]->itemValueStopPos + 1;
  item[29]->itemValueStopPos = item[29]->itemValueStartPos;
  item[29]->itemLastUpdate = "☐";


  item[30] = new itemClass();
  item[30]->itemValueType = "float";
  item[30]->itemName = "0-1:24.2.1";
  item[30]->fItemValue = 00005.070;
  item[30]->itemUnit = "m3";
  item[30]->itemNameStartPos = 0;
  item[30]->itemNameStopPos = item[30]->itemNameStartPos + item[30]->itemName.length() - 1;
  item[30]->itemValueStartPos = item[29]->itemValueStopPos + 3;
  item[30]->itemValueStopPos = item[30]->itemValueStartPos + 8;
  item[30]->itemLastUpdate = "☐";

};

int CountDigits(String line)
{

    int intS = 0;

    for (int charPosition = 0; charPosition < line.length(); charPosition++)
    {
        if (isDigit(line[charPosition]))
        {
            intS++;
        };
    };
    return intS;
};

bool FoundOnlyFloat(String line)
{

    int iDots = 0;
    int iDigits = CountDigits(line);

    for (int charPosition = line.length(); charPosition > 0; charPosition--)
    {
        if (line[charPosition] == char('.'))
        {
            iDots++;
        };
    };

    if (iDots == 1 && iDigits == line.length() - 1)
    {
        return true;
    }
    else
    {
        return false;
        Serial.print(" ✗ FoundOnlyFloat");
    }
};

bool FoundOnlyAlphas(String line) //Letters
{

    int intS = 0;

    for (int charPosition = 0; charPosition < line.length(); charPosition++)
    {
        if (isAlpha(line[charPosition]))
        {
            intS++;
        };
    };

    if (intS == line.length())
    {
        return true;
    }
    else
    {
        return false;
        Serial.print(" ✗ FoundOnlyAlphas");
    };
};

bool FoundOnlyDigits(String line)
{
    int intS = 0;

    for (int charPosition = 0; charPosition < line.length(); charPosition++)
    {
        if (isDigit(line[charPosition]))
        {
            intS++;
        };
    };

    if (intS == line.length())
    {
        return true;
    }else{
        return false;
        Serial.print(" ✗ FoundOnlyDigits");
    }
    
};

String ReturnValueFromString(String lineContent, int startPos, int stopPos)
{
    String returnString = "";

    if (startPos == stopPos)//If 1 character
    {
        returnString = lineContent[startPos]; //1 character to returnString
    }
    else
    {
        for (int pos = startPos; pos <= stopPos; pos++)
        {
            returnString += String(lineContent[pos]); //sum of character to returnString
        };
    };

    return returnString;
};

void TelegramProcessItem(String lineContent, int classItemFound, int lineNumber)
{
    //Skip the first 3 items(0,1,2)
    if (classItemFound < 3)
    {
        return;
    }

    String str = "";
    if (classItemFound % 4 == 0)
    {
        str = "\n\t";
    } else {
        str = "\t\t\t";
    }

    Serial.print( str + " -> {}ProcessItem:  " + String(classItemFound) +  " Result:" + item[classItemFound]->itemLastUpdate + " Content: " + lineContent);

    //fetch Value
    String foundValue = ReturnValueFromString(lineContent, item[classItemFound]->itemValueStartPos, item[classItemFound]->itemValueStopPos);

    if (foundValue == ""){
        Serial.print(" ✗ foundValue empty ");
        return;
    }

    if (item[classItemFound]->itemValueType == "int")
    {

        if (FoundOnlyDigits(foundValue)) //✓
        {
            item[classItemFound]->iItemValue = foundValue.toInt();
            item[classItemFound]->itemLastUpdate = "☒";
        }
        else
        {
            Serial.print(" ✗ FoundOnlyDigits");
        }
    }
    else if (item[classItemFound]->itemValueType == "digits")
    {
        if (FoundOnlyDigits(foundValue))
        {
            item[classItemFound]->sItemValue = foundValue;
            item[classItemFound]->itemLastUpdate = "☒";
        }
        else
        {
            Serial.print(" ✗ FoundOnlyDigits");
        }
    }
    else if (item[classItemFound]->itemValueType == "string")
    {
        if (FoundOnlyAlphas(foundValue))
        {
            item[classItemFound]->sItemValue = foundValue;
            item[classItemFound]->itemLastUpdate = "☒";
        }
        else
        {
            Serial.print(" ✗ FoundOnlyAlphas");
        }
    }
    else if (item[classItemFound]->itemValueType == "float")
    {
        if (FoundOnlyFloat(foundValue))
        {
            item[classItemFound]->fItemValue = foundValue.toFloat();
            item[classItemFound]->itemLastUpdate = "☒";
        }
        else
        {
            Serial.print(" ✗ FoundOnlyFloat");
        }
    };
    
    if (classItemFound == 30)
    {
       Serial.println();
    }
};

void TelegramProcessLine(String lineContent, int lineNumber) //input is one random line of the telegram
{
    //Search for itemName in line, on the left side of the bracket
    String ItemName = "";
    for (int lineChar = lineContent.indexOf('(') - 1; lineChar >= 0; lineChar--)
    {
        ItemName = lineContent[lineChar] + ItemName;
    };

    //Compare itemName to the itemNames in class itemClass, add the first in nd collect needed main item in classItems
    int classItemFound[4] = {0, 0, 0, 0};

    int doCount = 0;
    for (int i = 0; i < ITEMS; i++)
    {
        if (ItemName == item[i]->itemName)
        {
            classItemFound[doCount++] = i;
        };
    };

    //Search for more items when we have item 15
    if (ItemName == item[15]->itemName)
    {
        for (int i = 0; i < ITEMS; i++)
        {
            if (item[i]->itemName == item[16]->itemName)
            {
                classItemFound[doCount++] = i;
            };
        };
    }

    for (int i = 0; i < 4; i++)
    {
        if (classItemFound[i] != 0)
        {
            TelegramProcessItem(lineContent, classItemFound[i], lineNumber);
        }
    }
};

void TelegramTooLines(String lineContent)
{

    String line = "";
    int lineNumber = 0;

    for (int i = 0; i < lineContent.length() + 1; i++)
    {

        if (char(lineContent[i]) != '\n')
        {
            line += lineContent[i];
        }
        else
        {
            TelegramProcessLine(line, lineNumber);
            line = "";
            lineNumber++;
        };
    };
};


String serialTelegramLine = "";//Contains the telegramline while read/write1
String serialTelegramString = "";//Contains the telegram while read/write

void readSerial()
{

    int linePosition = -1;

    while (mySerial.available() && linePosition < LINES)
    {
        char incomingByte = mySerial.read(); //;

        if (incomingByte != char('\r')) //Ignore characters(linereturn '\r')
        {
            serialTelegramLine += incomingByte;

            if (incomingByte == '\n')
            {
                serialTelegramString += serialTelegramLine;
                serialTelegramLine = ""; // Clear line variable
                linePosition++;          //Increase lineNumber
            }
            else if (incomingByte == '!') //LastChar
            {

                serialTelegramString += serialTelegramLine; //Store Line to full string
                serialTelegramLine = ""; // Clear variable

                int iStrLength = serialTelegramString.length();
                String sStrLength = String(iStrLength);

                if (linePosition >= LINES)
                {
                    countFail++;
                    Serial.print("\n ✗ LinePosition:" + String(linePosition));
                }
                else
                {
                    //LineLength
                    if (iStrLength == BUFFERLENGTH || iStrLength == BUFFERLENGTH - 1) //609 //666 //678
                    {
                        int foundPrintable = 0;

                        for (int charPos = 0; charPos < serialTelegramString.length(); charPos++)
                        {
                            if (isPrintable(serialTelegramString[charPos])) //" Printable:" + String(countPrintable(serialTelegramBuffer))
                            {
                                foundPrintable++;
                            };
                        };

                        if (foundPrintable < PRINTABLE)
                        {
                            countFail++;
                            Serial.print("\n ✗ Printable:" + String(foundPrintable));
                        }
                        else
                        { ////TelegramProcessLines();
                            countOk++;
                            serialTelegramBuffer = serialTelegramString;
                            Serial.println("\n 🗸 Found update");
                            gotUpdate = true;
                        };
                    }
                    else
                    {
                        countFail++;
                        Serial.print("\n ✗ iStrLength:" + String(iStrLength));
                    };
                };

                //Reset
                serialTelegramString = "";
                linePosition = -1;
            }; //End of if (incomingByte == '!')
        };     //End of if (incomingByte != char('\r'))
               //charPosition++;
    };         //End of while loop
};

void loop()
{
    readSerial();

    if (gotUpdate)
    {
        gotUpdate = false;
        Serial.print("\nUpdate: " + String(counter));

        TelegramTooLines(serialTelegramBuffer);

        counter++;
        if (counter > 9999)
        {
            counter = 0;
        }
    };
}; //End of function


void setup()
{

    mySerial.begin(115200);
    Serial.begin(115200);
    Serial.println("\n\nSerialconnections initialized");
    delay(500);

    InitItemClass();
};