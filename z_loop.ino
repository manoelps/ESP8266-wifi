void loop( void ) {
  // put your main code here, to run repeatedly:


	//static uint32_t loop_time = 0, loop_count = 0, loop_timeout = 0, loop_cycle = 0 ; 

if (!isOTAupdate) {

 	  mqttclient.loop();
 	//optimistic_yield(1000);
 	  yield();
  	
  	server.handleClient();
  	
  	yield();
  	
  	timer.run();
  	
  	yield();

  	loop_Plugin();

  	yield();

 // #ifdef MDNSSERVICE
 //   	MDNS.update();
 // #endif

//if (wifimode == 1) mdns.update();  

//  Async EEPROM write... wait 500mSec
//
	if (EEPROM_commit_var == true) {
		static unsigned long eepromcommit_timer = 0;
		if (eepromcommit_timer == 0 ) eepromcommit_timer = millis();
			if (millis() - eepromcommit_timer > 200) { // wait two seconds to save changes...
			EEPROM.commit(); // takes 35msec to perform. 
			eepromcommit_timer = 0;
			EEPROM_commit_var = false;
	//		Debugln("EEPROM written!"); 
			}	 
	}

  }; // END of NOT OTA check!  

//delay(1); /// test loop... see how it goes... 
//loop_count++;

// if (millis() - loop_timeout > 500) {
// //Serial.println(ESP.getFreeHeap());
//  //uint32_t temp = (ESP.getCycleCount() - loop_cycle ) / loop_count ; 
// loop_timeout = millis();
// Debug(".");
// };


//loop_count = loop_count / 10 ; 

 //Serial.printf( "Loops per second = %u,  Average cycles per loop = %u \n", loop_count , temp  ); 


 //loop_cycle = ESP.getCycleCount(); 
 //loop_count = 0;
 //loop_timeout = millis();
//}

 
OTA_UPDATE();

yield(); 

}

