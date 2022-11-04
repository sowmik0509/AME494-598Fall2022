var express = require("express");
var app = express();
var bodyParser = require('body-parser');
var errorHandler = require('errorhandler');
var methodOverride = require('method-override');
var hostname = process.env.HOSTNAME || 'localhost';
var port = 8080;

var volts


app.get("/", function (req, res) {
    res.redirect("index.html")
});

app.get("/sendData", function (req, res) {
    pinValue = req.query.pv;
    avgPinValue = req.query.apv;
    volts = req.query.v;
    chargeLevel = req.query.cl;
    req.query.time = new Date().getTime();
    res.end("OK");
    console.log(req.query, new Date());
});


app.get("/getData", function (req, res) {
  var ret = {}

    ret.pv = pinValue;
    ret.apv = avgPinValue;
    ret.v = volts;
    ret.cl = chargeLevel; 
    
    res.send(JSON.stringify(ret));
});

app.use(methodOverride());
app.use(bodyParser());
app.use(express.static(__dirname + '/public'));
app.use(errorHandler());

console.log("Simple static server listening at http://" + hostname + ":" + port);
app.listen(port);
