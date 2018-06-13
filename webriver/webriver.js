// A lightweight(?) javascript gui for freeliner!

// window.onload = function() {
// globals
var sendCMD, cmdPrompt, socket;
var configOptions = {};
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



function receiveWebsocketMessage (mess) {
    var json = 0;
    var config = 0;
    try{
        json = JSON.parse(mess);
        if(json === null){
            console.log(mess)
        }
        else if('config' in json){
            // console.log(json.config.ip)
            config = json.config
        }
    }
    catch(e){}
    if(config != 0){
        receiveConfig(config);
    }
}

function receiveConfig (config) {
    document.getElementById("infoline").innerHTML = config.name;

    console.log(configOptions);
}



function loadJsonUI(json){
    var maindiv = document.getElementById("maindiv");
    while (maindiv.hasChildNodes()) {
        maindiv.removeChild(maindiv.lastChild);
    }
    for(x in json.interface){
        maindiv.appendChild( makeControlBlock(json.interface[x]) )
    }
}

function makeControlBlock(block){
    var div = document.createElement("div")
    div.innerHTML = block.name;
    div.className = "controlBlock";
    // manualy update of inputs
    if(block.update === "button"){
        var inputDivArray = new Array();
        for(x in block.args){
            var input = makeControlElement(block.args[x], block, false)
            // input.name = block.args[x].name
            configOptions[block.args[x].name] = input.children[1];
            // console.log(input.children[1])
            inputDivArray.push(input);
            div.appendChild(input)
        }
        var updateDiv = document.createElement("div");
        updateDiv.className = "widget"
        updateDiv.innerHTML = "push"
        var button = document.createElement("input");
        button.innerHTML = "update";
        button.setAttribute("type", "button")
        button.onclick = function(){
            var mess = {};
            mess[block.name] = {};
            var _i
            for(x in inputDivArray){
                _p = inputDivArray[x].children[0];
                _i = inputDivArray[x].children[1];
                // console.log(_i);
                if(_i.type === "checkbox"){
                    mess[block.name][_p.innerText] = _i.checked ? 1 : 0;
                }
                else if(_i.tagName === "SELECT"){
                    // _i = inputDivArray[x].children[0];
                    console.log(inputDivArray[x])//.innerText)
                    mess[block.name][_p.innerText] = _i.options[_i.selectedIndex].value;
                }
                else {
                    mess[block.name][_p.innerText] = _i.value;
                }
            }
            console.log(JSON.stringify(mess));
            sendCMD(JSON.stringify(mess));
        }

        updateDiv.appendChild(button);
        div.appendChild(updateDiv);
    }
    // live update of inputs
    else {
        // make Callback here and pass it to the thing creation?
        for(x in block.args){
            var input = makeControlElement(block.args[x], block, true)
            div.appendChild(input)
            // add input to an array
        }
    }
    // take the array of input and create the Callback
    // pass the callback to all input.
    return div
}

function makeControlElement(property, parent, makeCallback){

    var div = document.createElement("div")
    var _p = document.createElement("p");
    _p.innerText = property.name;
    div.appendChild(_p)
    div.className = "widget"
    var input;
    switch (property.type) {
        case "toggle":
            div.appendChild(makeToggle(property, parent, makeCallback));
            break;
        case "color":
            div.appendChild(makeColor(property, parent, makeCallback));
            break;
        case "text":
            div.appendChild(makeText(property, parent, makeCallback));
            break;
        case "slider":
            div.appendChild(makeSlider(property, parent, makeCallback));
            break;
        case "bang":
            div.appendChild(makeBang(property, parent, makeCallback));
            break;
        case "select":
            div.appendChild(makeSelect(property, parent, makeCallback));
            break;
        default:
            console.log("unknown control type "+property.type)
    }
    return div;
}

function makeToggle(property, parent, makeCallback){
    var input = document.createElement("input")
    input.setAttribute("type", "checkbox")
    input.value = property.default
    if(makeCallback){
        input.onchange = function(){
            var mess = {};
            mess[parent.name] = {};
            mess[parent.name][property.name] = input.checked ? 1 : 0;
            sendCMD(JSON.stringify(mess));
        }
    }
    return input;
}

function makeText(property, parent, makeCallback){
    var input = document.createElement("input")
    input.setAttribute("type", "text")
    input.value = property.default
    if(makeCallback){
        input.oninput = function(){
            var mess = {};
            mess[parent.name] = {};
            mess[parent.name][property.name] = input.value;
            sendCMD(JSON.stringify(mess));
        }
    }
    return input;
}

function makeColor(property, parent, makeCallback){
    var input = document.createElement("input")
    input.setAttribute("type", "color")
    // input.value = property.default
    if(makeCallback){
        input.oninput = function(){
            var str = input.value;
            var mess = {};
            mess[parent.name] = {};
            mess[parent.name][property.name] = parseInt(str.replace('#',''), 16);
            sendCMD(JSON.stringify(mess));
        }
    }
    return input;
}

function makeSlider(property, parent, makeCallback){
    var input = document.createElement("input")
    input.setAttribute("type", "range")
    input.setAttribute("min", property.min)
    input.setAttribute("max", property.max)
    if(makeCallback){
        input.onchange = function(){
            var mess = {};
            mess[parent.name] = {};
            mess[parent.name][property.name] = input.value
            sendCMD(JSON.stringify(mess));
        }
    }
    // input.value = property.default
    return input;
}

function makeBang(property, parent, makeCallback){
    var input = document.createElement("input")
    input.setAttribute("type", "button")
    if(makeCallback){
        input.onclick = function(){
            var mess = {};
            mess[parent.name] = {};
            mess[parent.name][property.name] = input.value
            sendCMD(JSON.stringify(mess));
        }
    }
    return input;
}

function makeSelect(property, parent, makeCallback){
    var input = document.createElement("select")

    for(x in property.options){
        var _option = document.createElement("option");
        _option.text = property.options[x];
        input.add(_option)
        // _option.value = _modeArray[i]["index"];
        // _option.title = _modeArray[i]["description"];
    }
    // input.setAttribute("type", "button")
    if(makeCallback){
        input.onchange = function(){
            var mess = {};
            mess[parent.name] = {};
            mess[parent.name][property.name] = input.options[input.selectedIndex].value;
            sendCMD(JSON.stringify(mess));
        }
    }
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
        // populateGUItwo();
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
