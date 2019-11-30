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

// Pixel Task and routines

/*

  // simple blend function
  void BlendAnimUpdate(const AnimationParam& param)
  {
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbwColor updatedColor = RgbwColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        param.progress);

    // apply the color to the strip
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        strip.SetPixelColor(pixel, updatedColor);
    }
  }

  void FadeInFadeOutRinseRepeat(float luminance)
  {
    if (fadeToColor)
    {
        // Fade upto a random color
        // we use HslColor object as it allows us to easily pick a hue
        // with the same saturation and luminance so the colors picked
        // will have similiar overall brightness
        RgbwColor target = HslColor(random(360) / 360.0f, 1.0f, luminance);
        uint16_t time = random(800, 2000);

        animationState[0].StartingColor = strip.GetPixelColor(0);
        animationState[0].EndingColor = target;

        animations.StartAnimation(0, time, BlendAnimUpdate);
    }
    else
    {
        // fade to black
        uint16_t time = random(600, 700);

        animationState[0].StartingColor = strip.GetPixelColor(0);
        animationState[0].EndingColor = RgbwColor(0);

        animations.StartAnimation(0, time, BlendAnimUpdate);
    }

    // toggle to the next effect state
    fadeToColor = !fadeToColor;
  }
*/

void taskPixelCommand( void * parameter)
{
  int i = 0;


  // Enter RTOS Task Loop
  for (;;)  {

    for (i = 5; i < 150; i++)
    {

      strip.SetPixelColor(0, RgbwColor(0, 0, i, 0));

      strip.Show();
      vTaskDelay(10 / portTICK_PERIOD_MS);

    }

    for (i = 150; i > 5; i--)
    {
      strip.SetPixelColor(0, RgbwColor(0, 0, i, 0));
      strip.Show();
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }




  }
}
