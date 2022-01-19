/*********************************************************************************
 *  MIT License
 *
 *  Copyright (c) 2022 IlijaVorontsov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *  
 ********************************************************************************/

struct DEV_TemperatureSensor : Service::TemperatureSensor {     

  SpanCharacteristic *temp;                         
  uint32_t timer = 0;                                

    DEV_TemperatureSensor() : Service::TemperatureSensor() {      
    double tempC = mySensor.getTemperature();
             
    temp=new Characteristic::CurrentTemperature(tempC);      
    temp->setRange(-50,100);                                  
        
    Serial.printf("Configuring Temperature Sensor SCD41: %0.2f C.\n",tempC);           
    
  }

  void loop() {

    char c[64];

    if (millis() - timer > 5000) {          
      
      double tempC = mySensor.getTemperature();
      temp->setVal(tempC);                                
      sprintf(c,"SCD41 Temperature Update: %g\n",tempC);
      LOG1(c);
      
    }

  }

};

struct DEV_HumiditySensor : Service::HumiditySensor {   

  SpanCharacteristic *humid;                         
  uint32_t timer = 0;                               

  DEV_HumiditySensor() : Service::HumiditySensor() {      
    double humidP = mySensor.getHumidity();
             
    humid=new Characteristic::CurrentRelativeHumidity(humidP);       
        
    Serial.printf("Configuring Humidity Sensor SCD41: %f0.2f %%.\n",humidP);          
    
  }

  void loop() {

    char c[64];

    if (millis() - timer > 5000) {         
      timer = millis();

      double humidP = mySensor.getHumidity();

     
      humid->setVal(humidP);                           
      sprintf(c,"SCD41 Humidity Update: %g\n",humidP);
      LOG1(c);
    }
  }

};


struct DEV_CO2Sensor : Service::CarbonDioxideSensor {   

  SpanCharacteristic *co2level;
  SpanCharacteristic *co2abnormal;                       
  uint32_t timer = 0;                               

  DEV_CO2Sensor() : Service::CarbonDioxideSensor() {      
    double co2ppm = mySensor.getCO2();
    
    if(co2ppm < 1000) co2abnormal=new Characteristic::CarbonDioxideDetected(0);
    else co2abnormal=new Characteristic::CarbonDioxideDetected(1);
    
    co2level=new Characteristic::CarbonDioxideLevel(co2ppm);       
        
    Serial.printf("Configuring Carbon Dioxide Sensor SCD41: %f ppm.\n",co2ppm);          
    
  }

  void loop() {

    char c[64];

    if (millis() - timer > 5000) {          
      timer = millis();

      double co2ppm = mySensor.getCO2();

      if(co2ppm < 1000) co2abnormal->setVal(0);
      else if (co2ppm > 1100) co2abnormal->setVal(1);
      
      co2level->setVal(co2ppm);                         
      sprintf(c,"SCD41 Carbon Dioxide Update: %g\n",co2ppm);
      LOG1(c);
    }
  }

};
