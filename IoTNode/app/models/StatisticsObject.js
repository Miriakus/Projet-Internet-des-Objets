/**
 * Created by Guillaume on 10/05/2016.
 */

var mongoose = require('mongoose');

var schema = new mongoose.Schema({
    cpu: {
        user: {type: Number},
        nice: {type: Number},
        system: {type: Number},
        idle: {type: Number},
        pcentUsed: {type: Number}
    },
    ram: {
        total: {type: Number},
        free: {type: Number},
        cached: {type: Number},
        used: {type: Number},
        pcentUsed: {type: Number}
    },
    swap: {
        total: {type: Number},
        free: {type: Number},
        cached: {type: Number},
        used: {type: Number},
        pcentUsed: {type: Number}
    },
    disk: {
        totalRead: {type: Number},
        totalWrite: {type: Number},
        totalTimeActive: {type: Number},
        debitRead: {type: Number},
        debitWrite: {type: Number},
        pcentActive: {type: Number}
    },
    network: {
        totalDown: {type: Number},
        totalUp: {type: Number},
        debitDown: {type: Number},
        debitUp: {type: Number}
    },
    time:{
        sec: {type:Number},
        microsec:{type:Number}
    }
});

exports.model = mongoose.model('StatisticsObject', schema);