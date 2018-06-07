// A lightweight(?) javascript gui for freeliner!

// window.onload = function() {
// globals
var sendCMD, cmdPrompt, socket;

var messageIncrement = 0;

var autoScroll = true;
var DEFAULT_WEBSOCKET_ADDR = "ws://10.0.0.42/";//':8026/control';

// fetch json data
// function loadJSON(callback) {
//     var xobj = new XMLHttpRequest();
//     xobj.overrideMimeType("application/json");
//     xobj.open('GET', 'freelinerData.json', true); // Replace 'my_data' with the path to your file
//     xobj.onreadystatechange = function () {
//         if (xobj.readyState == 4 && xobj.status == "200") {
//             // Required use of an anonymous callback as .open will NOT return a value but simply returns undefined in asynchronous mode
//             callback(xobj.responseText);
//         }
//     };
//     xobj.send(null);
// }

// // called when the socket opens, this way we get fresh info from freeliner
// function populateGUI() {
//     loadJSON(function(response) {
//         populateDiv(JSON.parse(response));
//     });
// }


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
    populateGUI();

    var socket = new WebSocket(_adr);
    socket.onopen = function() {
        populateGUI();
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
    // document.getElementById("reload").onclick = function() {
    //     socket.close();
    //     console.log("closed socket");
    //     window.location.reload(true);
    // }

    return socket;
}

function receiveWebsocketMessage (mess) {
    var _splt = mess.split(" ",1);
    if(_splt[0] == "info") setInfo(mess);
    else if(_splt[0] == "fps") console.log("haha "+mess);
    // else if(_splt[0] == "template") setTemplateStat(mess);
    // else if(_splt[0] == "layers") parseLayerInfo(mess);
    // else if(_splt[0] == "files") parseAvailableFiles(mess);
    else console.log("Received ? :" + mess);
}

// generate gui for controller. retrieve name and such.
function populateGUI () {
    makeInterface();
    var maindiv = document.getElementById("maindiv");
    var _input = document.createElement("input");
    _input.setAttribute("type", "range");
    _input.setAttribute("min", 0);
    _input.setAttribute("max", 255);
    _input.setAttribute("id", "brightness");
    maindiv.appendChild(_input);
    _input.oninput = function (){
        // needs downSampling
        // sendCMD("/a/ "+Math.round(_input.value));
        // oscPort.on("ready", function () {
            var _val = Math.round(_input.value)
            // console.log(_val);
            sendOSC({
                address: "/a/",
                args: [
                    {
                        type: "i",
                        value: _val
                    },
                    {
                        type: "i", value: 42
                    }
                ]
            });
        // });
    }
}


function makeInterface(){
    var _haha = makeOSCDiv({
        address: "/foo/",
        args: [
            {
                type: "i",
                input: "range",
                min: 0,
                max: 127,
                name: "speed",
                description: "adjust playback speed"
            }

        ]
    })

    document.getElementById("maindiv").appendChild(_haha)
}

// only supports single arguments
function makeOSCDiv(json){
    var _div = document.createElement("div");
    _div.setAttribute("class", "widget");
    var _args = json.args[0];
    var _input = document.createElement("input");

    var _cb = function(){
        var _val = _input.value;
        sendOSC({
            address: json.address,
            args: [
                {
                    type: _args.type,
                    value : _val
                }
            ]
        })
    }
    if(_args.input == "range"){
        _input.setAttribute("type", "range");
        _input.oninput = _cb;
    }
    else if(args.input == "number"){
        _input.setAttribute("type", "number");
        _input.onclick = _cb;
    }
    _div.innerHTML = _args.name
    _div.title = _args.description
    _div.appendChild(_input);
    return _div;
}




// {
//     address: "/foo/",
//     args: [
//         {
//             type: "i",
//             input: "range",
//             min: 0,
//             max: 127,
//             name: "speed",
//             description: "adjust playback speed"
//         }
//
//     ]
// }


function sendOSC(message){
    oscPort.send(message);
    var _output = message.address+" ";
    for(x in message.args){
        _output += message.args[x].value+" ";
    }
    document.getElementById("logline").innerHTML = _output;
}

// fetch json data
// function loadJSON(callback) {
//     var xobj = new XMLHttpRequest();
//     xobj.overrideMimeType("application/json");
//     xobj.open('GET', 'freelinerData.json', true); // Replace 'my_data' with the path to your file
//     xobj.onreadystatechange = function () {
//         if (xobj.readyState == 4 && xobj.status == "200") {
//             // Required use of an anonymous callback as .open will NOT return a value but simply returns undefined in asynchronous mode
//             callback(xobj.responseText);
//         }
//     };
//     xobj.send(null);
// }


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
