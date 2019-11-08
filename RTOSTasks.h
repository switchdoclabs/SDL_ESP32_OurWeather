// RTOS Tasks for various modes and periphials


/*

void taskSingleBlink( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreSingleBlink ) > 0)
    {

      blinkLED(Blue, 300);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);

  }

}
*/



void taskReadSensors( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreReadSensor ) > 0)
    {

      readSensors();
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);

  }

}



void taskReadWXLink( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreReadWXLink ) > 0)
    {

      readWXLinkSensors();
    }
    vTaskDelay(30000 / portTICK_PERIOD_MS);

  }

}



void taskRESTCommand( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreRESTCommand ) > 0)
    {


      // Handle REST calls
      WiFiClient client = server.available();

      int timeout;
      timeout = 0;
      if (client)
      {

        // Thank you to MAKA69 for this suggestion
        while (!client.available()) {
          Serial.print(".");
          vTaskDelay(10 / portTICK_PERIOD_MS);
          timeout++;
          if (timeout > 1000) {
            Serial.print("INFINITE LOOP BREAK!");
            break;
          }
        }
        if (client.available())
        {




          rest.handle(client);

        }
      }
      client.stop();

      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    else
    {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }


  }

}
