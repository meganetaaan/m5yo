load('api_config.js');
load('api_events.js');
load('api_gpio.js');
load('api_ili9341_spi.js');
load('api_net.js');
load('api_rpc.js');
load('api_sys.js');
load('api_timer.js');
load('api_http.js');
load('api_math.js');
load('consts.js');

let BTN1 = 39, BTN2 = 38, BTN3 = 37;
let LCD_BACKLIGHT = 32;

let devID = Cfg.get('device.id');
let btnc = [-1, 0, 0, 0];
let netStatus = null;
let cloudName = null;
let cloudConnected = false;
let clearCallback = null;

Cfg.set({ wifi: { ap: { enable: false } } });  // Disable WiFi AP mode
Cfg.set({
  wifi: {
    sta: {
      enable: true,
      ssid: WIFI_SSID,
      pass: WIFI_PASSWORD
    }
  }
});
Cfg.set({ debug: { level: 3 } });            // Set debug level to 3

let getFont = ffi('void* get_font(int)');
let fonts = [getFont(0), getFont(1), getFont(2), getFont(3)];
function line(n) {
  let res = n * ILI9341.getMaxFontHeight();
  if (res < 0) res = res + 240;
  return res;
}
function clearLine(n) {
  ILI9341.setFgColor(155, 89, 182);
  ILI9341.fillRect(0, line(n), 319, ILI9341.getMaxFontHeight());
  ILI9341.setFgColor565(ILI9341.WHITE);
}

function printCentered(xc, y, text) {
  ILI9341.print(xc - ILI9341.getStringWidth(text) / 2, y, text);
}

GPIO.set_mode(LCD_BACKLIGHT, GPIO.MODE_OUTPUT);
GPIO.write(LCD_BACKLIGHT, 1);
ILI9341.setRotation(ILI9341.PORTRAIT_FLIP);
ILI9341.setBgColor(155, 89, 182);
ILI9341.setFgColor(155, 89, 182);
ILI9341.fillScreen();
ILI9341.setFont(fonts[1]);
ILI9341.setFgColor565(ILI9341.WHITE);
printCentered(160, line(0), devID);

let formatTime = ffi('char *format_time(char *)');

function printNetStatus() {
  if (!netStatus) netStatus = 'not configured';
  ILI9341.setFont(fonts[1]);
  ILI9341.setFgColor565(ILI9341.WHITE);
  ILI9341.print(5, line(1), 'WiFi: ' + netStatus + '         ');
}

function printTime() {
  ILI9341.setFont(fonts[1]);
  ILI9341.setFgColor565(ILI9341.WHITE);
  let ts = formatTime('%H:%M:%S');
  ILI9341.print(5, line(2), 'Time: ' + (ts ? ts : 'not set') + '   ');
}

function printYo() {
  // ILI9341.setFont(fonts[3]);
  // ILI9341.setFgColor565(ILI9341.WHITE);
  // let y = line(2);
  // printCentered(160, y, 'Yo')
  ILI9341.drawDIF(0, 0, '/yo.dif');
  Timer.set(1000, 0, function() {
    ILI9341.fillRect(0, 0, 320, 240);
    // clearLine(2);
  }, null);
}

function printStatus() {
  printNetStatus();
  printTime()
}

// Monitor network connectivity.
Event.addGroupHandler(Net.EVENT_GRP, function (ev, evdata, arg) {
  if (ev === Net.STATUS_DISCONNECTED) {
    netStatus = 'not connected';
  } else if (ev === Net.STATUS_CONNECTING) {
    netStatus = 'connecting';
  } else if (ev === Net.STATUS_GOT_IP) {
    netStatus = 'connected';
  }
  printNetStatus();
}, null);

function postYo(onSuccess, onError) {
  HTTP.query({
    url: 'http://api.justyo.co/yo/',
    // headers: { 'X-Foo': 'bar' },     // Optional - headers
    // data: {foo: 1, bar: 'baz'},      // Optional. If set, JSON-encoded and POST-ed
    data: {
      'api_token': API_TOKEN,
      'username': 'mgntn',
      'link': 'http://google.com'
    },
    success: onSuccess,
    error: onError
  });
}

function reportBtnPress(n) {
  postYo(
    function (body, full_http_msg) {
      print(body);
      printYo();
    },
    function (err) { print(err); }
  )
}

function rbet(from, to) {
  return Math.floor(from + (to - from) * Math.random());
}

let fromX = 240;
let toX = 320;
let fromY = 100;
let toY = 200;

function printTriangle() {
  let xs = [rbet(fromX, toX), rbet(fromX, toX), rbet(fromX, toX)];
  let ys = [rbet(fromY, toY), rbet(fromY, toY), rbet(fromY, toY)];
  ILI9341.setFgColor(rbet(0, 255), rbet(0, 255), rbet(0, 255));
  ILI9341.fillTriangle(xs[0], ys[0], xs[1], ys[1], xs[2], ys[2]);
  ILI9341.setFgColor565(ILI9341.WHITE);
}

GPIO.set_button_handler(BTN1, GPIO.PULL_UP, GPIO.INT_EDGE_NEG, 20, function () { reportBtnPress(1) }, null);
GPIO.set_button_handler(BTN2, GPIO.PULL_UP, GPIO.INT_EDGE_NEG, 20, function () { reportBtnPress(2) }, null);
GPIO.set_button_handler(BTN3, GPIO.PULL_UP, GPIO.INT_EDGE_NEG, 20, function () { reportBtnPress(3) }, null);

printStatus();
Timer.set(1000 /* 1 sec */, Timer.REPEAT, printStatus, null);
Timer.set(10 /* 1 sec */, Timer.REPEAT, printTriangle, null);
