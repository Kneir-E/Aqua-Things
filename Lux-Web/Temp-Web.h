#ifndef WEBPAGE_H
#define WEBPAGE_H

const char* webpage = R"=====(
  <!DOCTYPE HTML>
  <html>
  <head>
    <!-- <meta http-equiv="refresh" content="5"> -->
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Local Temperature Sensor</title>
    <link rel="stylesheet" href="/style.css">
  </head>
  <body>
    <h1>ESP32 Wireless Light Sensor</h1>
    <p id="Message">Sensor Not Detected. Connect DS18B20 to MCU then restart.<br>&emsp;VCC to 3.3V<br>&emsp;DATA to pin 2</p>

    <div class="container">
      <div class="gauge-a"></div>
      <div class="gauge-b"></div>
      <div class="gauge-c" id="barval"></div>
      <div class="gauge-data"><h2 id="temperature">25C</h2><br>Light</div>
    </div>

    <script>
      const max_temp = 120.0;
      const min_temp = 0.0;
      let curr_temp = 0.0;

      let bar_element = document.getElementById('barval');

      function updateBar(){
        let percent = curr_temp/(max_temp-min_temp);
        bar_element.style["transform"] = `rotate(${percent*.5}turn)`;
        bar_element.style["background-color"] = getColorFromValue(percent);
      }

      async function fetchTemperature() {
        console.log("Fetching /temperature");
        fetch('/temperature')
          .then(response => response.text())
          .then(data => {
            const [status, tempVal] = data.split('-');
            console.log(`status ${status} \ttemp ${tempVal}`)
            if(status=="Detected"){
              document.getElementById('Message').innerHTML  = "Sensor is Detected";
              document.getElementById('temperature').innerHTML  = `${tempVal}C`;
              curr_temp = parseFloat(tempVal);
              updateBar();
            }else if(status=="Not_Detected"){
              document.getElementById('Message').innerHTML  = "Sensor Not Detected. Connect BH1750 to MCU then restart.<br>&emsp;VCC to 3.3V<br>&emsp;DATA to pin 2";
            }else{
              console.error('Unexpected response:', data);
            }
          })
          .catch(error => {
            console.error('There was a problem with the fetch operation:', error);
          });
      }


      function getColorFromValue(value) {
        // Define color stop points
        const lowThreshold = 15;
        const mediumThreshold = 35;
        const highThreshold = 80;
        
        // Normalize the value to a range between 0 and 100
        const normalizedValue = Math.min(Math.max(value, 1), 100);
        
        let color;
        
        if (normalizedValue <= lowThreshold) {
          // Transition from blue to cyan
          const t = normalizedValue / lowThreshold;
          const blue = interpolateColor([0, 0, 255], [0, 255, 255], t); // Blue to Cyan
          color = `rgb(${blue[0]}, ${blue[1]}, ${blue[2]})`;
        } else if (normalizedValue <= mediumThreshold) {
          // Transition from cyan to green
          const t = (normalizedValue - lowThreshold) / (mediumThreshold - lowThreshold);
          const cyan = interpolateColor([0, 255, 255], [0, 255, 0], t); // Cyan to Green
          color = `rgb(${cyan[0]}, ${cyan[1]}, ${cyan[2]})`;
        } else if (normalizedValue <= highThreshold) {
          // Transition from green to red
          const t = (normalizedValue - mediumThreshold) / (highThreshold - mediumThreshold);
          const green = interpolateColor([0, 255, 0], [255, 0, 0], t); // Green to Red
          color = `rgb(${green[0]}, ${green[1]}, ${green[2]})`;
        } else {
          // For values above highThreshold, use red
          color = 'rgb(255, 0, 0)';
        }
        
        return color;
      }

      // Helper function to interpolate between two colors
      function interpolateColor(color1, color2, t) {
        const r = Math.round(color1[0] + t * (color2[0] - color1[0]));
        const g = Math.round(color1[1] + t * (color2[1] - color1[1]));
        const b = Math.round(color1[2] + t * (color2[2] - color1[2]));
        return [r, g, b];
      }

      setInterval(fetchTemperature, 1000);
      fetchTemperature();
    </script>
  </body>
  </html>
)=====";

const char* style_css = R"=====(
  :root{
    --bg-main: #121212;
    --bg-panel: #282828;
    --active: #382bf0;
    --tx-lght: #343a40;
    --tx-dark: #f8f9fa;
  }

  body{
    height: 100vh;
    width: 100vw;
    display: flex;
    flex-direction: column;
    justify-content: flex-start;
    margin: 0;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif, "Apple Color Emoji", "Segoe UI Emoji", "Segoe UI Symbol";
    font-size: 1rem;
    font-weight: 400;
    line-height: 1.5;
    text-align: left;
    background-color: var(--bg-main);
  }

  body > *{
    width: min(85%, 400px);
    text-align: center;
    align-self: center;
  }

  body > *:last-child{
    box-sizing: border-box;
    margin-bottom: 5em;
  }

  h1{
    font-family: inherit;
    color: var(--tx-dark);
    margin-top: 1em;
    margin-bottom: 1.5em;
  }

  p{
    font-family: inherit;
    font-size: inherit;
    line-height: inherit;
    overflow: visible;
    text-decoration: none;
    text-transform: none;
    text-align: justify;
    color: var(--tx-dark);
    font-size: .8em;
    margin: 0;
  }

  .container{
    width:20em;
    height:10em;
    position: absolute;
    top: 23em;
    left: 50%;
    overflow: hidden;
    text-align: center;
    transform: translate(-50%, -50%);
  }


  .gauge-a{
    z-index: 1;
    position: absolute;
    background-color: rgba(255,255,255,.2);
    width: 20em;
    height: 10em;
    top: 0%;
    border-radius:125em 125em 0em 0em ;
  }
  .gauge-b{
    z-index: 3;
    position: absolute;
    background-color: #222;
    width: 12.5em;
    height: 6.25em;
    top: 3.75em;
    margin-left: 3.75em;
    margin-right: auto;
    border-radius:12.5em 12.5em 0em 0em ;
  }
  .gauge-c{
    z-index: 2;
    position: absolute;
    background-color: #5664F9;
    width: 20em;
    height: 10em;
    top: 10em;
    margin-left: auto;
    margin-right: auto;
    border-radius:0em 0em 10em 10em ;
      transform-origin:center top;
    transition: all .5s ease-in-out;
  }

  .gauge-data{
    z-index: 4;
    color:var(--tx-dark);
    font-size: 1.5em;
    line-height: 1.25em;
    position: absolute;
    width: 4em;
    height: 10em;
    top: 50%;
    margin:0em;
    left: 50%;
    transform: translateX(-2em);
    transition: all .5ss ease-out;
  }
)=====";

#endif