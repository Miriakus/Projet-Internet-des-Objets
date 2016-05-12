/**
 * Created by Guillaume on 11/05/2016.
 */

require('../models/StatisticsObject');

var mongoose = require('mongoose');
var statisticsObject = mongoose.model('StatisticsObject');

function convertStringTimeToSec(time){
    var timeSplit = time.split(':');
    var hour = (timeSplit[0]-2) * 3600 ;
    var min = timeSplit[1] * 60;
    console.log(min);
    return hour + min;
}

var dataBaseRequests = {
    search: function(req, res){

        var date = new Date(req.body.date).getTime() / 1000;
        var begin = convertStringTimeToSec(req.body.beginHour) + date;
        var last = convertStringTimeToSec(req.body.lastHour) + date;
        var result = statisticsObject.find().where('time.sec').gt(begin).lt(last);
        result.exec(function(err, values){
            values = JSON.stringify(values);
            if(err){
                console.log('error');
                values = JSON.stringify([{}]);
            }

            res.render('dataRequest', {values: values, showGraph: true});
        });



    }
};
module.exports = dataBaseRequests;