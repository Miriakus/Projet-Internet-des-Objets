require('../app/models/StatisticsObject');
var net = require('net');
var mongoose = require('mongoose');
var statisticsObject = mongoose.model('StatisticsObject');

var io;

var oldTimeDataReceived = 0;

var IOTNodeConnection = {
    init : function(IO){
        io = IO;
        IOTNodeConnection.connectionToServe();
    },

    saveToDatabase : function (values) {
        if (values.time.sec - oldTimeDataReceived >= 60) {
            statisticsObject(values).save(function (err, save) {
                console.log('saved');
                if (err) return console.error('fail to save data');
            });
            oldTimeDataReceived = values.time.sec;
        }
    },

    checkData: function(data){
        data = /<start\|(.*?)\|end>/g.exec(data.toString());
        return data;
    },

    connectionToServe: function(){
        const client = net.connect(42000, '82.123.161.192');

        client.on('connect', () =>{
            console.log('connected to serve');
            client.write('CAPT_JSON_INTERVAL\n');

        });
        client.on('data', (data) => {
            data = IOTNodeConnection.checkData(data);
            if (data) {
                var values = JSON.parse(data[1]);
                io.sockets.emit('message', {data: values});
            }
            IOTNodeConnection.saveToDatabase(values);
        });

        client.on('error', function (err) {
            console.log("Connection failed");
            setTimeout(function(){
                IOTNodeConnection.connectionToServe()
            }, 10000)
        });

        client.on('end', () => {
            setTimeout(function(){
                IOTNodeConnection.connectionToServe()
            }, 10000)
        });
    }
};

module.exports = IOTNodeConnection;
