/*
  TechTadka360 - ESP32 Servo Web Controller (SG90)
  ------------------------------------------------
  Control a servo from any phone/laptop browser. No router or internet needed:
  the ESP32 creates its own password-protected WiFi hotspot.

  Wiring:
    Servo signal (orange/yellow) -> GPIO 18
    Servo power  (red)           -> VIN / 5V
    Servo GND    (brown)         -> GND (common with ESP32)

  Library needed: "ESP32Servo" (Arduino Library Manager)

  Usage:
    1. Upload the sketch.
    2. On your phone/laptop connect to WiFi:  ESP32-Servo   (password: 12345678)
    3. Open a browser and go to:  http://192.168.4.1
*/

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// ---------- Settings ----------
const char* AP_SSID = "ESP32-Servo";   // WiFi name people will see
const char* AP_PASS = "12345678";      // WiFi password (minimum 8 characters)

const int SERVO_PIN = 18;
// ------------------------------

Servo servo;
WebServer server(80);
int currentAngle = 90;

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en-IN">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
<meta name="theme-color" content="#0a0f1c">
<title>TechTadka360 Servo Control</title>
<style>
  :root{
    --bg:#0a0f1c; --card:#111a2c; --cyan:#19d3e0; --purple:#6a4f8f;
    --text:#e8eef8; --mute:#8fa1bd; --line:#24324d;
  }
  *{box-sizing:border-box;-webkit-tap-highlight-color:transparent}
  body{
    margin:0; min-height:100vh; background:var(--bg); color:var(--text);
    font-family:system-ui,-apple-system,"Segoe UI",Roboto,sans-serif;
    display:flex; justify-content:center; padding:16px;
  }
  .wrap{width:100%; max-width:440px; display:flex; flex-direction:column; gap:14px}

  .brand{display:flex; align-items:center; gap:12px; padding:2px 4px}
  .mark{
    width:42px; height:42px; border-radius:12px; background:var(--cyan); color:#05222a;
    display:flex; align-items:center; justify-content:center; font-weight:800; font-size:17px;
  }
  .bname{font-size:20px; font-weight:800; letter-spacing:.2px}
  .bname span{color:var(--cyan)}
  .bsub{font-size:12px; color:var(--mute); margin-top:1px}

  .card{background:var(--card); border-radius:22px; padding:18px 16px}
  .head{display:flex; justify-content:space-between; align-items:flex-start}
  .lbl{font-size:11px; letter-spacing:.12em; font-weight:700; color:#6f86a8; text-transform:uppercase}
  .big{font-size:48px; font-weight:800; color:var(--cyan); line-height:1.1; margin-top:2px}
  .right{text-align:right}
  .applied{font-size:26px; font-weight:700; margin-top:6px}
  .sub{font-size:12px; color:var(--mute); margin-top:4px}
  .sub.off{color:#ff6b6b}
  svg{width:100%; height:auto; display:block; touch-action:none; user-select:none; cursor:pointer}
  .caption{text-align:center; font-size:13px; color:var(--mute); margin:4px 0}
  h2{font-size:18px; margin:0 0 14px; font-weight:700}

  input[type=range]{
    -webkit-appearance:none; appearance:none; width:100%; height:12px; border-radius:8px; outline:none; margin:10px 0;
    background:linear-gradient(to right,var(--cyan) var(--p,50%),var(--purple) var(--p,50%));
  }
  input[type=range]::-webkit-slider-thumb{
    -webkit-appearance:none; width:5px; height:38px; border-radius:3px; background:#d9f6f9; border:none;
  }
  input[type=range]::-moz-range-thumb{width:5px; height:38px; border-radius:3px; background:#d9f6f9; border:none}
  .ticks{display:flex; justify-content:space-between; font-size:13px; color:var(--mute)}

  .btns{display:flex; gap:12px; margin-top:16px}
  button{
    flex:1; height:50px; border-radius:26px; font-size:16px; font-weight:600; cursor:pointer;
    background:transparent; color:var(--text); border:1.5px solid var(--line);
  }
  button:active{transform:scale(.97)}
  button.main{flex:1.6; background:var(--cyan); color:#05222a; border-color:var(--cyan)}

  .presets{display:flex; gap:8px; margin-top:12px}
  .presets button{height:44px; border-radius:14px; font-size:14px; padding:0}
  .presets button.on{border-color:var(--cyan); color:var(--cyan)}

  .note{font-size:12px; color:var(--mute); line-height:1.5; padding:0 6px 8px}
</style>
</head>
<body>
<div class="wrap">

  <div class="brand">
    <div class="mark">TT</div>
    <div>
      <div class="bname">Tech<span>Tadka</span>360</div>
      <div class="bsub">ESP32 Servo Controller &middot; v4</div>
    </div>
  </div>

  <div class="card">
    <div class="head">
      <div>
        <div class="lbl">Selected angle</div>
        <div class="big" id="sel">90&deg;</div>
      </div>
      <div class="right">
        <div class="lbl">Applied command</div>
        <div class="applied" id="applied">--&deg;</div>
        <div class="sub" id="sub">Reported by ESP32</div>
      </div>
    </div>

    <svg id="svg" viewBox="0 0 340 245">
      <path id="track" fill="none" stroke="#1f2c44" stroke-width="6" stroke-linecap="round"/>
      <path id="active" fill="none" stroke="#19d3e0" stroke-width="6" stroke-linecap="round"/>
      <g id="ticks"></g>
      <g id="labels" fill="#8fa1bd" font-size="10" text-anchor="middle" dominant-baseline="middle"></g>

      <!-- servo body -->
      <g>
        <rect x="132" y="143" width="76" height="80" rx="6" fill="#1f6fe0"/>
        <rect x="118" y="204" width="104" height="11" rx="3" fill="#1b5fc4"/>
        <circle cx="127" cy="209.5" r="3" fill="#0a0f1c"/>
        <circle cx="213" cy="209.5" r="3" fill="#0a0f1c"/>
        <text x="170" y="192" text-anchor="middle" fill="#ffffff" font-size="13" font-weight="800">SG90</text>
        <text x="170" y="200" text-anchor="middle" fill="#bcd6ff" font-size="5">MICRO SERVO</text>
        <rect x="165" y="223" width="3" height="14" fill="#ff5a4d"/>
        <rect x="170" y="223" width="3" height="14" fill="#ffb02e"/>
        <rect x="175" y="223" width="3" height="14" fill="#8a5a2b"/>
      </g>

      <!-- horn -->
      <g id="horn">
        <path d="M0,-13 L-82,-7 A7,7 0 0 0 -82,7 L0,13 Z" fill="#f2f5fa"/>
        <circle cx="-30" cy="0" r="2.2" fill="#0a0f1c" opacity=".55"/>
        <circle cx="-45" cy="0" r="2.2" fill="#0a0f1c" opacity=".55"/>
        <circle cx="-60" cy="0" r="2.2" fill="#0a0f1c" opacity=".55"/>
        <circle cx="0" cy="0" r="14" fill="#f2f5fa"/>
        <circle cx="0" cy="0" r="6" fill="none" stroke="#b9c3d4" stroke-width="1.5"/>
        <path d="M-3,0 H3 M0,-3 V3" stroke="#b9c3d4" stroke-width="1.5"/>
      </g>

      <circle id="knob" r="5" fill="#19d3e0"/>
    </svg>

    <div class="caption">Top view &middot; single-arm horn</div>
    <div class="caption">Drag the horn or tap the arc to rotate.</div>
  </div>

  <div class="card">
    <h2>Adjust position</h2>
    <input type="range" id="slider" min="0" max="180" step="1" value="90">
    <div class="ticks"><span>0&deg;</span><span>180&deg;</span></div>

    <div class="btns">
      <button id="minus">-1&deg;</button>
      <button id="center" class="main">Centre</button>
      <button id="plus">+1&deg;</button>
    </div>

    <div class="presets">
      <button class="pre" data-a="0">0&deg;</button>
      <button class="pre" data-a="45">45&deg;</button>
      <button class="pre" data-a="90">90&deg;</button>
      <button class="pre" data-a="135">135&deg;</button>
      <button class="pre" data-a="180">180&deg;</button>
    </div>
  </div>

  <div class="note">
    The horn shows the selected angle. The command is sent to the ESP32, and the angle it
    reports back is shown under "Applied command".
  </div>

</div>

<script>
var DEG = '\u00B0';
var NS = 'http://www.w3.org/2000/svg';
var CX = 170, CY = 165, R = 115;
function $(id){ return document.getElementById(id); }
var svg = $('svg'), slider = $('slider');

function pt(a, r){
  var t = (180 - a) * Math.PI / 180;
  return [CX + r * Math.cos(t), CY - r * Math.sin(t)];
}
function arc(a){
  if (a <= 0) return '';
  var p0 = pt(0, R), p1 = pt(a, R);
  return 'M' + p0[0] + ' ' + p0[1] + ' A' + R + ' ' + R + ' 0 0 1 ' + p1[0] + ' ' + p1[1];
}
$('track').setAttribute('d', arc(180));

// ticks + labels
for (var a = 0; a <= 180; a += 6){
  var major = (a % 30 === 0);
  var q1 = pt(a, R - 9), q2 = pt(a, R - (major ? 19 : 14));
  var l = document.createElementNS(NS, 'line');
  l.setAttribute('x1', q1[0]); l.setAttribute('y1', q1[1]);
  l.setAttribute('x2', q2[0]); l.setAttribute('y2', q2[1]);
  l.setAttribute('stroke', major ? '#6f86a8' : '#2c3b58');
  l.setAttribute('stroke-width', major ? 1.6 : 1);
  $('ticks').appendChild(l);
  if (major){
    var tp = pt(a, R + 17);
    var t = document.createElementNS(NS, 'text');
    t.setAttribute('x', tp[0]); t.setAttribute('y', tp[1]);
    t.textContent = a + DEG;
    $('labels').appendChild(t);
  }
}

var angle = 90, dragging = false;
var presets = document.querySelectorAll('.pre');

function setAngle(a, doSend){
  a = Math.max(0, Math.min(180, Math.round(a)));
  angle = a;
  $('sel').textContent = a + DEG;
  slider.value = a;
  slider.style.setProperty('--p', (a / 180 * 100) + '%');
  $('horn').setAttribute('transform', 'translate(' + CX + ',' + CY + ') rotate(' + a + ')');
  $('active').setAttribute('d', arc(a));
  var k = pt(a, R);
  $('knob').setAttribute('cx', k[0]); $('knob').setAttribute('cy', k[1]);
  for (var i = 0; i < presets.length; i++){
    presets[i].classList.toggle('on', +presets[i].getAttribute('data-a') === a);
  }
  if (doSend) send(a);
}

// --- talking to the ESP32 (one request in flight at a time) ---
var busy = false, pending = null;
function setStatus(ok){
  var s = $('sub');
  s.textContent = ok ? 'Reported by ESP32' : 'ESP32 not reachable';
  s.classList.toggle('off', !ok);
}
function send(a){ pending = a; if (!busy) flush(); }
function flush(){
  if (pending === null) return;
  var a = pending; pending = null; busy = true;
  fetch('/set?angle=' + a, {cache: 'no-store'})
    .then(function(r){ return r.json(); })
    .then(function(j){ $('applied').textContent = j.angle + DEG; setStatus(true); })
    .catch(function(){ setStatus(false); })
    .then(function(){ busy = false; if (pending !== null) flush(); });
}
function poll(){
  if (dragging || busy || pending !== null) return;
  fetch('/status', {cache: 'no-store'})
    .then(function(r){ return r.json(); })
    .then(function(j){
      $('applied').textContent = j.angle + DEG; setStatus(true);
      if (j.angle !== angle) setAngle(j.angle, false);
    })
    .catch(function(){ setStatus(false); });
}

// --- controls ---
slider.addEventListener('input', function(){ setAngle(+slider.value, true); });
$('minus').onclick  = function(){ setAngle(angle - 1, true); };
$('plus').onclick   = function(){ setAngle(angle + 1, true); };
$('center').onclick = function(){ setAngle(90, true); };
for (var i = 0; i < presets.length; i++){
  presets[i].onclick = function(){ setAngle(+this.getAttribute('data-a'), true); };
}

function fromPointer(e){
  var b = svg.getBoundingClientRect();
  var s = 340 / b.width;
  var x = (e.clientX - b.left) * s - CX;
  var y = CY - (e.clientY - b.top) * s;
  var a;
  if (y < 0) a = (x < 0) ? 0 : 180;
  else a = 180 - Math.atan2(y, x) * 180 / Math.PI;
  setAngle(a, true);
}
svg.addEventListener('pointerdown', function(e){ dragging = true; svg.setPointerCapture(e.pointerId); fromPointer(e); });
svg.addEventListener('pointermove', function(e){ if (dragging) fromPointer(e); });
svg.addEventListener('pointerup', function(){ dragging = false; });
svg.addEventListener('pointercancel', function(){ dragging = false; });

setAngle(90, false);
poll();
setInterval(poll, 2500);
</script>
</body>
</html>
)rawliteral";

void handleRoot() {
  // Never let the browser keep an old copy of the page
  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.send_P(200, "text/html; charset=utf-8", PAGE);
}

void handleSet() {
  if (server.hasArg("angle")) {
    currentAngle = constrain(server.arg("angle").toInt(), 0, 180);
    servo.write(currentAngle);
  }
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", "{\"angle\":" + String(currentAngle) + "}");
}

void handleStatus() {
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", "{\"angle\":" + String(currentAngle) + "}");
}

void setup() {
  Serial.begin(115200);

  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN, 500, 2400);
  servo.write(currentAngle);

  // Offline hotspot: password protected (WPA2)
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.print("WiFi: ");
  Serial.print(AP_SSID);
  Serial.print("  |  Open: http://");
  Serial.println(WiFi.softAPIP());   // normally 192.168.4.1

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.on("/status", handleStatus);
  server.begin();
}

void loop() {
  server.handleClient();
}
