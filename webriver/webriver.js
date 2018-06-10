// A lightweight(?) javascript gui for freeliner!

// window.onload = function() {
// globals
var sendCMD, cmdPrompt, socket;

var messageIncrement = 0;

var autoScroll = true;
var DEFAULT_WEBSOCKET_ADDR = "ws://10.0.0.42/";//':8026/control';

populateGUI();
// fetch json data
function loadJSON(callback) {
    var xobj = new XMLHttpRequest();
    xobj.overrideMimeType("application/json");
    xobj.open('GET', 'river_setup.json', true); // Replace 'my_data' with the path to your file
    xobj.onreadystatechange = function () {
        if (xobj.readyState == 4 && xobj.status == "200") {
            // Required use of an anonymous callback as .open will NOT return a value but simply returns undefined in asynchronous mode
            callback(xobj.responseText);
        }
    };
    xobj.send(null);
}

// called when the socket opens, this way we get fresh info from freeliner
function populateGUI() {

    loadJSON(function(response) {
        loadJsonUI(JSON.parse(response));
    });
}

function loadJsonUI(json){
    var maindiv = document.getElementById("maindiv");
    for(x in json.interface){
        maindiv.appendChild( makeControlBlock(json.interface[x]) )
    }
}

function makeControlBlock(block){
    var div = document.createElement("div")
    div.innerHTML = block.name;
    div.className = "controlBlock";
    // make Callback here and pass it to the thing creation?
    for(x in block.args){
        var input = makeControlElement(block.args[x])
        div.appendChild(input)
        // add input to an array
    }
    // take the array of input and create the Callback
    // pass the callback to all input.
    return div
}

function makeControlElement(property){
    var div = document.createElement("div")
    div.innerHTML = property.name
    div.className = "widget"
    var input;
    switch (property.type) {
        case "toggle":
            div.appendChild(makeToggle(property));
            break;
        case "color":
            div.appendChild(makeColor(property));
            break;
        case "text":
            div.appendChild(makeText(property));
            break;
        case "slider":
            div.appendChild(makeSlider(property));
            break;
        case "bang":
            div.appendChild(makeBang(property));
            break;
        default:
            console.log("unknown control type "+property.type)
    }
    return div;
}

function makeToggle(property){
    console.log(property.root)
    var input = document.createElement("input")
    input.setAttribute("type", "checkbox")
    input.value = property.default
    return input;
}

function makeText(property){
    var input = document.createElement("input")
    input.setAttribute("type", "text")
    input.value = property.default
    return input;
}

function makeColor(property){
    var input = document.createElement("input")
    input.setAttribute("type", "color")
    // input.value = property.default
    return input;
}

function makeSlider(property){
    var input = document.createElement("input")
    input.setAttribute("type", "range")
    input.setAttribute("min", property.min)
    input.setAttribute("max", property.max)
    // input.value = property.default
    return input;
}

function makeBang(property){
    var input = document.createElement("input")
    input.setAttribute("type", "button")
    return input;
}



// _input.oninput = function (){
//     // needs downSampling
//     var message = { config : {
//             ip : "10.0.0.45",
//             name : "riverFromSpace",
//             DHCP : 0
//         }
//     }
//     console.log(message);
//     sendCMD(JSON.stringify(message));
/*
* /////////////////////////////////////////////////////////////
* main or whatever
* /////////////////////////////////////////////////////////////
*/

// ping websocket every 10 secs to prevent timeout
setInterval(function() {
    // actualySendCMD('ping');

}, 1000);


/*
* /////////////////////////////////////////////////////////////
* webSocket!
* /////////////////////////////////////////////////////////////
*/

function sendCMD(_cmd) {
    actualySendCMD(_cmd);
    // display it!
    document.getElementById("logline").innerHTML = _cmd;
}

// make a function to send commands through a websocket
actualySendCMD = (function () {
    var socket, _addr;
    _addr = prompt("connect to", DEFAULT_WEBSOCKET_ADDR);
    if (_addr != null) socket = makeSocket(_addr);
    else socket = makeSocket(DEFAULT_WEBSOCKET_ADDR);
    return function (_cmd) {
        if(socket.readyState) socket.send(_cmd);
    }
})();

// make a websocket
function makeSocket(_adr) {
    // oscPort = new osc.WebSocketPort({
    //     url: _adr, // URL to your Web Socket server.
    //     metadata: true
    // });
    // oscPort.open();
    // oscPort.on("message", function (oscMsg) {
    //     console.log("An OSC message just arrived!", oscMsg);
    // });
    // populateGUI();

    var socket = new WebSocket(_adr);
    socket.onopen = function() {
        populateGUI();
        populateGUItwo();
    }
    socket.onmessage = function (evt) {
        // parseInfo(evt.data);
        // var _messageJSON = JSON.parse(evt.data);
        receiveWebsocketMessage(evt.data);
        document.getElementById("logline").innerHTML = evt.data;
    }
    // socket.onclose = function () {};
    // socket.onclose = function () {socket.close(); console.log("closing socket??")}; // disable onclose handler first

    //
    // window.onbeforeunload = function() {
    //     socket.close();
    // };
    document.getElementById("reload").onclick = function() {
        socket.onclose = function () {}; // disable onclose handler first
        socket.close();
        console.log("closed socket");
        // window.location.reload(true);
    }

    return socket;
}

function receiveWebsocketMessage (mess) {
    var json = 0;
    try{
        json = JSON.parse(mess);
        if(json === null){
            console.log(mess)
        }
        else {
            console.log(json.config.ip)
            document.getElementById("infoline").innerHTML = json.config.name;
        }
    }
    catch(e){}

}



cmdPrompt = (function () {
    var cmdIndex, cmdHistory, cmdPrompt;
    cmdHistory = ["no previous cmd"];
    prompt = document.getElementById("prompt");
    return function (e) {
        if(e.keyCode == 13) {
            sendCMD(prompt.value);
            cmdHistory.push(prompt.value);
            prompt.value = "";
            cmdIndex = 0;
        }
        else if(e.keyCode == 38) {
            cmdIndex++;
            if(cmdIndex >= cmdHistory.length) cmdIndex = cmdHistory.length-1;
            prompt.value = cmdHistory[cmdHistory.length-1];
            prompt.value = cmdHistory[cmdHistory.length - cmdIndex];
        }
        else if(e.keyCode == 40) {
            cmdIndex--;
            if(cmdIndex < 1) cmdIndex = 1;
            prompt.value = cmdHistory[cmdHistory.length - cmdIndex];
        }
    }
})();

document.addEventListener("keydown", function(e) {
    if (document.activeElement == document.getElementById("prompt")) cmdPrompt(e);
}, false);

// }
